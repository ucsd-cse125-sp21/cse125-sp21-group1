#ifndef __NETWORKING__
#define __NETWORKING__

#include <asio.hpp>
#include <iostream>
#include <vector>

#define TCP_PAYLOAD_MAX_LEN 1460  // According to Google.

using asio::ip::tcp;

class Networking {
 public:
  static asio::io_context io_context;
  static tcp::resolver resolver;
  static tcp::socket socket;

  static void initClientNetworking(std::string, std::string);

  // This will send synchronously.
  static void send(std::string msg);

  // This will read synchronously.
  static std::string receive();
};

#endif