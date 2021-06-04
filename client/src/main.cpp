#include "main.h"

#include <GLUT/glut.h>
#include <time.h>

#include <asio.hpp>
#include <iostream>
#include <thread>
#include <vector>

#include "Networking.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && \
    !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

void errorCallback(int error, const char* description) {
  // Print error.
  std::cerr << description << std::endl;
}

void setupCallbacks(GLFWwindow* window) {
  // Set the error callback.
  glfwSetErrorCallback(errorCallback);
  // Set the key callback.
  glfwSetKeyCallback(window, Window::keyCallback);
  // Set the window resize callback.
  glfwSetWindowSizeCallback(window, Window::resizeCallback);

  glfwSetMouseButtonCallback(window, Window::mouse_button_callback);
  glfwSetScrollCallback(window, Window::scroll_callback);
  glfwSetCursorPosCallback(window, Window::cursor_pos_callback);
}

void setupOpenglSettings() {
  // Enable depth buffering.
  glEnable(GL_DEPTH_TEST);
  // Related to shaders and z value comparisons for the depth buffer.
  glDepthFunc(GL_LEQUAL);
  // Set polygon drawing mode to fill front and back of each polygon.
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // Set clear color to black.
  glClearColor(0.0, 0.0, 0.0, 0.0);
}

void printVersions() {
  // Get info of GPU and supported OpenGL version.
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "OpenGL version supported: " << glGetString(GL_VERSION)
            << std::endl;

  // If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
  std::cout << "Supported GLSL version is: "
            << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}

void starterDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1, 0, 0);
  glRasterPos3f(-0.8, 1.5, 0);
  char msg[] = "Press X to start the game";
  for (int i = 0; i < strlen(msg); i++) {
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, msg[i]);
  }
  glutSwapBuffers();
}

void normalDisplay() {}

void handleKeys(unsigned char key, int x, int y) {
  if (key == 'x') {
    glutDisplayFunc(normalDisplay);
  }
  glutPostRedisplay();
}

