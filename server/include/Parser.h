#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdlib.h>

#include <asio.hpp>
#include <iostream>
#include <thread>
#include <vector>

#include "GameObject.h"
#include "Networking.h"

class Parser {
 public:
  game_object_t parseMessage(std::string msg);
};

#endif