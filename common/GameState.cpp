#include "GameState.h"

GameState::GameState() {
  for (int i = 0; i < NUM_PLAYERS; i++) {
    initialize_player(i);
  }
  initialize_board();
  initialize_weapon_list();
}

/* i ranges from 0 - 3 */
void GameState::initialize_player(int playerI) {
  Player p;
  p.id = playerI + 1;
  p.max_bomb = 1;
  p.bomb_left = 1;
  p.weapon = BOMB;
  p.life_left = 1;
  p.bomb_power = 1;
  p.speed = 1;
  /*p.gen;*/  // no idea
  //players[playerI] = p;
  players.insert(players.begin() + playerI, p);
}

void GameState::initialize_weapon_list() {
  weapon_list.insert(weapon_list.begin(), num_laser, LASER);
  weapon_list.insert(weapon_list.begin(), num_grenade, GRENADE);
  weapon_list.insert(weapon_list.begin(), num_rocket, ROCKET);
  weapon_list.insert(weapon_list.begin(), num_landmine, LANDMINE);
  weapon_list.insert(weapon_list.begin(), num_fire, FIRE);
  weapon_list.insert(weapon_list.begin(), num_frozen, FROZEN);
  weapon_list.insert(weapon_list.begin(), num_glove, GLOVE);
  weapon_list.insert(weapon_list.begin(), num_elixir, ELIXIR);
  weapon_list.insert(weapon_list.begin(), num_ball, BALL);
  weapon_list.insert(weapon_list.begin(), num_shield, SHIELD);
  weapon_list.insert(weapon_list.begin(), num_shoes, SHOES);
}

/* i ranges from 0 - 3 */
void GameState::initialize_board() {
    // read from file to initialize non-border non-destroyable cubes
    FILE * fp;
    if ((fp = fopen("./board.txt", "r")) == NULL) {
        std::cerr << "Error when loading the board." << std::endl;
        exit(1);
    }
    fscanf(fp, "%d\n", &board_x);
    fscanf(fp, "%d\n", &board_y);
    fscanf(fp, "%d %d", &(players[0].x), &(players[0].y));
    fscanf(fp, "%d %d", &(players[1].x), &(players[1].y));
    fscanf(fp, "%d %d", &(players[2].x), &(players[2].y));
    fscanf(fp, "%d %d", &(players[3].x), &(players[3].y));

    board = (char *) calloc(board_x * board_y, sizeof(char));
    //memset(board, NOTHING, sizeof(board));
    int row, col;
    while (fscanf(fp, "%d %d", &row, &col) > 0) {
        board[getIndex(row, col)] = NOT_DESTROYABLE_CUBE;
    }
    // set the boarder to be non-destroyable cubes
    for (int i = 0; i < board_x; i++) {
        board[board_y*i] = NOT_DESTROYABLE_CUBE;
        board[board_y*i+board_y-1] = NOT_DESTROYABLE_CUBE;
    }
    for (int i = 0; i < board_y; i++) {
        board[i] = NOT_DESTROYABLE_CUBE;
        board[board_y*(board_x-1)+i] = NOT_DESTROYABLE_CUBE;
    }

    fclose(fp);

    // generate destroyable blocks
    for (int i = 0; i < num_destroyables; i++) {
      int x = rand() % board_x;
      int y = rand() % board_y;
      if (board[getIndex(x, y)] != NOT_DESTROYABLE_CUBE) {
        board[getIndex(x, y)] = DONUT;
      }
    }
}

/* playerI ranges from 0 - 3 */
void GameState::updateWithAction(int playerI, char action) {
  switch (action) {
    case 'W':
      players[playerI].facing = '0';
      if (players[playerI].y < board_y - 1) {
        //char nextStepBoard = board[players[playerI].x][players[playerI].y + 1];
        char nextStepBoard = board[getIndex(players[playerI].x, players[playerI].y + 1)];
        if (nextStepBoard != NOT_DESTROYABLE_CUBE && nextStepBoard != DONUT &&
            nextStepBoard != BOMB) {
          players[playerI].y++;
        }
        check_bubble(playerI);
      }
      break;
    case 'A':
      players[playerI].facing = '2';
      if (players[playerI].x > 0) {
        //char nextStepBoard = board[players[playerI].x - 1][players[playerI].y];
        char nextStepBoard = board[getIndex(players[playerI].x - 1, players[playerI].y)];
        if (nextStepBoard != NOT_DESTROYABLE_CUBE && nextStepBoard != DONUT &&
            nextStepBoard != BOMB) {
          players[playerI].x--;
        }
        check_bubble(playerI);
      }
      break;
    case 'S':
      players[playerI].facing = '1';
      if (players[playerI].y > 0) {
        //char nextStepBoard = board[players[playerI].x][players[playerI].y - 1];
        char nextStepBoard = board[getIndex(players[playerI].x, players[playerI].y - 1)];
        if (nextStepBoard != NOT_DESTROYABLE_CUBE && nextStepBoard != DONUT &&
            nextStepBoard != BOMB) {
          players[playerI].y--;
        }
        check_bubble(playerI);
      }
      break;
    case 'D':
      players[playerI].facing = '3';
      if (players[playerI].x < board_x - 1) {
        //char nextStepBoard = board[players[playerI].x + 1][players[playerI].y];
        char nextStepBoard = board[getIndex(players[playerI].x + 1, players[playerI].y)];
        if (nextStepBoard != NOT_DESTROYABLE_CUBE && nextStepBoard != DONUT &&
            nextStepBoard != BOMB) {
          players[playerI].x++;
        }
        check_bubble(playerI);
      }
      break;
    case ' ':
      attack(playerI, players[playerI].x, players[playerI].y );
      break;

    default:
      break;
  }
}

