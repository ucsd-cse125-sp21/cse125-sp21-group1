#ifndef __GameState__
#define __GameState__

#include <iostream>
#include <thread>
#include <vector>

#include "Networking.h"
#include "board.h"
#include "objects.h"
class GameState {
 public:
  static std::vector<struct Player> players(4);

  std::vector<std::pair<int, int>> playerCoord;

  GameState(int playerNum);

  void updateWithAction(int playerI, char action);

  std::string toString();

  void initialize_player(int playerNum);
  void check_bubble(int playerNum);
};

#endif
