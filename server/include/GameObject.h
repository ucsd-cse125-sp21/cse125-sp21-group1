#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <stdint.h>

#include <asio.hpp>
#include <iostream>
#include <thread>
#include <vector>

#include "Networking.h"

#define GRAPHICS 0

/* Structure for a Game Object */
struct game_object {
  unsigned char oid;  // object id
  uint16_t xcoord;    // x coordinate
  uint16_t ycoord;    // y coordinate
  uint16_t cmd;       // command
} __attribute__((packed));
typedef struct game_object game_object_t;

#endif