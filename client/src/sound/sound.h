#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>

#include <map>

#include "fmod.hpp"

// Names of some of the possible sound usege
#define SND_EXPLOAD 0
#define SND_BEZZ 1  // Bomb makes noise
#define SND_ITEM_PICKUP 2
#define SND_YOU_WIN 3
#define SND_YOU_LOSS 4
#define SND_BGM1 5
#define SND_BGM2 6

#define SND_NUM_CHANNELS 32

// load all sound; only need to call this once
int load_all_sound();
// play the sound with given sound_id
int playsound(int sound_id);

/*******************************************************/
/** implementation details; no need to read this part **/
static std::map<int, FMOD_SOUND*> sounds;
FMOD_SYSTEM* system;
FMOD_CHANNEL* channel = 0;
FMOD_RESULT result;
int init() {
  static int called = 0;
  if (called > 0) return 0;
  called = 1;

  unsigned int version;
  FMOD_System_Create(&system);
  FMOD_System_GetVersion(system, &version);

  if (version < FMOD_VERSION) {
    fprintf(stderr,
            "Error! You are using an old version of FMOD %08x. This program "
            "requires %08x\n",
            version, FMOD_VERSION);
    return -1;
  }

  FMOD_System_Init(system, SND_NUM_CHANNELS, FMOD_INIT_NORMAL, NULL);
}
// load sound at given path into specific sound_id
int load_sound(char* path, int sound_id) {
  FMOD_SOUND* sound;
  result = FMOD_System_CreateSound(system, path, 0, 0, &sound);
  sounds.insert({sound_id, sound});
  return 0;
}

int load_all_sound() {
  static int called = 0;
  if (called > 0) {
    printf("aleady loaded all sound.\n");
    return 0;
  }
  called = 1;

  load_sound("source/explosion.wav", SND_EXPLOAD);
  load_sound("source/explosion.wav", SND_BEZZ);
  load_sound("source/explosion.wav", SND_ITEM_PICKUP);
  load_sound("source/explosion.wav", SND_YOU_WIN);
  load_sound("source/explosion.wav", SND_YOU_LOSS);
  load_sound("source/explosion.wav", SND_BGM1);
  load_sound("source/explosion.wav", SND_BGM2);
  return 0;
}

int playsound(int sound_id) {
  result = FMOD_System_PlaySound(system, sounds[sound_id], NULL, 0, &channel);
  return 0;
}

#endif