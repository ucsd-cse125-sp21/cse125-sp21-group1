#include "GameState.h"

GameState::GameState() {
  initialize_players();
  initialize_board();
  initialize_weapon_list();
}

/* i ranges from 0 - 3 */
void GameState::initialize_players() {
  for (int i = 0; i < NUM_PLAYERS; i++) {
    Player p;
    p.id = i;
    p.max_bomb = 1;
    p.weapon_left = p.max_bomb;
    p.weapon = BOMB;
    p.life_left = 1;
    p.bomb_power = 1;
    p.speed = 1;
    p.facing = DOWNWARD;
    /*p.gen;*/  // no idea
    players[i] = p;
  }
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
  FILE* fp;
  if ((fp = fopen("../common/board.txt", "r")) == NULL) {
    std::cerr << "Error when loading the board." << std::endl;
    exit(1);
  }
  fscanf(fp, "%d\n", &board.width);
  fscanf(fp, "%d\n", &board.height);
  fscanf(fp, "%d %d", &(players[0].x), &(players[0].y));
  fscanf(fp, "%d %d", &(players[1].x), &(players[1].y));
  fscanf(fp, "%d %d", &(players[2].x), &(players[2].y));
  fscanf(fp, "%d %d", &(players[3].x), &(players[3].y));

  // declaration for justification.
  // memset(board, NOTHING, sizeof(board));
  int row, col;
  while (fscanf(fp, "%d %d", &row, &col) > 0) {
    board[row][col] = NOT_DESTROYABLE_CUBE;
  }
  // set the boarder to be non-destroyable cubes
  // Unsure about border, board.txt suggests players are on the boarders.
  // for (int i = 0; i < board.width; i++) {
  //   board[i][0] = NOT_DESTROYABLE_CUBE;
  //   board[i][board.height - 1] = NOT_DESTROYABLE_CUBE;
  // }
  // for (int i = 0; i < board.height; i++) {
  //   board[0][i] = NOT_DESTROYABLE_CUBE;
  //   board[board.width - 1][i] = NOT_DESTROYABLE_CUBE;
  // }

  fclose(fp);

  // generate destroyable blocks
  for (int i = 0; i < num_destroyables;) {
    int x = rand() % board.width;
    int y = rand() % board.height;
    if (board[x][y] != NOT_DESTROYABLE_CUBE) {
      board[x][y] = DONUT;
      i++;
    }
  }

  // Ensure four corners are clear for player to place their first bomb.
  // Top left.
  board[0][0] = NOTHING;
  board[0][1] = NOTHING;
  board[1][0] = NOTHING;
  // Top right.
  board[board.width - 1][0] = NOTHING;
  board[board.width - 1][1] = NOTHING;
  board[board.width - 2][0] = NOTHING;
  // Bottom right.
  board[board.width - 1][board.height - 1] = NOTHING;
  board[board.width - 1][board.height - 2] = NOTHING;
  board[board.width - 2][board.height - 1] = NOTHING;
  // Bottom left.
  board[0][board.height - 1] = NOTHING;
  board[0][board.height - 2] = NOTHING;
  board[1][board.height - 1] = NOTHING;
}

