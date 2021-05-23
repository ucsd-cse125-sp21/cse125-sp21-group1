#include "Window.h"

#include <glm/gtx/string_cast.hpp>

#include "model.h"

/*
 * Declare your variables below. Unnamed namespace is used here to avoid
 * declaring global or static variables.
 */
namespace {
int width, height;
std::string windowTitle("GLFW Starter Project");

Model* cake;
Model* choco_cake;
Model* coffee;
Model* gingerbreadHouse;
Model* bomb;
Model* glove;
Model* gun;
Model* medicine;
Model* motarshell;
Model* sheild;
Model* shoe;

// PointCloud* plsPoints;  // point light sphere

// collection of PointCloud, store all the game objects
std::vector<Model*> geometrys;
std::map<int, Model*> models;

glm::vec3 eye(0, 0, 30);    // Camera position.
glm::vec3 center(0, 0, 0);  // The point we are looking at.
glm::vec3 up(0, 1, 0);      // The up direction of the camera.
float fovy = 60;
float nearVal = 1;
float farVal = 1000;
glm::mat4 view = glm::lookAt(
    eye, center, up);  // View matrix, defined by eye, center and up.
glm::mat4 projection;  // Projection matrix.

Shader* shader;  // The shader.
// GLuint projectionLoc;  // Location of projection in shader.
// GLuint viewLoc;        // Location of view in shader.
// GLuint modelLoc;       // Location of model in shader.

// GLuint colorLoc;
// GLuint ambientLoc;
// GLuint diffuseLoc;
// GLuint specularLoc;
// GLuint shininessLoc;

// bool enablePhongColoring = 0;
// bool enableDirectionalLight = 0;

bool isMouseClicked[3] = {0, 0, 0};
double lastXpos, lastYpos;
};  // namespace

void moveSthBy(int i, int x, int y, int z) { geometrys[i]->moveTo(x, y, z); }

bool Window::initializeProgram() {
  // Create a shader program with a vertex shader and a fragment shader.
  shader =
      new Shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

  // Activate the shader program.
  // shader->use();
  // std::cout << "shaderID: in initialize " << shader->ID << std::endl;

  return true;
}

bool Window::initializeObjects() {
  // TODO: load all objects here

  // PlayerObj playerObjs[]
  // ItemObj itemObjs[]
  // etc.

  // cake = new Model("source/obstacle_cake/cake_without_plate.obj", 0.1);  //
  // black choco_cake =
  //     new Model("source/obstacle_cake2_texture/cake_obj/Chocolate Cake.obj",
  //     0.2);
  // // coffee = new Model("source/obstacle_coffee_texture/Cup _ Saucer.obj");
  // gingerbreadHouse =
  //     new Model("source/obstacle_GingerbreadHouse/GingerbreadHouse.obj");
  bomb = new Model("source/weapon_bomb_texture/Bomb.obj", 4);
  // glove = new Model("source/weapon_gloves/gloves.obj");
  // gun = new Model("source/weapon_gun/gun.obj");
  medicine = new Model("source/weapon_medicine/medicine.obj", 0.05);
  // motarshell = new Model("source/weapon_mortarshell/weapon_mortarshell.obj");
  sheild = new Model("source/weapon_shiled_texture/shiled.obj", 1);
  // shoe = new Model("source/weapon_shoe/shoe.obj");

  // TODO: add scale to model's constructor
  // cake->scale(0.1);
  // choco_cake->scale(0.2);
  // coffee->scale(1);
  // gingerbreadHouse->scale(6);
  // bomb->scale(4);
  // glove->scale(0.3);
  // gun->scale(0.7);
  // medicine->scale(0.05);
  // motarshell->scale(0.1);
  // sheild->scale(1);
  // shoe->scale(1);

  // geometrys.push_back(cake);
  // geometrys.push_back(choco_cake);
  // geometrys.push_back(coffee);
  // geometrys.push_back(gingerbreadHouse);
  geometrys.push_back(bomb);
  // geometrys.push_back(glove);
  // geometrys.push_back(gun);
  geometrys.push_back(medicine);
  // geometrys.push_back(motarshell);
  geometrys.push_back(sheild);
  // geometrys.push_back(shoe);

  // bind other values
  // glUniform3fv(glGetUniformLocation(program, "eyePos"), 1,
  // glm::value_ptr(eye));

  return true;
}

void Window::cleanUp() {
  // Deallcoate the objects.
  // TODO: add new model
  //   delete quad;

  // Delete the shader programs.
  glDeleteProgram(shader->ID);
  // glDeleteProgram(programQuad);
}

