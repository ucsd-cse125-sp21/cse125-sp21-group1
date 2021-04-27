#include <chrono>
#include <iostream>
#include <thread>

#include "GameObject.h"
#include "Networking.h"
#include "Parser.h"

#define PORT 13

using asio::ip::tcp;

std::string make_daytime_string() {
  using namespace std;  // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

int main(int argc, char* argv[]) {
  // The initialization of the network.
  Networking::initServerNetworking(PORT);

  while (true) {
    // Get all received data.
    std::vector<Message> data = Networking::recvData->getAll();

    // Message contains a sessionId and the actual message
    // received. One client can send multiple strings and will result in
    // multiple Messages. The vector is ordered by the received time.
    for (auto it = data.begin(); it != data.end(); it++) {
      std::cout << "Client " << it->sessionId << ": " << it->msg << std::endl;

      // Send message.
      std::string writeBuf =
          "message from server for client " + std::to_string(it->sessionId);
      Networking::allSessions[it->sessionId]->send(writeBuf);
    }

    // Sleep for 1 secs.
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 0;
}

void draw_main_board() {
  Parser p;
  // pseudo-code
  std::vector<Message> data = Networking::recvData->getAll();
  for (auto it = data.begin(); it != data.end(); it++) {
    game_object_t obj = p.parseMessage(it->msg);
    handle_object(obj);  // TO-IMPLEMENT
  }
}