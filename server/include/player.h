struct Player {
	int id;
	int max_bomb; // how many bombs can the player place at most
	int bomb_left; // how many bombs can the player still place
	int weapon; // use index of object array
	int life_left; // number of remaining lives
	float speed;
	int bomb_power; // how many grids are influenced (1D)
	float[4][4] gen; // 4 * 4 float matrix used for graphics
	int facing; // 0 if upward, 1 if downward, 2 if leftward, 3 if rightward
}
typedef struct Player Player;
