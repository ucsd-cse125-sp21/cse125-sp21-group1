#include "Window.h"

/*
 * Declare your variables below. Unnamed namespace is used here to avoid
 * declaring global or static variables.
 */
namespace {
int width, height;
std::string windowTitle("GLFW Starter Project");
PointCloud* cake;
PointCloud* gingerbreadHouse;
PointCloud* bomb;
PointCloud* gun;
PointCloud* mortarshell;
PointCloud* shoe;
PointCloud* coffee;
PointCloud* plsPoints;  // point light sphere

// collection of PointCloud, store all the game objects
std::vector<PointCloud*> geometrys;

Light* directLight;

glm::vec3 eye(0, 0, 20);    // Camera position. up to down: (0,20,0)
glm::vec3 center(0, 0, 0);  // The point we are looking at.
glm::vec3 up(0, 1, 0);  // The up direction of the camera. up to down: (1,0,0)
float fovy = 60;
float nearVal = 1;
float farVal = 1000;
glm::mat4 view = glm::lookAt(
    eye, center, up);  // View matrix, defined by eye, center and up.
glm::mat4 projection;  // Projection matrix.

GLuint program;        // The shader program id.
GLuint projectionLoc;  // Location of projection in shader.
GLuint viewLoc;        // Location of view in shader.
GLuint modelLoc;       // Location of model in shader.

GLuint colorLoc;
GLuint ambientLoc;
GLuint diffuseLoc;
GLuint specularLoc;
GLuint shininessLoc;

bool enablePhongColoring = 0;
bool enableDirectionalLight = 0;

bool isMouseClicked[3] = {0, 0, 0};
double lastXpos, lastYpos;
};  // namespace

void moveSthBy(int i, int x, int y, int z) { geometrys[i]->moveTo(x, y, z); }

bool Window::initializeProgram() {
  // Create a shader program with a vertex shader and a fragment shader.
  program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

  // Check the shader programs.
  if (!program) {
    std::cerr << "Failed to initialize shader program" << std::endl;
    return false;
  }

  // Activate the shader program.
  glUseProgram(program);
  // Get the locations of uniform variables.
  projectionLoc = glGetUniformLocation(program, "projection");
  viewLoc = glGetUniformLocation(program, "view");
  modelLoc = glGetUniformLocation(program, "model");

  colorLoc = glGetUniformLocation(program, "mat.color");
  ambientLoc = glGetUniformLocation(program, "mat.ambient");
  diffuseLoc = glGetUniformLocation(program, "mat.diffuse");
  specularLoc = glGetUniformLocation(program, "mat.specular");
  shininessLoc = glGetUniformLocation(program, "mat.shininess");

  return true;
}

bool Window::initializeObjects() {
  // TODO: load all objects here

  // PlayerObj playerObjs[]
  // ItemObj itemObjs[]
  // etc.

  cake = new PointCloud(
      "source/obstacle_cake2_texture/cake_obj/Chocolate Cake.obj", 1);
  gingerbreadHouse = new PointCloud(
      "source/obstacle_GingerbreadHouse/GingerbreadHouse.obj", 1);
  bomb = new PointCloud("source/weapon_bomb_texture/Bomb.obj", 1);
  gun = new PointCloud("source/weapon_gun/gun.obj", 1);
  mortarshell =
      new PointCloud("source/weapon_mortarshell/weapon_mortarshell.obj", 1);
  shoe = new PointCloud("source/weapon_shoe/shoe.obj", 1);
  coffee = new PointCloud("source/obstacle_coffee_texture/Cup _ Saucer.obj", 1);

  // TESTING
  // coffee->scale(10);

  // TODO: match the object number
  geometrys.push_back(cake);              // geometry[0]
  geometrys.push_back(gingerbreadHouse);  // geometry[1]
  geometrys.push_back(bomb);              // geometry[2]
  geometrys.push_back(gun);               // geometry[3]
  geometrys.push_back(mortarshell);
  geometrys.push_back(shoe);
  geometrys.push_back(coffee);

  // plsPoints = new PointCloud("sphere.obj", -1);

  // plsPoints->scale(0.3);
  // plsPoints->translate(0.0, 2.0, 0.0);

  // init material values
  //    bunnyPoints->mat.color = glm::vec3(1.0f, 0.0f, 0.0f);
  //    bunnyPoints->mat.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
  //    bunnyPoints->mat.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
  //    bunnyPoints->mat.specular = glm::vec3(0.5f, 0.5f, 0.5f);
  //    bunnyPoints->mat.shininess = 1.0f;

  // cake->mat.color = glm::vec3(1.0f, 0.0f, 0.0f);
  // cake->mat.ambient = glm::vec3(0.5f);
  // cake->mat.diffuse = glm::vec3(0.0f);
  // cake->mat.specular = glm::vec3(0.5f, 0.5f, 0.5f);
  // cake->mat.shininess = 64.0f;

  // medicine->mat.color = glm::vec3(1.0f, 0.0f, 0.0f);
  // mortarshell->mat.color = glm::vec3(0.0f, 1.0f, 0.0f);
  // shield->mat.color = glm::vec3(1.0f, 0.0f, 1.0f);
  // gingerbreadHouse->mat.color = glm::vec3(1.0f, 0.0f, 0.0f);
  // bomb->mat.color = glm::vec3(1.0f, 0.0f, 0.0f);
  // glove->mat.color = glm::vec3(1.0f, 0.0f, 0.0f);
  // gun->mat.color = glm::vec3(1.0f, 0.0f, 0.0f);

  directLight = new Light(false);

  // bind light
  // glUniform3fv(glGetUniformLocation(program, "DirLight.direction"), 1,
  //              glm::value_ptr(glm::vec3(0.5f)));
  // glUniform3fv(glGetUniformLocation(program, "DirLight.position"), 1,
  //              glm::value_ptr(glm::vec3(0.0f)));
  // glUniform3fv(glGetUniformLocation(program, "DirLight.ambient"), 1,
  //              glm::value_ptr(glm::vec3(0.0f)));
  // glUniform3fv(glGetUniformLocation(program, "DirLight.diffuse"), 1,
  //              glm::value_ptr(glm::vec3(1.0f)));
  // glUniform3fv(glGetUniformLocation(program, "DirLight.specular"), 1,
  //              glm::value_ptr(glm::vec3(1.0f)));
  // glUniform1f(glGetUniformLocation(program, "DirLight.constant"), 1.0f);
  // glUniform1f(glGetUniformLocation(program, "DirLight.linear"), 0.35f);
  // glUniform1f(glGetUniformLocation(program, "DirLight.quadratic"), 0.44f);

  // bind other values
  glUniform3fv(glGetUniformLocation(program, "eyePos"), 1, glm::value_ptr(eye));

  return true;
}

