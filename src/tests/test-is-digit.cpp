#include "customparsers.hpp"
#include <iostream>
#include <fmt/format.h>

int main()
{
  for (int c=0; c<256; c++) {
    std::cout << fmt::format("{0:#04x} {0:08b} {1:>5} {2:08b} {3:08b} {3:03d} {4} ",
    c, Parser::is_digit(c), ((c&0xFE)^0x38), (((c & 0x7E)^0x30)>>1)|((c&0x80)>>3), 0x0FULL >> (unsigned)(((c & 0x7E)^0x30)>>1)|((c&0x80)>>3) ) <<std::endl;
  }
  return 0;
}
