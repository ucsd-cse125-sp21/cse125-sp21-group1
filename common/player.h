#ifndef __PLAYER_H__
#define __PLAYER_H__
#define UPWARD 2     // 0
#define DOWNWARD 3   // 1
#define LEFTWARD 1   // 2
#define RIGHTWARD 0  // 3

struct Player {
  char id;
  int x;
  int y;
  char max_bomb;     // how many bombs can the player place at most
  char weapon_left;  // how many weapons shots (including bomb) can the player
                     // still place
  char weapon;       // use index of object array
  int life_left;     // number of remaining lives, dead being negative.
  float speed;
  int bomb_power;   // how many grids are influenced (1D)
  float gen[4][4];  // 4 * 4 float matrix used for graphics
  char facing;      // 0 if upward, 1 if downward, 2 if leftward, 3 if rightward
};
typedef struct Player Player;
#endif
