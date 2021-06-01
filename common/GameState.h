#ifndef __GameState__
#define __GameState__

#include <stdlib.h>

#include <iostream>
#include <thread>
#include <vector>

#include "Networking.h"
#include "board.h"
#include "bomb.h"
#include "metrics.h"
#include "objects.h"
#include "player.h"

// TODO (decide actual number)
// Moved outside GameState to minimize bytes needed for transmission, and they
// are also constants.
#define num_laser 0     // 20
#define num_grenade 0   // 20
#define num_rocket 0    // 20
#define num_landmine 0  // 100  // 20
#define num_fire 0
#define num_frozen 0
#define num_glove 20
#define num_elixir 0    // 30
#define num_ball 100    // 30
#define num_shield 100  // 20
#define num_shoes 0     // 20
#define num_destroyables 230

class GameState {
 public:
  // We need the board to be part of the GameState's memory, instead of a
  // pointer, in order for toString to work. A pointer will result in board not
  // being transmitted to the client.
  Board board;  // follow objects.h

  // Client rendering does not care about weapon_list, so vector is fine.
  std::vector<char> weapon_list;

  // We also need players on the stack. So unfortunately, this has to be an
  // array. Vector stores element on the heap, which the client will not have
  // players' information.
  // std::vector<struct Player> players;
  Player players[NUM_PLAYERS];

  // Bombs are already in the board for rendering, vector is fine.
  std::vector<Bomb> bombs_on_map;

  GameState();

  void updateWithAction(int, char);

  char* toString();

  void initialize_players();
  void initialize_board();
  void initialize_weapon_list();
  void check_bubble(int);
  void attack(int, int, int);

  // Update the board as this location is being bombed.
  int check_bomb_effect(int, int);

  // Check if bomb goes off.
  void tick_bomb();
};

#endif
