#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Light.hpp"
#include "Networking.h"
#include "Object.h"
#include "PointCloud.h"
#include "shader.h"

void moveSthBy(int i, int x, int y, int z);

class Window {
 public:
  // Initializes your shader program(s) and uniform variable locations
  static bool initializeProgram();
  // Initialize your OBJ objects here
  static bool initializeObjects();
  // Make sure to clean up any data you dynamically allocated here
  static void cleanUp();
  // Creates a GLFW window context
  static GLFWwindow* createWindow(int width, int height);
  // Is called whenever the window is resized
  static void resizeCallback(GLFWwindow* window, int width, int height);
  // Is called on idle.
  static void idleCallback();
  // This renders to the glfw window. Add draw calls here
  static void displayCallback(GLFWwindow*);
  // Add your key press event handling here
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action,
                          int mods);

  static void mouse_button_callback(GLFWwindow* window, int button, int action,
                                    int mods);
  static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
  static void scroll_callback(GLFWwindow* window, double xoffset,
                              double yoffset);
};

#endif
