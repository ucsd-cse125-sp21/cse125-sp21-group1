#ifndef __PLAYER_H__
#define __PLAYER_H__
struct Player {
  char id;
  char x;
  char y;
  char max_bomb;   // how many bombs can the player place at most
  char bomb_left;  // how many bombs can the player still place
  char weapon;     // use index of object array
  char life_left;  // number of remaining lives
  float speed;
  char bomb_power;  // how many grids are influenced (1D)
  float gen[4][4];  // 4 * 4 float matrix used for graphics
  char facing;      // 0 if upward, 1 if downward, 2 if leftward, 3 if rightward
};
typedef struct Player Player;
#endif