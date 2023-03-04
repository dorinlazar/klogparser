#include "customparsers.hpp"
#include <iostream>
#include <fmt/format.h>
#include <chrono>

int main()
{
  for (int c=0; c<256; c++) {
    std::cout << fmt::format("{0:#04x} {0:08b} {1:>5} {2:08b} {3:08b} {3:03d} {4} ",
    c, Parser::is_digit(c), ((c&0xFE)^0x38), (((c & 0x7E)^0x30)>>1)|((c&0x80)>>3), 0x1FULL >> (unsigned)((((c & 0x7E)^0x30)>>1)|((c&0x80)>>3) )&0x3F) <<std::endl;
  }
  size_t size = 100'000'000;
  auto buffer= std::make_unique_for_overwrite<char[]>(size);
  size_t count1 = 0, count2 = 0, count3=0;

  auto start = std::chrono::system_clock::now();
  for (size_t i=0; i<size; i++) {
    count1 += Parser::is_digit1(buffer[i]);
  }
  
  auto end1 = std::chrono::system_clock::now();

  for (size_t i=0; i<size; i++) {
    count2 += Parser::is_digit2(buffer[i]);
  }

  auto end2 = std::chrono::system_clock::now();

  for (size_t i=0; i<size; i++) {
    count3 += Parser::is_digit(buffer[i]);
  }

  auto end3 = std::chrono::system_clock::now();

  auto nanos1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end1-start).count();
  auto nanos2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end2-end1).count();
  auto nanos3 = std::chrono::duration_cast<std::chrono::nanoseconds>(end3-end2).count();
  std::cout << fmt::format("{}  --  {}", count1, nanos1) << std::endl;
  std::cout << fmt::format("{}  --  {}", count2, nanos2) << std::endl;
  std::cout << fmt::format("{}  --  {}", count3, nanos3) << std::endl;
  return 0;
}
