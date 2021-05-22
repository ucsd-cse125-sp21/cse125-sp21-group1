#ifndef __GameState__
#define __GameState__

#include <iostream>
#include <thread>
#include <vector>

#include "Networking.h"
class GameState {
 public:
  std::vector<std::pair<int, int>> playerCoord;

  GameState(int playerNum);
  
  void updateWithAction(int playerI, char action);

  std::string toString();
};

#endif