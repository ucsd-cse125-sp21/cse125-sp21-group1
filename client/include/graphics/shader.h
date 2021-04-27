#ifndef SHADER_HPP
#define SHADER_HPP

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <stdio.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

GLuint LoadShaders(const char* vertex_file_path,
                   const char* fragment_file_path);

#endif
