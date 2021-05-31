#include "main.h"

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

int setupLoader() {
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
  bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
  bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
  bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
  bool err = gladLoadGL(glfwGetProcAddress) ==
             0;  // glad2 recommend using the windowing library loader instead
                 // of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
  bool err = false;
  glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
  bool err = false;
  glbinding::initialize([](const char* name) {
    return (glbinding::ProcAddress)glfwGetProcAddress(name);
  });
#else
  bool err = false;  // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader
                     // is likely to requires some form of initialization.
#endif
  if (err) {
    fprintf(stderr, "Failed to initialize OpenGL loader!\n");
    return 1;
  }
  return 0;
}

void setupImGui(GLFWwindow* window) {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
  // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  const char* glsl_version = "#version 150";
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font.
}

void startImGuiFrame(GLFWwindow* window, bool show_demo, bool show_another,
                     ImVec4 clear_color) {
  // 1. Show the big demo window (Most of the sample code is in
  // ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear
  // ImGui!).
  if (show_demo) ImGui::ShowDemoWindow(&show_demo);

  // 2. Show a simple window that we create ourselves. We use a Begin/End pair
  // to created a named window.
  {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");  // Create a window called "Hello, world!"
                                    // and append into it.

    ImGui::Text("This is some useful text.");  // Display some text (you can use
                                               // a format strings too)
    ImGui::Checkbox(
        "Demo Window",
        &show_demo);  // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another);

    ImGui::SliderFloat("float", &f, 0.0f,
                       1.0f);  // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3(
        "clear color",
        (float*)&clear_color);  // Edit 3 floats representing a color

    if (ImGui::Button("Button"))  // Buttons return true when clicked (most
                                  // widgets return true when edited/activated)
      counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
  }

  // 3. Show another simple window.
  if (show_another) {
    ImGui::Begin("Another Window",
                 &show_another);  // Pass a pointer to our bool variable (the
                                  // window will have a closing button that will
                                  // clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me")) show_another = false;
    ImGui::End();
  }

  // Rendering
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
               clear_color.z * clear_color.w, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window);
}

