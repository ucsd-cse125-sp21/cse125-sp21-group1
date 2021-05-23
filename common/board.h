// int board_x = 20;
// int board_y = 20;
// char board[board_x][board_y];  // follow objects.h
#define BOARD_WIDTH_MAX 30
#define BOARD_HEIGHT_MAX 30

#include "objects.h"

// char * board;

class Board {
 public:
  char raw_board[BOARD_WIDTH_MAX * BOARD_HEIGHT_MAX];
  int width, height;

  Board() { memset(raw_board, NOTHING, BOARD_WIDTH_MAX * BOARD_HEIGHT_MAX); }
  char* operator[](const int xi) { return &raw_board[xi * BOARD_HEIGHT_MAX]; }
};