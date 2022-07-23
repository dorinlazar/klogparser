#pragma once

#include "loginfo.hpp"
#include "kltime.hpp"
#include <map>

class LogDatabase {
public:
  void AddLogStatement(const std::string& log);
  void AddLogFile(const std::string& path);

private:
  std::multimap<kl::DateTime, LogInfo> m_logs;
};
