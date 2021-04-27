#include "GameState.h"

GameState::GameState(int playerNum) {
  for (int i = 0; i < playerNum; i++) {
    playerCoord.push_back(std::pair<int, int>(0, 0));
  }
}
void GameState::updateWithAction(int playerI, char action) {
  switch (action) {
    case 'W':
      playerCoord[playerI].second++;
      break;
    case 'A':
      playerCoord[playerI].first--;
      break;
    case 'S':
      playerCoord[playerI].second--;
      break;
    case 'D':
      playerCoord[playerI].first++;
      break;
    case ' ':
      break;

    default:
      break;
  }
}

std::string GameState::toString() {
  std::string s;
  auto i = playerCoord.begin();
  s += std::to_string(i->first) + " " + std::to_string(i->second);
  i++;
  for (; i < playerCoord.end(); i++) {
    s += " " + std::to_string(i->first) + " " + std::to_string(i->second);
  }
  return s;
}
