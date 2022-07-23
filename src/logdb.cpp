#include "logdb.hpp"

#include <ctre.hpp>
#include <iostream>

//    '$remote_addr - $remote_user [$time_local] "$request" ' '$status $body_bytes_sent "$http_referer" '
//    '"$http_user_agent" "$http_x_forwarded_for"';
// date/time format: 06/Jul/2022:06:32:41 +0300
auto combined_format =
    ctre::match<R"%(([0-9a-fA-F\.]+) - [^ ]+ \[([^\]]+)\] "([^"]+)" ([0-9]+) ([0-9]+) "([^"]+)" "([^"]+)".*)%">;

enum class TimeZoneDiffDirection { Plus, Minus };

void LogDatabase::AddLogStatement(const std::string& line) {
  auto parsed = combined_format(line);
  if (parsed.get<0>()) {
    LogInfo li;
    li.remote_addr = parsed.get<1>();
    // TimeZoneDiffDirection tzsign =
    //     parsed.get<8>()[0] == '+' ? TimeZoneDiffDirection::Plus : TimeZoneDiffDirection::Minus;
    // uint32_t tzhours = (parsed.get<9>()[0] - '0') * 10 + parsed.get<9>()[1] - '0';
    // uint32_t tzminutes = (parsed.get<10>()[0] - '0') * 10 + parsed.get<10>()[1] - '0';
    auto res = kl::DateTime::Parse(std::string(parsed.get<2>()));
    if (!res.has_value()) {
      return;
    }
    li.time_local = *res;
    li.request = parsed.get<3>();
    li.status = std::stoul(std::string(parsed.get<4>()));
    li.body_bytes_sent = std::stoul(std::string(parsed.get<5>()));
    li.http_referer = parsed.get<6>();
    li.http_user_agent = parsed.get<7>();
    std::cout << li.http_user_agent << std::endl;
  }
}

void LogDatabase::AddLogFile([[maybe_unused]] const std::string& path) {}
