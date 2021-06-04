
// compile this at client/:
// gcc src/sound/sound_test.h  -Ilib/fmod/inc -Llib/fmod/lib -lfmod -lfmodL -out

#include "sound.h"

#include <stdio.h>

#ifndef __APPLE__
#include <windows.h>
#endif

int main() {
  load_all_sound();
  playsound(SND_EXPLOAD);

  sleep(10000);

  playsound(SND_EXPLOAD);
  sleep(100);
  playsound(SND_EXPLOAD);
  sleep(10000);

  playsound(SND_EXPLOAD);
  sleep(100);
  playsound(SND_ITEM_PICKUP);
  sleep(10000);

  return 0;
}

// int main() {
//   FMOD_SYSTEM* system;
//   FMOD_SOUND* sound1;
//   FMOD_CHANNEL* channel = 0;
//   FMOD_RESULT result;
//   unsigned int version;

//   /*
//    Create a System object and initialize.
//   */
//   FMOD_System_Create(&system);
//   FMOD_System_GetVersion(system, &version);

//   if (version < FMOD_VERSION) {
//     printf(
//         "Error! You are using an old version of FMOD %08x. This program "
//         "requires %08x\n",
//         version, FMOD_VERSION);
//     return -1;
//   }
//   FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, NULL);

//   result =
//       FMOD_System_CreateSound(system, "source/explosion.wav", 0, 0, &sound1);

//   result = FMOD_System_PlaySound(system, sound1, NULL, 0, &channel);

//   int isplaying = true;
//   while (isplaying) {
//     FMOD_Channel_IsPlaying(channel, &isplaying);
//     FMOD_System_Update(system);
//   }

//   result = FMOD_Sound_Release(sound1);
//   result = FMOD_System_Close(system);
//   result = FMOD_System_Release(system);
// }