/* playerI ranges from 0 - 3 */
void GameState::updateWithAction(int playerI, char action) {
  std::cout << "player: " << playerI << "action: " << action << std ::endl;
  if (players[playerI].life_left < 0) {
    return;
  }
  switch (action) {
    case 'W':
      players[playerI].facing = UPWARD;
      if (players[playerI].y < board.height) {
        char nextStepBoard = board[players[playerI].x][players[playerI].y + 1];
        if (nextStepBoard != NOT_DESTROYABLE_CUBE && nextStepBoard != DONUT &&
            nextStepBoard != BOMB) {
          players[playerI].y++;
        }
        check_bubble(playerI);
        if (board[players[playerI].x][players[playerI].y] == LANDMINE) {
          check_bomb_effect(players[playerI].x, players[playerI].y);
        }
      }
      break;
    case 'A':
      // printf("1\n");
      players[playerI].facing = LEFTWARD;
      // printf("2\n");
      if (players[playerI].x > 0) {
        // printf("inside if\n");
        char nextStepBoard = board[players[playerI].x - 1][players[playerI].y];
        // printf("ini 2\n");
        if (nextStepBoard != NOT_DESTROYABLE_CUBE && nextStepBoard != DONUT &&
            nextStepBoard != BOMB) {
          // printf("nested if\n");
          players[playerI].x--;
          // printf("x--\n");
        }
        // printf("before check bubble\n");
        check_bubble(playerI);
        // printf("after\n");
        if (board[players[playerI].x][players[playerI].y] == LANDMINE) {
          // printf("another\n");
          check_bomb_effect(players[playerI].x, players[playerI].y);
        }
      }
      break;
    case 'S':
      players[playerI].facing = DOWNWARD;
      if (players[playerI].y > 0) {
        char nextStepBoard = board[players[playerI].x][players[playerI].y - 1];
        if (nextStepBoard != NOT_DESTROYABLE_CUBE && nextStepBoard != DONUT &&
            nextStepBoard != BOMB) {
          players[playerI].y--;
        }
        check_bubble(playerI);
        if (board[players[playerI].x][players[playerI].y] == LANDMINE) {
          check_bomb_effect(players[playerI].x, players[playerI].y);
        }
      }
      break;
    case 'D':
      players[playerI].facing = RIGHTWARD;
      if (players[playerI].x < board.width - 1) {
        char nextStepBoard = board[players[playerI].x + 1][players[playerI].y];
        if (nextStepBoard != NOT_DESTROYABLE_CUBE && nextStepBoard != DONUT &&
            nextStepBoard != BOMB) {
          players[playerI].x++;
        }
        check_bubble(playerI);
        if (board[players[playerI].x][players[playerI].y] == LANDMINE) {
          check_bomb_effect(players[playerI].x, players[playerI].y);
        }
      }
      break;
    case ' ':
      attack(playerI, players[playerI].x, players[playerI].y);
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
  // switch (board[players[playerI].x][players[playerI].y]) {
  switch (board[players[playerI].x][players[playerI].y]) {
    case LASER:
      players[playerI].weapon = LASER;
      players[playerI].weapon_left =
          1;  // TODO: Discuss number for weapon_left for special weapons.
      break;
    case GRENADE:
      players[playerI].weapon = GRENADE;
      players[playerI].weapon_left = 2;
      break;
    case ROCKET:
      players[playerI].weapon = ROCKET;
      players[playerI].weapon_left = 2;
      break;
    case LANDMINE:
      players[playerI].weapon = LANDMINE;
      players[playerI].weapon_left = 1;
      break;
    case FIRE:
      players[playerI].weapon = FIRE;
      players[playerI].weapon_left = 2;
      break;
    case FROZEN:
      players[playerI].weapon = FROZEN;
      players[playerI].weapon_left = 2;
      break;
    case GLOVE:  // TODO
      break;
    case ELIXIR:
      players[playerI].bomb_power += 1;
      break;
    case BALL:
      players[playerI].max_bomb += 1;
      players[playerI].weapon_left += 1;
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
  board[players[playerI].x][players[playerI].y] = NOTHING;
}

void GameState::attack(int playerI, int x, int y) {
  if (players[playerI].weapon == BOMB) {
    if (players[playerI].weapon_left > 0) {
      players[playerI].weapon_left--;
      board[x][y] = BOMB;
      bombs_on_map.push_back(Bomb(x, y, playerI));
    }
  } else {
    if (players[playerI].weapon_left > 0) {
      switch (players[playerI].weapon) {
        case LASER:
          break;
        case GRENADE:
          break;
        case ROCKET:
          break;
        case LANDMINE:
          board[x][y] = LANDMINE;
          players[playerI].weapon = BOMB;
          players[playerI].weapon_left = players[playerI].max_bomb;
          break;
        case FIRE:
          break;
        case FROZEN:
          break;
        default:
          break;
      }
      players[playerI].weapon_left--;
    }
    if (players[playerI].weapon_left <= 0) {
      players[playerI].weapon = BOMB;
      players[playerI].weapon_left = players[playerI].max_bomb;
    }
  }
}

int GameState::check_bomb_effect(int x, int y) {
  char obj = board[x][y];
  if (obj == NOT_DESTROYABLE_CUBE) return 1;
  if (obj == LANDMINE) {
    board[x][y] = NOTHING;
    return 1;
  }
  if (obj == DONUT) {
    int ind = rand() % weapon_list.size();
    board[x][y] = weapon_list[ind];  // update corresponding object in board
    weapon_list.erase(weapon_list.begin() +
                      ind);  // remove the weapon from weapon list
    return 1;
  }
  // encounter player
  for (int i = 0; i < NUM_PLAYERS; i++) {
    if (x == players[i].x && y == players[i].y) {
      players[i].life_left--;
      if (players[i].life_left <= 0) {
        // TODO - send message to client side notifying the death
        //      - erase player from player list
        players[i].life_left = 0;
        board[x][y] = NOTHING;
      }
      return 1;
    }
  }
  // Players are not drawn directly on the board. They could conflict with a
  // bomb. For example, placing a bomb on one's face.

  // Check if landmine. If it is, remove it.
  if (obj == LANDMINE) {
    board[x][y] = NOTHING;
  }

  return 0;
}

void GameState::tick_bomb() {
  for (auto b = bombs_on_map.begin(); b != bombs_on_map.end();) {
    if (b->is_bombed()) {
      // check upward, also consider putting a bomb on others' face.
      for (int i = 0; i <= players[b->bomberI].bomb_power; i++) {
        if (check_bomb_effect(b->x, b->y - i)) break;
      }
      // check downward
      for (int i = 1; i <= players[b->bomberI].bomb_power; i++) {
        if (check_bomb_effect(b->x, b->y + i)) break;
      }
      // check leftward
      for (int i = 1; i <= players[b->bomberI].bomb_power; i++) {
        if (check_bomb_effect(b->x - i, b->y)) break;
      }
      // check rightward
      for (int i = 1; i <= players[b->bomberI].bomb_power; i++) {
        if (check_bomb_effect(b->x + i, b->y)) break;
      }
      players[b->bomberI].weapon_left++;
      board[b->x][b->y] = NOTHING;
      bombs_on_map.erase(b);
    } else {
      b++;
    }
  }
}
