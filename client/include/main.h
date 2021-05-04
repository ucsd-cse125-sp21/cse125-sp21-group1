#ifndef _MAIN_H_
#define _MAIN_H_
#define GL_SILENCE_DEPRECATION

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "GameState.h"
#include "Window.h"

#endif
