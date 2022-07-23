#pragma once

#include "loginfo.hpp"
#include "kltime.hpp"
#include <map>
#include <string_view>

class LogDatabase {
public:
  void AddLogStatement(std::string_view log);
  void AddLogFile(const std::string& path);

private:
  std::multimap<kl::DateTime, LogInfo> m_logs;
};
