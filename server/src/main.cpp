#include <chrono>
#include <iostream>
#include <thread>

#include "asio.hpp"

// #include "GameObject.h"
#include "GameState.h"
#include "Networking.h"
#include "player.h"
// #include "Parser.h"

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

  GameState state;

  while (true) {
    // Get all received data.
    std::vector<Message> data = Networking::recvData->getAll();

    // Message contains a sessionId and the actual message
    // received. One client can send multiple strings and will result in
    // multiple Messages. The vector is ordered by the received time.
    for (auto it = data.begin(); it != data.end(); it++) {
      std::cout << "Client " << it->sessionId << ": " << it->msg.c_str()[0]
                << std::endl;

      state.updateWithAction(it->sessionId, it->msg.c_str()[0]);
    }
    // Send message.
    char* writeBuf = state.toString();
    for (int i = 0; i < 4; i++) {
      if (Networking::allSessions[i] != NULL) {
        Networking::allSessions[i]->send(writeBuf, sizeof(GameState));
      }
    }
    free(writeBuf);

    // Sleep for 1 secs.
    std::this_thread::sleep_for(std::chrono::microseconds(100000));
  }
  free(GameState::board);
  return 0;
}

// void draw_main_board() {
//   Parser p;
//   // pseudo-code
//   std::vector<Message> data = Networking::recvData->getAll();
//   for (auto it = data.begin(); it != data.end(); it++) {
//     game_object_t obj = p.parseMessage(it->msg);
//     handle_object(obj);  // TO-IMPLEMENT
//   }
// }
