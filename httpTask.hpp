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

void httpTask(int sockfd) {
  // 暂且认为能收到一个完整的http报文
  char buf[4096];
  ssize_t n = recv(sockfd, buf, sizeof(buf) - 1, 0);
  if (n > 0) {
    buf[n] = 0;
    std::cout << buf << std::endl;
    Request req;
    req.Parse(buf);
    Response resp;
    conversion(req, resp);
    std::string resp_str;
    resp.Serialize(&resp_str);
    send(sockfd, resp_str.c_str(), resp_str.size(), 0);
  }
}