#ifndef __BOMB_H__
#define __BOMB_H__

#include <chrono>

#define BOMB_MS 2000  // The time for bomb to go off in milliseconds.

class Bomb {
 public:
  int x;
  int y;
  std::chrono::high_resolution_clock::time_point
      goes_off_time;  // Remaining ticks in ms.
  int bomberI;

  Bomb(int x, int y, int bomberI) : x(x), y(y), bomberI(bomberI) {
    goes_off_time = std::chrono::high_resolution_clock::now() +
                    std::chrono::duration<int, std::milli>(BOMB_MS);
  }

  // Check if bomb should go off.
  bool is_bombed() {
    return goes_off_time < std::chrono::high_resolution_clock::now();
  }
};

#endif