#include "GameState.h"

GameState::GameState(int playerNum) {
  for (int i = 0; i < playerNum; i++) {
    playerCoord.push_back(std::pair<int, int>(0, 0));
    initialize_player(playerNum);
  }
}

/* playNum ranges from 1 - 4 */
void GameState::initialize_player(int playerNum){
    Player p;
    p.id = playerNum + 2;
    p.max_bomb = 1;
    p.bomb_left = 1;
    p.weapon = BOMB;
    p.life_left = 1;
    p.bomb_power = 1;
    p.speed = 1;
    /*p.gen;*/ // no idea
    players[playerNum - 1] = p;
}

void GameState::updateWithAction(int playerI, char action) {
  switch (action) {
    case 'W':
      if (playerCoord[playerI] < board_y) {
        playerCoord[playerI].second++;
        check_bubble(playerI);
      }
      break;
    case 'A':
      if (playerCoord[playerI] > 0) {
        playerCoord[playerI].first--;
        check_bubble(playerI);
      }
      break;
    case 'S':
      if (playerCoord[playerI] > 0) {
        playerCoord[playerI].second--;
        check_bubble(playerI);
      }
      break;
    case 'D':
      if (playerCoord[playerI] < board_x) {
        playerCoord[playerI].first++;
        check_bubble(playerI);
      }
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

/* might update players and board */
void GameState::check_bubble(int playerNum) {
  switch (playerCoord[playerI]) {
      case LASER:
        players[playNum-1].weapon = LASER;
        break;
      case GRENADE:
        players[playNum-1].weapon = GRENADE;
        break;
      case ROCKET:
        players[playNum-1].weapon = ROCKET;
        break;
      case LANDMINE:
        players[playNum-1].weapon = LANDMINE;
        break;
      case FIRE:
        players[playNum-1].weapon = FIRE;
        break;
      case FROZEN:
        players[playNum-1].weapon = FROZEN;
        break;
      case GLOVE: //TODO
        break;
      case ELIXIR:
        players[playNum-1].bomb_power += 1;
        break;
      case BALL:
        players[playNum-1].max_bomb += 1;
        players[playNum-1].bomb_left += 1;
        break;
      case SHIELD:
        players[playNum-1].life_left += 1;
        break;
      case SHOES: // update speed
        // speed *= 1.1;
        // speed = max(speed, MAX_SPEED);
        break; //TODO
      default:
  }
}
