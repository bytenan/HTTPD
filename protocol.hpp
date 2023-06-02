#include <iostream>
#include <map>
#include <string>

using Headers = std::multimap<std::string, std::string>;

struct Request {
  std::string method;
  std::string path;
  std::string version;
  Headers headers;
  //   std::string blank;
  std::string body;

  void Parse(const std::string &str) {}
};

struct Response {
  std::string version;
  std::string status;
  std::string reason;
  Headers headers;
};