#include <iostream>
#include "stringdb.hpp"
#include "logdb.hpp"

int main() {
  std::string line;
  LogDatabase logdb;
  while (std::getline(std::cin, line)) {
    logdb.AddLogStatement(line);
  }
  return 0;
}
