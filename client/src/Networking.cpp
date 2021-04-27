#include "Networking.h"

using asio::ip::tcp;

asio::io_context Networking::io_context;
tcp::resolver Networking::resolver(io_context);
tcp::socket Networking::socket(io_context);
void Networking::initClientNetworking(std::string host, std::string port) {
  tcp::resolver::results_type endpoints = resolver.resolve(host, port);
  asio::connect(socket, endpoints);
  asio::ip::tcp::no_delay option(true);
  socket.set_option(option);
  socket.non_blocking(true);
}

// This will send synchronously.
void Networking::send(std::string msg) {
  size_t sentLen = socket.send(asio::buffer(msg));
}

// This will read synchronously.
std::string Networking::receive() {
  asio::error_code error;
  char buf[TCP_PAYLOAD_MAX_LEN];
  memset(buf, 0, TCP_PAYLOAD_MAX_LEN);
  // Sync read.
  size_t len = socket.read_some(asio::buffer(buf), error);

  if (error == asio::error::would_block) {
    // Closed by remote.
    return std::string("");
  } else if (error == asio::error::eof) {
    // Closed by remote.
    throw asio::system_error(error);
  } else if (error) {
    // Real error.
    throw asio::system_error(error);
  }
  return std::string(buf);
}