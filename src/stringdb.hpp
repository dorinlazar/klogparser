#pragma once
#include <string>
#include <unordered_map>
#include <cstdint>

using Token = uint32_t;

class TokenGenerator {
public:
  Token AllocateNewToken();

private:
  Token m_next_token = 0;
};

class StringDatabase {
public:
  Token RegisterString(const std::string& str);

private:
  std::unordered_map<std::string, Token> m_cache;
  TokenGenerator m_token_generator;
};