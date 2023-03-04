#pragma once
#include <concepts>
#include <string_view>

struct Parser {
static constexpr bool is_digit (char c)  {
//  return !((c&0xFE)^0x38)||!((c&0xF8)^0x30);
  return !(((c & 0xFE)^0x30)&0xF8);
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