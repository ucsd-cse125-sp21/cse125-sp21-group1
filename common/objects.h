#define NOTHING 0
#define NOT_DESTROYABLE_CUBE 1
#define DONUT 2
// TODO second type of destroyable cube
#define PLAYER_1 4
#define PLAYER_2 5
#define PLAYER_3 6
#define PLAYER_4 7
#define LASER 8
#define GRENADE 9
#define ROCKET 10
#define LANDMINE 11
#define FIRE 12
#define FROZEN 13
#define GLOVE 14
#define ELIXIR 15
#define BALL 16
#define SHIELD 17
#define SHOES 18
#define BOMB 19

struct Obj4graphics {
  char id;
  float gen[4][4];  // 4 * 4 float matrix used for graphics
};
typedef struct Object Object;