void cleanupImGui() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
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
  if (setupLoader()) {
    return 1;
  }

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
  setupImGui(window);
  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  Networking::initClientNetworking(argv[1], argv[2]);
  std::cout << "sessionId: " << Networking::sessionId << std::endl;

  GameState s;
  std::vector<Obj4graphics> objects;
  Obj4graphics o;
  o.id = PLAYER_1;
  // TODO update generation matrix of cubes
  ModelMatrix mm = ModelMatrix();
  mm.move_to(0, 0, 0);
  o.gen = mm.get_model();
  objects.push_back(o);
  // Loop while GLFW window should stay open.
  while (!glfwWindowShouldClose(window)) {
    // Main render display callback. Rendering of objects is done here.

    // display ImGui
    startImGuiFrame(window, show_demo_window, show_another_window, clear_color);

    // TODO
    // third parameter represents playerI = sessionId + 4 (follow object.h)
    // Networking::sessionId is [0, 3].
    Window::displayCallback(window, objects, Networking::sessionId + PLAYER_1);

    // Idle callback. Updating objects, etc. can be done here.
    Window::idleCallback();

    // Networking::send("Message from client. ");
    char* msg = Networking::receive(sizeof(GameState));
    // GameState* s = (GameState*)malloc(sizeof(GameState));
    // std::cout << std::string(msg) << std::endl;
    // std::cout << "in while loop" << std::endl;
    if (msg != NULL) {
      // std::cout << "in if statement" << std::endl;
      memcpy(&s, msg, sizeof(GameState));
      // std::cout << s.toString() << std::endl;

      // std::cout << (int)s.board_x << " " << (int)s.board_y << std::endl;
      /*for (int i = 0; i < NUM_PLAYERS; i++) {
        moveSthBy(i, s.players[i].x, s.players[i].y, 0);
        std::cout << i << " " << (int)s.players[i].x << " "
                  << (int)s.players[i].y << std::endl;
       original main*/
      /*for (int i = 0; i < NUM_PLAYERS; i++) {
        moveSthBy(i, s.players[i].x - s.board.width / 2,
                  s.players[i].y - s.board.height / 2, 0);
        s.board[s.players[i].x][s.players[i].y] = 4 + i;
      }*/
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
          mm.move_to(2 * (i - s.board.width / 2), 2 * (j - s.board.height / 2),
                     0);
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
            case UPWARD:
              mmw.rotate(glm::radians(180.0f), 0, 0, 1);
              break;
            case LEFTWARD:
              mmw.rotate(glm::radians(270.0f), 0, 0, 1);
              break;
            case RIGHTWARD:
              mmw.rotate(glm::radians(90.0f), 0, 0, 1);
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

      free(msg);

      // Print GameState board.
      // std::system("CLS");
      std::ostringstream os;
      // for (int i = 0; i <= s.board.height; i++) {
      //   for (int j = 0; j <= s.board.width; j++) {
      //     switch (s.board[j][i]) {
      //       case NOTHING:
      //         os << "  ";
      //         break;
      //       case NOT_DESTROYABLE_CUBE:
      //         os << "\033[1;40m  \033[0m";
      //         break;
      //       case DONUT:
      //         os << "\033[1;42m  \033[0m";
      //         break;
      //       case PLAYER_1:
      //         if (s.players[0].life_left > 0) {
      //           os << "\033[1;37m11\033[0m";
      //         } else {
      //           os << "\033[1;37m  \033[0m";
      //         }
      //         break;
      //       case PLAYER_2:
      //         if (s.players[1].life_left > 0) {
      //           os << "\033[1;37m22\033[0m";
      //         } else {
      //           os << "\033[1;37m  \033[0m";
      //         }
      //         break;
      //       case PLAYER_3:
      //         if (s.players[2].life_left > 0) {
      //           os << "\033[1;37m33\033[0m";
      //         } else {
      //           os << "\033[1;37m  \033[0m";
      //         }
      //         break;
      //       case PLAYER_4:
      //         if (s.players[3].life_left > 0) {
      //           os << "\033[1;37m44\033[0m";
      //         } else {
      //           os << "\033[1;37m  \033[0m";
      //         }
      //         break;
      //       case LASER:
      //         os << "\033[1;31mLL\033[0m";
      //         break;
      //       case GRENADE:
      //         os << "\033[1;31mGG\033[0m";
      //         break;
      //       case ROCKET:
      //         os << "\033[1;31mRR\033[0m";
      //         break;
      //       case LANDMINE:
      //         os << "\033[1;31mLL\033[0m";
      //         break;
      //       case FIRE:
      //         os << "\033[1;31mFF\033[0m";
      //         break;
      //       case FROZEN:
      //         os << "\033[1;31mZZ\033[0m";
      //         break;
      //       case GLOVE:
      //         os << "\033[1;31mGG\033[0m";
      //         break;
      //       case ELIXIR:
      //         os << "\033[1;31mEE\033[0m";
      //         break;
      //       case BALL:
      //         os << "\033[1;31mBB\033[0m";
      //         break;
      //       case SHIELD:
      //         os << "\033[1;31mSS\033[0m";
      //         break;
      //       case SHOES:
      //         os << "\033[1;31mMM\033[0m";
      //         break;
      //       case BOMB:
      //         os << "\033[1;33mOO\033[0m";
      //         break;

      //       default:
      //         break;
      //     }
      //   }
      //   os << "\n";
      // }
      // std::cout << os.str() << std::endl;

      // Sleep for 1000 nanosecs.
      std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
    }
  }

  // Cleanup ImGui
  cleanupImGui();

  Window::cleanUp();
  // Destroy the window.
  glfwDestroyWindow(window);
  // Terminate GLFW.
  glfwTerminate();

  exit(EXIT_SUCCESS);
}
