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
  int board_x;// = START_BOARD_X;
  int board_y;// = START_BOARD_Y;
  // char board[START_BOARD_X][START_BOARD_Y];  // follow objects.h
  char * board; // follow objects.h
  
  // TODO (decide actual number)
  int num_laser = 5;
  int num_grenade = 5;
  int num_rocket = 5;
  int num_landmine = 5;
  int num_fire = 0;
  int num_frozen = 0;
  int num_glove = 5;
  int num_elixir = 10;
  int num_ball = 10;
  int num_shield = 5;
  int num_shoes = 5;

  std::vector <char> weapon_list;

  Player players[NUM_PLAYERS];

  GameState();

  void updateWithAction(int, char);

  char* toString();

  void initialize_player(int);
  void initialize_board();
  void initialize_weapon_list();
  void check_bubble(int);

  int getIndex(int row, int col);
};

#endif
