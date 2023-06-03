#pragma once
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> splitHeaders(const std::string &str) {
  std::string token;
  std::vector<std::string> tokens;
  for (int i = 0;; ++i) {
    if (str[i] != '\r') {
      token += str[i];
    } else {
      ++i;
      tokens.push_back(token);
      token.clear();
      if (str[i + 1] == '\r') {
        break;
      }
    }
  }
  return tokens;
}

std::vector<std::string> splitLine(const std::string &str) {
  std::vector<std::string> tokens;
  auto pos = str.find(':');
  tokens.push_back(str.substr(0, pos));
  tokens.push_back(str.substr(pos + 2));
  return tokens;
}
