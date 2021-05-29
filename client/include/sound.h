#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>

// load all sound; only need to call this once
int load_all_sound();
// play the sound with given sound_id
int playsound(int sound_id);

/** implementation details; no need to read this part **/
// static
// load sound at given path into specific sound_id
int load_sound(char* path, int sound_id) { return 0; }
int load_all_sound() {
  static int call_once = 0;
  if (call_once > 0) {
    // std::cout << "aleady loaded all sound." << std::endl;
    return 0;
  }
  call_once = 1;

  return 0;
}
int playsound() { return 0; }

#endif