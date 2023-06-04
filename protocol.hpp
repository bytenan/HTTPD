#pragma once
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "util.hpp"

const std::string sep = "\r\n";

const std::string page_root = "./wwwroot";
const std::string page_home = "./wwwroot/index.html";
const std::string page_404 = "./wwwroot/404.html";

using Headers = std::multimap<std::string, std::string>;

struct Request {
  std::string method;
  std::string url;
  std::string version;
  Headers headers;

  std::string path;
  std::string suffix;
  int size;
  //   std::string blank;
  // std::string body;

  void Parse(const std::string &input) {
    std::vector<std::string> lines = splitHeaders(input);
    std::stringstream ss(lines[0]);
    ss >> method >> url >> version;

    for (int i = 1; i < lines.size(); ++i) {
      std::vector<std::string> parts = splitLine(lines[i]);
      headers.insert(std::make_pair(parts[0], parts[1]));
    }

    if (url[url.size() - 1] == '/') {
      path = page_home;
    } else {
      path = page_root + url;
    }

    auto pos = path.rfind('.');
    if (pos == std::string::npos) {
      suffix = ".html";
    } else {
      suffix = path.substr(pos);
    }

    struct stat st;
    int n = stat(path.c_str(), &st);
    if (!n) {
      size = st.st_size;
    } else {
      struct stat st1;
      stat(page_404.c_str(), &st1);
      size = st1.st_size;
    }
  }
};

struct Response {
  std::string version;
  std::string status;
  std::string reason;
  // Headers headers;
  std::string headers;
  std::string body;
  void Serialize(std::string *output) {
    output->append(version)
        .append(" ")
        .append(status)
        .append(" ")
        .append(reason)
        .append(sep)
        .append(headers)
        .append(sep)
        .append(body);
  }
};