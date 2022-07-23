#pragma once

#include <string_view>

class StringReader {
public:
  explicit StringReader(std::string_view view);

  std::string_view ReadUntil(char c);
  void Expect(char c);

private:
  std::string_view m_view;
};
