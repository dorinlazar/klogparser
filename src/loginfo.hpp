#pragma once

#include "stringdb.hpp"
#include "kltime.hpp"

struct LogInfo {
  std::string remote_addr;
  std::string time_local;
  std::string request;
  uint32_t status;
  uint32_t body_bytes_sent;
  Token http_referer;
  Token http_user_agent;
};
