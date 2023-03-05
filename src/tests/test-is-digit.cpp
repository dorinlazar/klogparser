#include "customparsers.hpp"
#include <iostream>
#include <fmt/format.h>
#include <chrono>

constexpr bool is_digit1(char c) {
  return 0x1FULL >> (unsigned)((((c & 0x7E) ^ 0x30) >> 1) | ((c & 0x80) >> 3)) & 0x3F;
}
constexpr bool is_digit2(char c) { return (unsigned __int128)0x1F >> (unsigned)((c ^ 0x30) >> 1); }
constexpr bool is_digit3(char c) { return !((c | 0x07) ^ 0x37) || !((c | 0x01) ^ 0x39); }
// return !((c&0xFE)^0x38)||!((c&0xF8)^0x30);
// return !(((c & 0xFE)^0x30)&0xF8);
// !((c | 0x07) ^ 0x37) || !((c | 0x01) ^ 0x39)
int main() {
  for (int c = 0; c < 256; c++) {
    std::cout << fmt::format("{0:#04x} {0:08b} {1:>5}", c, Parser::is_digit(c)) << std::endl;
  }
  size_t size = 100'000'000;
  auto buffer = std::make_unique_for_overwrite<char[]>(size);

  for (size_t i = 0; i < size; i++) {
    buffer[i] = (i * 71) & 0xFF;
  }

  auto start = std::chrono::system_clock::now();
  size_t count1 = 0;
  for (size_t i = 0; i < size; i++) {
    count1 += Parser::is_digit(buffer[i]);
  }
  auto end = std::chrono::system_clock::now();

  size_t count2 = 0;
  for (size_t i = 0; i < size; i++) {
    count2 += is_digit1(buffer[i]);
  }
  auto end2 = std::chrono::system_clock::now();

  size_t count3 = 0;
  for (size_t i = 0; i < size; i++) {
    count3 += is_digit2(buffer[i]);
  }

  auto end3 = std::chrono::system_clock::now();

  size_t count4 = 0;
  for (size_t i = 0; i < size; i++) {
    count4 += is_digit3(buffer[i]);
  }

  auto end4 = std::chrono::system_clock::now();

  auto nanos1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  auto nanos2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - end).count();
  auto nanos3 = std::chrono::duration_cast<std::chrono::nanoseconds>(end3 - end2).count();
  auto nanos4 = std::chrono::duration_cast<std::chrono::nanoseconds>(end4 - end3).count();
  std::cout << fmt::format("{:>10}  --  {}", count1, nanos1) << std::endl;
  std::cout << fmt::format("{:>10}  --  {}", count2, nanos2) << std::endl;
  std::cout << fmt::format("{:>10}  --  {}", count3, nanos3) << std::endl;
  std::cout << fmt::format("{:>10}  --  {}", count4, nanos4) << std::endl;
  std::cout << fmt::format("{}  --  {}", Parser::parse_int<uint32_t>("15542"), Parser::parse_int<int32_t>("-15542"))
            << std::endl;
  return 0;
}
