#include "ImGuiRunner.h"

#include <asio.hpp>
#include <iostream>
#include <thread>
#include <vector>

#include "Networking.h"

bool ImGuiRunner::isLoaderSetup;
bool ImGuiRunner::isWindowSetup;
bool ImGuiRunner::isImGuiFrameSetup;
GLFWwindow* ImGuiRunner::window;
// states
bool ImGuiRunner::show1 = true;
bool ImGuiRunner::show2 = false;
ImVec4 ImGuiRunner::color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void ImGuiRunner::setupLoader() {
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
    isLoaderSetup = false;
  }
  isLoaderSetup = true;
}

void ImGuiRunner::setupImGui(GLFWwindow* _window) {
  // check loader
  if (!isLoaderSetup) {
    fprintf(stderr, "OpenGL loader is not initialized!\n");
    isWindowSetup = false;
    return;
  }

  window = _window;
  isWindowSetup = true;

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
  const char* glsl_version = "#version 330";
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font.
}

void ImGuiRunner::updateStates(bool _show1, bool _show2, ImVec4 _color) {
  show1 = _show1;
  show2 = _show2;
  color = _color;
}

void ImGuiRunner::startImGuiFrame() {
  // Poll and handle events (inputs, window resize, etc.)
  // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell
  // if dear imgui wants to use your inputs.
  // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
  // your main application.
  // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data
  // to your main application. Generally you may always pass all inputs to dear
  // imgui, and hide them from your application based on those two flags.
  glfwPollEvents();

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  isImGuiFrameSetup = true;
}

void ImGuiRunner::showStarterPage() {
  static int counter = 0;

  ImGui::Begin("GROUP 1 GAME");  // Create a window called "Hello, world!"
                                 // and append into it.
  if (ImGui::Button("START"))    // Buttons return true when clicked (most
                                 // widgets return true when edited/activated)
  {
    counter++;
    cleanupImGui();
  }
  ImGui::SameLine();
  ImGui::Text("counter = %d", counter);
  ImGui::End();

  // // 3. Show another simple window.
  // if (show2) {
  //   ImGui::Begin("Another Window",
  //                &show2);  // Pass a pointer to our bool variable (the
  //                          // window will have a closing button that will
  //                          // clear the bool when clicked)
  //   ImGui::Text("Hello from another window!");
  //   if (ImGui::Button("Close Me")) show2 = false;
  //   ImGui::End();
  // }

  // Rendering
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(color.x * color.w, color.y * color.w, color.z * color.w,
               color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window);
}

void ImGuiRunner::showLobbyPage() {}

void ImGuiRunner::showEndPage() {}

void ImGuiRunner::cleanupImGui() {
  isLoaderSetup = false;
  isWindowSetup = false;
  isImGuiFrameSetup = false;
  window = nullptr;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}