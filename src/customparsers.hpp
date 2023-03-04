#pragma once
#include <concepts>
#include <string_view>
#include <array>

struct Parser {
  static constexpr bool is_digit(char c) { return (unsigned char)(c - '0') < 10; }

  template <typename T>
    requires(std::is_integral<T>::value)
  static constexpr T parse_int(std::string_view value) noexcept {
    if (value.size() == 0) {
      return 0;
    }
    size_t offset = 0;
    bool negative = false;
    if constexpr (std::is_signed<T>::value) {
      if (value[0] == '-') {
        negative = true;
        offset++;
      } else if (value[0] == '+') {
        offset++;
      }
    }
    T result = 0;
    while (offset < value.size()) {
      if (!is_digit(value[offset])) {
        break;
      }
      result = result * 10 + value[offset] - '0';
      offset++;
    }
    if constexpr (std::is_signed<T>::value) {
      if (negative) {
        return -result;
      }
    }
    return result;
  }
};
