#include "GameState.h"

GameState::GameState() {
  for (int i = 0; i < NUM_PLAYERS; i++) {
    initialize_player(i);
  }
  initialize_board();
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
  players[playerI] = p;
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

    board = calloc(sizeof(int) * board_x * board_y);
    //memset(board, NOTHING, sizeof(board));
    int row, col;
    while (fscanf(fp, "%d %d", &row, &col) > 0) {
        board[board_y*row+col] = NOT_DESTROYABLE_CUBE;
    }
    // set the boarder to be non-destroyable cubes
    for (int i = 0; i < board_x; i++) {
        board[board_y*i] = NOT_DESTROYABLE_CUBE;
        board[board_y*i+board_y-1] = NOT_DESTROYABLE_CUBE;
    }
    for (i = 0; i < board_y; i++) {
        board[i] = NOT_DESTROYABLE_CUBE;
        board[board_y*(board_x-1)+i] = NOT_DESTROYABLE_CUBE;
    }
}

/* playerI ranges from 0 - 3 */
void GameState::updateWithAction(int playerI, char action) {
  switch (action) {
    case 'W':
      if (players[playerI].y < board_y - 1) {
        char nextStepBoard = board[players[playerI].x][players[playerI].y + 1];
        if (nextStepBoard != NOT_DESTROYABLE_CUBE && nextStepBoard != DONUT &&
            nextStepBoard != BOMB) {
          players[playerI].y++;
        }
        check_bubble(playerI);
      }
      break;
    case 'A':
      if (players[playerI].x > 0) {
        char nextStepBoard = board[players[playerI].x - 1][players[playerI].y];
        if (nextStepBoard != NOT_DESTROYABLE_CUBE && nextStepBoard != DONUT &&
            nextStepBoard != BOMB) {
          players[playerI].x--;
        }
        check_bubble(playerI);
      }
      break;
    case 'S':
      if (players[playerI].y > 0) {
        char nextStepBoard = board[players[playerI].x][players[playerI].y - 1];
        if (nextStepBoard != NOT_DESTROYABLE_CUBE && nextStepBoard != DONUT &&
            nextStepBoard != BOMB) {
          players[playerI].y--;
        }
        check_bubble(playerI);
      }
      break;
    case 'D':
      if (players[playerI].x < board_x - 1) {
        char nextStepBoard = board[players[playerI].x + 1][players[playerI].y];
        if (nextStepBoard != NOT_DESTROYABLE_CUBE && nextStepBoard != DONUT &&
            nextStepBoard != BOMB) {
          players[playerI].x++;
        }
        check_bubble(playerI);
      }
      break;
    case ' ':
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
  switch (board[players[playerI].x][players[playerI].y]) {
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
  board[players[playerI].x][players[playerI].y] = NOTHING;
}
