#include "stringdb.hpp"

Token TokenGenerator::AllocateNewToken() { return m_next_token++; }

Token StringDatabase::RegisterString(const std::string& str) {
  auto it = m_cache.find(str);
  if (it != m_cache.end()) {
    return it->second;
  }
  auto token = m_token_generator.AllocateNewToken();
  m_cache.insert({str, token});
  return token;
}
