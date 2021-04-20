#include <asio.hpp>
#include <iostream>
#include <thread>
#include <vector>

#include "Networking.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: client <host> <port>" << std::endl;
    return 1;
  }

  Networking::initClientNetworking(argv[1], argv[2]);

  for (;;) {
    Networking::send("Message from client. ");
    std::string msg = Networking::receive();
    std::cout << msg << std::endl;

    // Sleep for 1 secs.
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}