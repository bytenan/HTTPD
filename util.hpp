#pragma once
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

bool readFile(const std::string &path, char *buf, int size) {
  std::ifstream in(path, std::ios::in | std::ios::binary);
  if (!in) {
    std::cout << "readFile error" << std::endl;
    return false;
  }
  in.read(buf, size);
  in.close();
  return true;
}

std::string getMIME(const std::string &suffix) {
  if (suffix == ".html") return "text/html";
  if (suffix == ".jpg") return "application/x-jpg;image/jpeg";
  if (suffix == ".png") return "application/x-png;image/png";
  if (suffix == ".ico") return "application/x-ico;image/x-icon;";
  if (suffix == ".js") return "application/x-javascript";
  if (suffix == ".svg") return "text/xml";
  if (suffix == ".css") return "text/css";
  return "text/html";
}