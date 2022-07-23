#include <ctre.hpp>
#include <iostream>
#include "stringdb.hpp"
#include "loginfo.hpp"

int main() {
  std::string line;
  //    '$remote_addr - $remote_user [$time_local] "$request" ' '$status $body_bytes_sent "$http_referer" '
  //    '"$http_user_agent" "$http_x_forwarded_for"';
  auto m = ctre::match<R"%(([0-9a-fA-F\.]+) - [^ ]+ \[([^\]]+)\] "([^"]+)" ([0-9]+) ([0-9]+) "([^"]+)" "([^"]+)".*)%">;
  while (std::getline(std::cin, line)) {
    auto parsed = m(line);
    if (parsed.get<0>()) {
      LogInfo li;
      li.remote_addr = parsed.get<1>();
      li.time_local = parsed.get<2>();
      li.request = parsed.get<3>();
      li.status = std::stoul(std::string(parsed.get<4>()));
      li.body_bytes_sent = std::stoul(std::string(parsed.get<5>()));
      li.http_referer = parsed.get<6>();
      li.http_user_agent = parsed.get<7>();
      std::cout << li.http_user_agent << std::endl;
    }
  }
  return 0;
}
