#include "logdb.hpp"
#include "stringreader.hpp"
#include <iostream>

//    '$remote_addr - $remote_user [$time_local] "$request" ' '$status $body_bytes_sent "$http_referer" '
//    '"$http_user_agent" "$http_x_forwarded_for"';
void LogDatabase::AddLogStatement(std::string_view line) {
  LogInfo li;

  StringReader reader(line);

  try {

    li.remote_addr = reader.ReadUntil(' ');

    reader.Expect('-');
    reader.Expect(' ');

    reader.ReadUntil(' '); // $remote_user

    reader.Expect('[');
    auto res = kl::DateTime::Parse(reader.ReadUntil(']'));
    if (!res.has_value()) {
      return;
    }
    li.time_local = *res;
    reader.Expect(' ');
    reader.Expect('"');

    li.request = reader.ReadUntil('"');
    reader.Expect(' ');

    li.status = std::stoul(std::string(reader.ReadUntil(' ')));
    li.body_bytes_sent = std::stoul(std::string(reader.ReadUntil(' ')));
    reader.Expect('"');
    li.http_referer = reader.ReadUntil('"');
    reader.Expect(' ');
    reader.Expect('"');
    li.http_user_agent = reader.ReadUntil('"');
    std::cout << li.http_user_agent << std::endl;
  } catch (const std::exception& ex) {
    std::cout << "Bad line : " << line << std::endl;
    return;
  }
}

void LogDatabase::AddLogFile([[maybe_unused]] const std::string& path) {}