char* GameState::toString() {
  char* buf = (char*)malloc(sizeof(GameState));
  memcpy(buf, this, sizeof(GameState));
  return buf;
}

/* might update players and board */
/* playerI ranges from 0 - 3 */
void GameState::check_bubble(int playerI) {
  // Player I picks up the weapon.
  //switch (board[players[playerI].x][players[playerI].y]) {
  switch (board[getIndex(players[playerI].x, players[playerI].y)]) {
    case LASER:
      players[playerI].weapon = LASER;
      break;
    case GRENADE:
      players[playerI].weapon = GRENADE;
      break;
    case ROCKET:
      players[playerI].weapon = ROCKET;
      break;
    case LANDMINE:
      players[playerI].weapon = LANDMINE;
      break;
    case FIRE:
      players[playerI].weapon = FIRE;
      break;
    case FROZEN:
      players[playerI].weapon = FROZEN;
      break;
    case GLOVE:  // TODO
      break;
    case ELIXIR:
      players[playerI].bomb_power += 1;
      break;
    case BALL:
      players[playerI].max_bomb += 1;
      players[playerI].bomb_left += 1;
      break;
    case SHIELD:
      players[playerI].life_left += 1;
      break;
    case SHOES:  // update speed
      // speed *= 1.1;
      // speed = max(speed, MAX_SPEED);
      break;  // TODO
    // MORE CASES. Including mined landmine, kills player if stepped on.
    default:
      break;
  }
  // Removes weapon from board.
  //board[players[playerI].x][players[playerI].y] = NOTHING;
  board[getIndex(players[playerI].x, players[playerI].y)] = NOTHING;
}

void GameState::attack(int playerI, int x, int y) {
  switch (players[playerI].weapon) {
    case BOMB:
      // check upward
      for (int i = 1; i <= players[playerI].bomb_power; i++) {
       if (check_bomb_effect(x, y - i)) break;
      }
      // check downward
      for (int i = 1; i <= players[playerI].bomb_power; i++) {
       if (check_bomb_effect(x, y + i)) break;
      }
      // check leftward
      for (int i = 1; i <= players[playerI].bomb_power; i++) {
       if (check_bomb_effect(x - i, y)) break;
      }
      // check rightward
      for (int i = 1; i <= players[playerI].bomb_power; i++) {
       if (check_bomb_effect(x + i, y)) break;
      }
      break;
    case LASER:
    case GRENADE:
    case ROCKET:
    case LANDMINE:
    case FIRE:
    case FROZEN:
    default:
      break;
  }
}

int GameState::check_bomb_effect(int x, int y) {
  int coord = getIndex(x, y);
  if (coord < 0) return 1;
  char obj = board[coord];
  if (obj == NOT_DESTROYABLE_CUBE) return 1;
  if (obj == DONUT) {
    int ind = rand() % weapon_list.size();
    board[coord] = weapon_list[ind]; // update corresponding object in board
    weapon_list.erase(weapon_list.begin() + ind); // remove the weapon from weapon list
    return 1;
  }
  // encounter player
  if (obj == PLAYER_1) {
    if (!players[0].life_left) {
      players.erase(players.begin());
      board[coord] = NOTHING;
    }
    else {
      players[0].life_left--;
    }
    return 1;
  }
  if (obj == PLAYER_2) {
    if (!players[1].life_left) {
      players.erase(players.begin());
      board[coord] = NOTHING;
    }
    else {
      players[1].life_left--;
    }
    return 1;
  }
  if (obj == PLAYER_3) {
    if (!players[2].life_left) {
      players.erase(players.begin());
      board[coord] = NOTHING;
    }
    else {
      players[2].life_left--;
    }
    return 1;
  }
  if (obj == PLAYER_4) {
    if (!players[3].life_left) {
      players.erase(players.begin());
      board[coord] = NOTHING;
    }
    else {
      players[3].life_left--;
    }
    return 1;
  }

  return 0;
}

int GameState::getIndex(int row, int col) {
  return GameState::board_y * row + col;
}