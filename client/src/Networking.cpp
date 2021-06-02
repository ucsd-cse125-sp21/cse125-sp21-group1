#include "Networking.h"

using asio::ip::tcp;

asio::io_context Networking::io_context;
tcp::resolver Networking::resolver(io_context);
tcp::socket Networking::socket(io_context);
int Networking::sessionId;

void Networking::initClientNetworking(std::string host, std::string port) {
  tcp::resolver::results_type endpoints = resolver.resolve(host, port);
  asio::connect(socket, endpoints);
  asio::ip::tcp::no_delay option(true);
  socket.set_option(option);
  socket.non_blocking(true);

  char* sessionIdStr = Networking::receive(1);
  while (sessionIdStr == NULL) {
    std::cout << "Session Id not received, retry" << std::endl;
    sessionIdStr = Networking::receive(1);
  }
  Networking::sessionId = sessionIdStr[0] - '0';
  free(sessionIdStr);
}

// This will send synchronously.
void Networking::send(std::string msg) {
  size_t sentLen = socket.send(asio::buffer(msg));
}

// This will read synchronously.
char* Networking::receive(size_t desiredLen) {
  asio::error_code error;
  char buf[desiredLen];
  memset(buf, 0, desiredLen);
  // Sync read.
  size_t len = socket.read_some(asio::buffer(buf, desiredLen), error);
  // std::cout << len << std::endl;

  if (error == asio::error::would_block) {
    // Nothing received, can try again.
    return NULL;
  } else if (error == asio::error::eof) {
    // Closed by remote.
    throw asio::system_error(error);
  } else if (error) {
    // Real error.
    throw asio::system_error(error);
  }
  char* returnbuf = (char*)malloc(desiredLen);
  memcpy(returnbuf, buf, desiredLen);
  return returnbuf;
}