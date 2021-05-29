#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>

// load sound at given path into specific sound_id
int load_sound(char* path, int sound_id);
// load all sound
static int load_all_sound();
// play the sound with given sound_id
int playsound(int sound_id);

/* implementation details  */
// static
int load_sound(char* path, int sound_id) {}
int load_all_sound() {}
int playsound() {}

#endif