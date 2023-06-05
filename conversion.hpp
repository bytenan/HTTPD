#include "protocol.hpp"

void conversion(const Request &req, Response &resp) {
  resp.version = "HTTP/1.1";
  resp.status = "200";
  resp.reason = "OK";
  resp.headers = "Content-Type: " + getMIME(req.suffix) + sep;
  resp.headers += "Content-Length: " + std::to_string(req.size) + sep;

  std::string buf;
  buf.resize(req.size);
  if (!readFile(req.path, (char *)buf.c_str(), req.size)) {
    readFile(page_404, (char *)buf.c_str(), req.size);
  }
  resp.body = buf;
}