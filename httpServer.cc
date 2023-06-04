#include "httpServer.hpp"

#include <iostream>
#include <map>
#include <string>

const int16_t port = 8080;

void usage(std::string proc) {
  std::cout << "usage\n\t" << proc << " port" << std::endl;
}

int main(int argc, char *args[]) {
  if (argc != 2) {
    usage(args[0]);
    exit(-1);
  }
  int port = atoi(args[1]);
  httpServer *svr = new httpServer(port);
  svr->start();

  return 0;
}