GLFWwindow* Window::createWindow(int width, int height) {
  // Initialize GLFW.
  // if (!glfwInit()) {
  //   std::cerr << "Failed to initialize GLFW" << std::endl;
  //   return NULL;
  // }

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

  // 4x antialiasing.
  // glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
  // Apple implements its own version of OpenGL and requires special treatments
  // to make it uses modern OpenGL.

  // Ensure that minimum OpenGL version is 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Enable forward compatibility and allow a modern OpenGL context
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // Create the GLFW window.
  GLFWwindow* window =
      glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

  // Check if the window could not be created.
  if (!window) {
    std::cerr << "Failed to open GLFW window." << std::endl;
    glfwTerminate();
    return NULL;
  }

  // Make the context of the window.
  glfwMakeContextCurrent(window);

  glewInit();
#ifndef __APPLE__
  // On Windows and Linux, we need GLEW to provide modern OpenGL
  // functionality.

  // Initialize GLEW.
  if (glewInit()) {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    return NULL;
  }
#endif

  // tell stb_image.h to flip loaded texture's on the y-axis (before loading
  // model).
  stbi_set_flip_vertically_on_load(true);
  // Set swap interval to 1.
  glfwSwapInterval(0);
  // glEnable(GL_DEPTH_TEST);

  // Call the resize callback to make sure things get drawn immediately.
  Window::resizeCallback(window, width, height);
  return window;
}

void Window::resizeCallback(GLFWwindow* window, int w, int h) {
  width = w;
  height = h;
#ifdef __APPLE__
  // In case your Mac has a retina display.
  glfwGetFramebufferSize(window, &width, &height);
#endif

  // Set the viewport size.
  glViewport(0, 0, width, height);

  // Set the projection matrix.
  projection = glm::perspective(glm::radians(fovy),
                                (float)width / (float)height, nearVal, farVal);
}

void Window::idleCallback() {
  // Perform any updates as necessary.

  //    std::cout << cos(directLight->degree / 10.0) <<'\n';
}

void Window::displayCallback(GLFWwindow* window,
                             std::vector<Obj4graphics> objects) {
  shader->use();
  // Switch back to using OpenGL's rasterizer
  // glUseProgram(program);
  // Clear the color and depth buffers.
  glClearColor(0.754f / 2, .821f / 2, 0.9375f / 2, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Specify the values of the uniform variables we are going to use.
  shader->setMat4("projection", projection);
  shader->setMat4("view", view);

  // TESTING: different locations
  glm::vec3 locations[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3(1.0f, 1.f, 0.0f),
      glm::vec3(-1.f, -1.0f, 0.0f),  glm::vec3(-3.0f, 1.1f, 0.0f),
      glm::vec3(2.0f, 3.f, 0.0f),    glm::vec3(1.f, -2.5f, 0.0f),
      glm::vec3(-3.0f, -1.3f, 0.0f), glm::vec3(-3.0f, 3.f, 0.0f),
      glm::vec3(-3.f, -3.0f, 0.0f),

      glm::vec3(6.0f, 0.0f, 0.0f)};

  // rendering
  /* for each input data of client
   * data contains object number and location
   */
  for (int i = 0; i < geometrys.size(); i++) {
    Model* currentObj = geometrys[i];
    // mat
    glm::mat4 curr_model = currentObj->getModel();

    glm::mat4 transform = glm::mat4(
        1.0f);  // make sure to initialize matrix to identity matrix first
    transform = glm::translate(transform, locations[i]);
    transform = glm::scale(transform, glm::vec3(currentObj->scale_factor));
    curr_model = curr_model * transform;

    shader->setMat4("model", curr_model);

    // std::cout << glm::to_string(curr_model) << std::endl;

    // Render the object.
    // std::cout << "shaderID: in display" << shader->ID << std::endl;
    currentObj->draw(*shader);
  }

  // Swap buffers.
  glfwSwapBuffers(window);
  // Gets events, including input such as keyboard and mouse or window resizing.
  glfwPollEvents();
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
  if (action == 0 || action == 2) {
    Networking::send(std::string(1, ((char)key)));
  }
}

void Window::scroll_callback(GLFWwindow* window, double xoffset,
                             double yoffset) {}

void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action,
                                   int mods) {
  if (button < 0 || button >= 3) return;
  if (action == GLFW_PRESS) isMouseClicked[button] = 1;
  if (action == GLFW_RELEASE) isMouseClicked[button] = 0;
}