int main(int argc, char* argv[]) {
  /*
  std::cout << "start loading with Assimp" << std::endl;

  // Create an instance of the Importer class
  Assimp::Importer importer;

  // And have it read the given file with some example postprocessing
  // Usually - if speed is not the most important aspect for you - you'll
  // probably to request more postprocessing than we do in this example.
  const aiScene* scene = importer.ReadFile(
      "bunny.obj", aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                       aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

  // If the import failed, report it
  if (!scene) {
    std::cout << "loading failed. " << std::endl;
    return 1;
  }

  // Now we can access the file's contents.
  std::cout << "loading success. " << std::endl;
  std::cout << "num meshes: " << scene->mNumMeshes << std::endl;
  std::cout << "num materials: " << scene->mNumMaterials << std::endl;
  */
  if (argc != 3) {
    std::cerr << "Usage: client <host> <port>" << std::endl;
    return 1;
  }

  // Create the GLFW window.
  GLFWwindow* window = Window::createWindow(640, 640);
  if (!window) exit(EXIT_FAILURE);

  // Initialize OpenGL loader
  // ImGuiRunner::setupLoader();

  // Print OpenGL and GLSL versions.
  printVersions();
  // Setup callbacks.
  setupCallbacks(window);
  // Setup OpenGL settings.
  setupOpenglSettings();
  // Initialize the shader program; exit if initialization fails.
  if (!Window::initializeProgram()) exit(EXIT_FAILURE);
  // Initialize objects/pointers for rendering; exit if initialization fails.
  if (!Window::initializeObjects()) exit(EXIT_FAILURE);

  // Setup ImGui Context
  // ImGuiRunner::setupImGui(window);

  Networking::initClientNetworking(argv[1], argv[2]);
  std::cout << "sessionId: " << Networking::sessionId << std::endl;

  GameState s;
  auto recvFunc = [](GameState* gameStatePtr) {
    while (true) {
      char* msg = Networking::receive(sizeof(GameState));
      if (msg != NULL) {
        memcpy(gameStatePtr, msg, sizeof(GameState));
        free(msg);
      }
      std::this_thread::sleep_for(std::chrono::nanoseconds(500));
    }
  };
  std::thread networkingRecvThread = std::thread(recvFunc, &s);

  std::vector<Obj4graphics> objects;
  Obj4graphics o;
  o.id = PLAYER_1;
  // TODO update generation matrix of cubes
  ModelMatrix mm = ModelMatrix();
  mm.move_to(0, 0, 0);
  o.gen = mm.get_model();
  objects.push_back(o);

  // display ImGui
  // ImGuiRunner::startImGuiFrame();
  // while (ImGuiRunner::numPlayers < 4) {
  //   ImGuiRunner::showStarterPage();
  // }
  glutDisplayFunc(starterDisplay);
  glutKeyboardFunc(handleKeys);

  // Loop while GLFW window should stay open.
  while (!glfwWindowShouldClose(window)) {
    // auto startTime = clock();
    // Main render display callback. Rendering of objects is done here.

    // TODO
    // third parameter represents playerI = sessionId + 4 (follow object.h)
    // Networking::sessionId is [0, 3].
    Window::displayCallback(window, objects, Networking::sessionId + PLAYER_1);

    // Idle callback. Updating objects, etc. can be done here.
    Window::idleCallback();

    // Networking::send("Message from client. ");
    // msg = Networking::receive(sizeof(GameState));
    // GameState* s = (GameState*)malloc(sizeof(GameState));
    // std::cout << std::string(msg) << std::endl;
    // std::cout << "in while loop" << std::endl;
    if (true) {
      // std::cout << "in if statement" << std::endl;
      // std::cout << s.toString() << std::endl;
      /****************************************************/
      objects.clear();
      // add object struct for gameboard grids
      for (int i = 0; i < s.board.width; i++) {
        for (int j = 0; j < s.board.height; j++) {
          if (s.board[i][j] == NOTHING) {
            continue;
          }
          Obj4graphics o;
          o.id = s.board[i][j];
          // TODO update generation matrix of cubes
          ModelMatrix mm = ModelMatrix();
          if (o.id == SHIELD) {
            mm.move_to(2 * (i - s.board.width / 2 + 1),
                       2 * (j - s.board.height / 2), 0);
          } else {
            mm.move_to(2 * (i - s.board.width / 2),
                       2 * (j - s.board.height / 2), 0);
          }
          switch (o.id) {
            case SHIELD:
              mm.scale(0.5);
              break;

            case ELIXIR:
              mm.scale(0.03);
              break;

            case BALL:
              mm.scale(0.3);
              break;

            default:
              break;
          }
          o.gen = mm.get_model();
          objects.push_back(o);
        }
      }
      // add object struct for player and their weapons
      // std::cout << "num player: " << NUM_PLAYERS << std::endl;
      for (int i = 0; i < NUM_PLAYERS; i++) {
        // player
        if (s.players[i].life_left == 0) continue;
        Obj4graphics o;
        o.id = i + 4;
        ModelMatrix mm = ModelMatrix();
        switch (s.players[i].facing) {
          case UPWARD:
            mm.rotate(glm::radians(180.0f), 0, 0, 1);
            break;
          case LEFTWARD:
            mm.rotate(glm::radians(270.0f), 0, 0, 1);
            break;
          case RIGHTWARD:
            mm.rotate(glm::radians(90.0f), 0, 0, 1);
            break;
          default:
            break;
        }

        mm.move_to(2 * (s.players[i].x - s.board.width / 2),
                   2 * (s.players[i].y - s.board.height / 2), 0);

        o.gen = mm.get_model();
        objects.push_back(o);

        // weapon
        if (s.players[i].weapon != BOMB) {
          Obj4graphics ow;
          ow.id = s.players[i].weapon;
          ModelMatrix mmw = ModelMatrix();
          switch (s.players[i].facing) {
            case DOWNWARD:
              mmw.rotate(glm::radians(90.0f), 0, 0, 1);
              break;
            case LEFTWARD:
              mmw.rotate(glm::radians(180.0f), 0, 0, 1);
              break;
            case UPWARD:
              mmw.rotate(glm::radians(270.0f), 0, 0, 1);
              break;
            default:
              break;
          }
          mmw.move_to(s.players[i].x - s.board.width / 2,
                      s.players[i].y - s.board.height / 2, 5);

          ow.gen = mm.get_model();
          objects.push_back(ow);
        }
      }
      /****************************************************/
    }
    // std::cout << (double)(clock() - startTime) / CLOCKS_PER_SEC << std::endl;
  }

  // Cleanup ImGui
  // ImGuiRunner::cleanupImGui();

  Window::cleanUp();
  // Destroy the window.
  glfwDestroyWindow(window);
  // Terminate GLFW.
  glfwTerminate();

  exit(EXIT_SUCCESS);
}
