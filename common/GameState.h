#ifndef __GameState__
#define __GameState__

#include <iostream>
#include <thread>
#include <vector>

#include "Networking.h"
#include "board.h"
#include "metrics.h"
#include "objects.h"
#include "player.h"
class GameState {
 public:
  char board_x = START_BOARD_X;
  char board_y = START_BOARD_Y;
  char board[START_BOARD_X][START_BOARD_Y];  // follow objects.h

  Player players[NUM_PLAYERS];

  GameState();

  void updateWithAction(int, char);

  char* toString();

  void initialize_player(int);
  void initialize_board();
  void check_bubble(int);
};

#endif
