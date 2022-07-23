#pragma once

#include "stringdb.hpp"
#include "kltime.hpp"

struct LogInfo {
  std::string remote_addr;
  kl::DateTime time_local;
  std::string request;
  uint32_t status;
  uint32_t body_bytes_sent;
  std::string http_referer;
  std::string http_user_agent;
};
