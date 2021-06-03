#ifndef _IMGUIRUNNER_H_
#define _IMGUIRUNNER_H_
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
#include "ModelMatrix.h"
#include "Networking.h"
#include "Window.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class ImGuiRunner {
 private:
  static bool isLoaderSetup;
  static bool isWindowSetup;
  static bool isImGuiFrameSetup;
  static GLFWwindow* window;
  // states
  static bool show1;
  static bool show2;
  static ImVec4 color;

 public:
  static int numPlayers;
  static void setupLoader();
  static void setupImGui(GLFWwindow* _window);
  static void updateStates(bool _show1, bool _show2, ImVec4 _color);
  static void startImGuiFrame();
  static void showStarterPage();
  static void showLobbyPage();
  static void showEndPage();
  static void cleanupImGui();
};

#endif