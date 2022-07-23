#pragma once

#include "loginfo.hpp"
#include "kltime.hpp"
#include <map>

class LogDatabase {
public:
  void AddLogStatement(const std::string& log);

private:
  std::multimap < DateTime
};
