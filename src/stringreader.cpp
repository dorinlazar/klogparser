#include "stringreader.hpp"
#include <stdexcept>

StringReader::StringReader(std::string_view view) : m_view(view) {}

std::string_view StringReader::ReadUntil(char c) {
  size_t offset = 0;
  while (offset < m_view.size() && m_view[offset] != c) {
    offset++;
  }
  auto res = m_view.substr(0, offset);
  m_view = m_view.substr(offset + 1);
  return res;
}

void StringReader::Expect(char c) {
  if (m_view.size() > 0 && m_view[0] == c) {
    m_view = m_view.substr(1);
  } else {
    throw std::runtime_error("Invalid character while scanning");
  }
}