void Window::cleanUp() {
  // Deallcoate the objects.
  delete plsPoints;
  delete directLight;
  //   delete quad;

  // Delete the shader programs.
  glDeleteProgram(program);
  // glDeleteProgram(programQuad);
}

GLFWwindow* Window::createWindow(int width, int height) {
  // Initialize GLFW.
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return NULL;
  }

  // 4x antialiasing.
  glfwWindowHint(GLFW_SAMPLES, 4);

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

#ifndef __APPLE__
  // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

  // Initialize GLEW.
  if (glewInit()) {
    std::cerr << "Failed to initialize GLEW" << std::endl;
    return NULL;
  }
#endif

  // Set swap interval to 1.
  glfwSwapInterval(0);

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
  if (enableDirectionalLight)
    glUniform3fv(
        glGetUniformLocation(program, "DirLight.direction"), 1,
        glm::value_ptr(glm::vec3(cos(++directLight->degree / 500.0), 0.0f,
                                 sin(directLight->degree / 500.0))));

  //    std::cout << cos(directLight->degree / 10.0) <<'\n';
}

void Window::displayCallback(GLFWwindow* window) {
  // Switch back to using OpenGL's rasterizer
  glUseProgram(program);
  // Clear the color and depth buffers.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Specify the values of the uniform variables we are going to use.
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

  // TESTING: different locations
  glm::vec3 locations[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),   glm::vec3(-1.5f, -2.2f, 0.0f),
      glm::vec3(-3.8f, -2.0f, 0.0f), glm::vec3(5.0f, 5.0f, 0.0f),
      glm::vec3(-1.5f, -2.2f, 0.0f), glm::vec3(-3.8f, -2.0f, 1.3f),
      glm::vec3(3.0f, -4.0f, 0.0f),  glm::vec3(-2.5f, -2.2f, 0.5f),
      glm::vec3(-3.8f, 2.0f, 1.3f),
  };

  // rendering
  /* for each input data of client
   * data contains object number and location
   */
  // TODO: change the loop to loop for client input
  for (int i = 0; i < geometrys.size(); i++) {
    PointCloud* currentObj = geometrys[i];
    // mat
    glm::mat4 curr_model = glm::translate(currentObj->getModel(), locations[i]);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(curr_model));

    glUniform3fv(colorLoc, 1, glm::value_ptr(currentObj->mat.color));
    glUniform3fv(diffuseLoc, 1, glm::value_ptr(currentObj->mat.diffuse));
    glUniform3fv(ambientLoc, 1, glm::value_ptr(currentObj->mat.ambient));
    glUniform3fv(specularLoc, 1, glm::value_ptr(currentObj->mat.specular));
    glUniform1f(shininessLoc, currentObj->mat.shininess);

    int coloringType = 0;
    // if (enablePointLight) coloringType += 2;
    // if (enableDirectionalLight) coloringType += 1;
    if (!enablePhongColoring) coloringType = 4;
    glUniform1i(glGetUniformLocation(program, "coloringType"), coloringType);

    // Render the object.
    currentObj->draw();
  }

  // Gets events, including input such as keyboard and mouse or window resizing.
  glfwPollEvents();
  // Swap buffers.
  glfwSwapBuffers(window);
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
