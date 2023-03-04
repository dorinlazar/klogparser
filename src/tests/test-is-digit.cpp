#include "customparsers.hpp"
#include <iostream>
#include <fmt/format.h>
#include <chrono>

int main() {
  for (int c = 0; c < 256; c++) {
    std::cout << fmt::format("{0:#04x} {0:08b} {1:>5}", c, Parser::is_digit(c)) << std::endl;
  }
  size_t size = 100'000'000;
  auto buffer = std::make_unique_for_overwrite<char[]>(size);
  size_t count1 = 0;

  for (size_t i = 0; i < size; i++) {
    buffer[i] = (i * 71) & 0xFF;
  }

  auto start = std::chrono::system_clock::now();
  for (size_t i = 0; i < size; i++) {
    count1 += Parser::is_digit(buffer[i]);
  }

  auto end = std::chrono::system_clock::now();

  auto nanos1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  std::cout << fmt::format("{}  --  {}", count1, nanos1) << std::endl;
  std::cout << fmt::format("{}  --  {}", Parser::parse_int<uint32_t>("15542"), Parser::parse_int<int32_t>("-15542"))
            << std::endl;
  return 0;
}
