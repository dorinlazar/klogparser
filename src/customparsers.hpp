#pragma once
#include <concepts>
#include <string_view>
#include <array>

struct Parser {
static constexpr bool is_digit (char c)  {
  //return !((c&0xFE)^0x38)||!((c&0xF8)^0x30);
  //return !(((c & 0xFE)^0x30)&0xF8);
  // return 0x1FULL >> (unsigned)((((c & 0x7E)^0x30)>>1)|((c&0x80)>>3) )&0x3F;
  return (unsigned char)(c-'0')<10;
}

static constexpr bool is_digit1(char c) {
return ('0' <= c) && (c <= '9');
}

static constexpr bool is_digit2(char c) {
return (unsigned char)(c - 0x3A) > 0xF5;
}

static constexpr bool is_digit3(char c) {
  constexpr std::array<bool, 256> precalc {
    false, false, false, false,false, false, false, false, false, false, false, false,false, false, false, false,
    false, false, false, false,false, false, false, false, false, false, false, false,false, false, false, false,
    false, false, false, false,false, false, false, false, false, false, false, false,false, false, false, false,
    true, true, true, true,true, true, true, true, true, true, false, false,false, false, false, false,
    false, false, false, false,false, false, false, false, false, false, false, false,false, false, false, false,
    false, false, false, false,false, false, false, false, false, false, false, false,false, false, false, false,
    false, false, false, false,false, false, false, false, false, false, false, false,false, false, false, false,
    false, false, false, false,false, false, false, false, false, false, false, false,false, false, false, false,
    false, false, false, false,false, false, false, false, false, false, false, false,false, false, false, false,
    false, false, false, false,false, false, false, false, false, false, false, false,false, false, false, false,
    false, false, false, false,false, false, false, false, false, false, false, false,false, false, false, false,
    false, false, false, false,false, false, false, false, false, false, false, false,false, false, false, false,
    false, false, false, false,false, false, false, false, false, false, false, false,false, false, false, false,
    false, false, false, false,false, false, false, false, false, false, false, false,false, false, false, false,
    false, false, false, false,false, false, false, false, false, false, false, false,false, false, false, false,
    false, false, false, false,false, false, false, false, false, false, false, false,false, false, false, false,
  };
  return precalc[(unsigned)c];
}

// template <typename T>
// requires (std::is_integral<T>::value)
// T parse(std::string_view value) noexcept {
//   if (value.size()==0) {return 0;}
//   size_t offset = 0;
//   if constexpr (std::is_signed<T>::value) {
//     if 
//   }
// }

};