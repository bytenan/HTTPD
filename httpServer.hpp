#pragma once
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "conversion.hpp"
#include "protocol.hpp"

const int backlog = 5;

class httpServer {
 public:
  httpServer(int32_t port) : server_port(port) {
    listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sockfd == -1) {
      std::cout << "creat socket error" << std::endl;
      exit(-1);
    }
    struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(server_port);
    if (bind(listen_sockfd, (struct sockaddr *)&local_addr,
             sizeof local_addr)) {
      std::cout << "bind error" << std::endl;
      exit(-1);
    }
    if (listen(listen_sockfd, backlog)) {
      std::cout << "listen error" << std::endl;
      exit(-1);
    }
  }
  void start() {
    while (true) {
      struct sockaddr_in remote_addr;
      socklen_t remote_addr_len = sizeof remote_addr;
      int sockfd = accept(listen_sockfd, (struct sockaddr *)&remote_addr,
                          &remote_addr_len);
      if (sockfd == -1) {
        std::cout << "accept error, contine try accept";
        continue;
      }
      pthread_t tid;
      if (pthread_create(&tid, nullptr, start_routine, &sockfd)) {
        std::cout << "pthread_create error" << std::endl;
      }
    }
  }

 private:
  static void *start_routine(void *args) {
    pthread_detach(pthread_self());
    int sockfd = *(static_cast<int *>(args));
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
    close(sockfd);
    pthread_exit(nullptr);
  }

 private:
  int listen_sockfd;
  int32_t server_port;
};