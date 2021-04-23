#include "Window.h"

/*
 * Declare your variables below. Unnamed namespace is used here to avoid
 * declaring global or static variables.
 */
namespace
{
int width, height;
std::string windowTitle("GLFW Starter Project");

PointCloud* bunnyPoints;
PointCloud* dragonPoints;
PointCloud* bearPoints;
PointCloud* plsPoints; // point light sphere

PointCloud* currentObj;  // The object currently displaying.

Light* directLight;
Light* pointLight;

RasterizerQuad* quad;  // Object textured with your rasterization results

glm::vec3 eye(0, 0, 20); // Camera position.
glm::vec3 center(0, 0, 0); // The point we are looking at.
glm::vec3 up(0, 1, 0); // The up direction of the camera.
float fovy = 60;
float near = 1;
float far = 1000;
glm::mat4 view = glm::lookAt(eye, center, up); // View matrix, defined by eye, center and up.
glm::mat4 projection; // Projection matrix.

GLuint program; // The shader program id.
GLuint projectionLoc; // Location of projection in shader.
GLuint viewLoc; // Location of view in shader.
GLuint modelLoc; // Location of model in shader.

GLuint colorLoc;
GLuint ambientLoc;
GLuint diffuseLoc;
GLuint specularLoc;
GLuint shininessLoc;

GLuint programQuad;

bool enablePhongColoring = 0;
bool enableDirectionalLight = 0;
bool enablePointLight = 0;
bool enableControlPointLight = 0;

bool isMouseClicked[3] = {0,0,0};
double lastXpos, lastYpos;
};

bool Window::initializeProgram()
{
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	// This shader program is for displaying your rasterizer results
	// DO NOT MODIFY THESE SHADER PROGRAMS
	programQuad = LoadShaders("shaders/RasterizerQuad.vert", "shaders/RasterizerQuad.frag");

	// Check the shader programs.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
	if (!programQuad)
	{
		std::cerr << "Failed to initialize shader quad program" << std::endl;
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

bool Window::initializeObjects()
{
	bunnyPoints = new PointCloud("/Users/apple/Documents/CSE/167/pa1/pa1/bunny.obj", 1);
    dragonPoints = new PointCloud("/Users/apple/Documents/CSE/167/pa1/pa1/dragon.obj", 1);
    bearPoints = new PointCloud("/Users/apple/Documents/CSE/167/pa1/pa1/bear.obj", 1);
    plsPoints = new PointCloud("/Users/apple/Documents/CSE/167/pa1/pa1/sphere.obj", -1);

    plsPoints->scale(0.3);
    plsPoints->translate(0.0, 2.0, 0.0);

    // init material values
//    bunnyPoints->mat.color = glm::vec3(1.0f, 0.0f, 0.0f);
//    bunnyPoints->mat.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
//    bunnyPoints->mat.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
//    bunnyPoints->mat.specular = glm::vec3(0.5f, 0.5f, 0.5f);
//    bunnyPoints->mat.shininess = 1.0f;

    bearPoints->mat.color = glm::vec3(1.0f, 0.0f, 0.0f);
    bearPoints->mat.ambient = glm::vec3(0.5f);
    bearPoints->mat.diffuse = glm::vec3(0.0f);
    bearPoints->mat.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    bearPoints->mat.shininess = 64.0f;
    
    dragonPoints->mat.color = glm::vec3(1.0f, 0.0f, 0.0f);
    dragonPoints->mat.ambient = glm::vec3(0.1f);
    dragonPoints->mat.diffuse = glm::vec3(1.0f);
    dragonPoints->mat.specular = glm::vec3(1.0f);
    dragonPoints->mat.shininess = 0.0f;
    
    bunnyPoints->mat.color = glm::vec3(1.0f, 0.0f, 0.0f);
    bunnyPoints->mat.ambient = glm::vec3(0.5f);
    bunnyPoints->mat.diffuse = glm::vec3(1.0f);
    bunnyPoints->mat.specular = glm::vec3(1.0f);
    bunnyPoints->mat.shininess = 16.0f;


    pointLight = new Light(true);
    directLight = new Light(false);
    pointLight->translate(0.0, 2.0, 0.0);

    // bind light
    glUniform3fv(glGetUniformLocation(program, "DirLight.direction"), 1, glm::value_ptr(glm::vec3(0.5f)));
    glUniform3fv(glGetUniformLocation(program, "DirLight.position"), 1, glm::value_ptr(glm::vec3(0.0f)));
    glUniform3fv(glGetUniformLocation(program, "DirLight.ambient"), 1, glm::value_ptr(glm::vec3(0.0f)));
    glUniform3fv(glGetUniformLocation(program, "DirLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0f)));
    glUniform3fv(glGetUniformLocation(program, "DirLight.specular"), 1, glm::value_ptr(glm::vec3(1.0f)));
    glUniform1f(glGetUniformLocation(program, "DirLight.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(program, "DirLight.linear"), 0.35f);
    glUniform1f(glGetUniformLocation(program, "DirLight.quadratic"), 0.44f);

    glUniform3fv(glGetUniformLocation(program, "PointLight.direction"), 1, glm::value_ptr(glm::vec3(0.0f)));
    glUniform3fv(glGetUniformLocation(program, "PointLight.position"), 1, glm::value_ptr(glm::vec3(0.0f, 2.0f, 0.0f)));
    glUniform3fv(glGetUniformLocation(program, "PointLight.ambient"), 1, glm::value_ptr(glm::vec3(0.0f)));
    glUniform3fv(glGetUniformLocation(program, "PointLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0f)));
    glUniform3fv(glGetUniformLocation(program, "PointLight.specular"), 1, glm::value_ptr(glm::vec3(1.0f)));
    glUniform1f(glGetUniformLocation(program, "PointLight.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(program, "PointLight.linear"), 0.35f);
    glUniform1f(glGetUniformLocation(program, "PointLight.quadratic"), 0.44f);

    // bind other values
    glUniform3fv(glGetUniformLocation(program, "eyePos"), 1, glm::value_ptr(eye));

    currentObj = bunnyPoints;
    return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
    delete bunnyPoints; delete dragonPoints; delete bearPoints; delete plsPoints;
    delete pointLight; delete directLight;
	delete quad;

	// Delete the shader programs.
	glDeleteProgram(program);
	glDeleteProgram(programQuad);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
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
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Initialize the quad that will be textured with your image
	// The quad must be made with the window
	quad = new RasterizerQuad(width, height);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int w, int h)
{
    width = w;
    height = h;
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height);
#endif


	// Resize our CPU rasterizer's pixel buffer and zbuffer
	quad->updateBufSiz(width, height);

	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	projection = glm::perspective(glm::radians(fovy),
		(float)width / (float)height, near, far);
}

void Window::idleCallback()
{
	// Perform any updates as necessary.
    if(enableDirectionalLight) glUniform3fv(glGetUniformLocation(program, "DirLight.direction"), 1, glm::value_ptr(glm::vec3(cos(++directLight->degree / 500.0), 0.0f, sin(directLight->degree / 500.0))));

//    std::cout << cos(directLight->degree / 10.0) <<'\n';
}

void Window::displayCallback(GLFWwindow* window)
{
    // Switch back to using OpenGL's rasterizer
    glUseProgram(program);
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Specify the values of the uniform variables we are going to use.
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // mat
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(currentObj->getModel()));
    glUniform3fv(colorLoc, 1, glm::value_ptr(currentObj->mat.color));
    glUniform3fv(diffuseLoc, 1, glm::value_ptr(currentObj->mat.diffuse));
    glUniform3fv(ambientLoc, 1, glm::value_ptr(currentObj->mat.ambient));
    glUniform3fv(specularLoc, 1, glm::value_ptr(currentObj->mat.specular));
    glUniform1f(shininessLoc, currentObj->mat.shininess);


    int coloringType = 0;
    if(enablePointLight) coloringType += 2;
    if(enableDirectionalLight) coloringType += 1;
    if(!enablePhongColoring) coloringType = 4;
    glUniform1i(glGetUniformLocation(program, "coloringType"), coloringType);
    glUniform3fv(glGetUniformLocation(program, "PointLight.position"), 1, glm::value_ptr(pointLight->position));
    // Render the object.
    currentObj->draw();


    if(enablePointLight) {
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(plsPoints->getModel()));
        glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(1.0f,1.0f,1.0f)));
        plsPoints->draw();
    }


    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Check for a key press.
    if (action != GLFW_PRESS) return;

    // Uppercase key presses (shift held down + key press)
    if (mods == GLFW_MOD_SHIFT) switch (key) {
        case GLFW_KEY_S: currentObj -> size(1.25); break;
        default: break;
    }

    // Deals with lowercase key presses
    else switch (key)
    {
    // Close the window. This causes the program to also terminate.
        case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE);  break;

        case GLFW_KEY_F1: currentObj = bunnyPoints; break;
        case GLFW_KEY_F2: currentObj = dragonPoints; break;
        case GLFW_KEY_F3: currentObj = bearPoints; break;

        case GLFW_KEY_N: enablePhongColoring = !enablePhongColoring; break;

        case GLFW_KEY_0: enableControlPointLight = !enableControlPointLight; break;
        case GLFW_KEY_1:
            enableDirectionalLight = 1;
            enablePointLight = 0;
            enableControlPointLight = 0;
            break;
        case GLFW_KEY_2:
            enableDirectionalLight = 1;
            enablePointLight = 1;
            enableControlPointLight = 1;
            break;
        case GLFW_KEY_4: enableDirectionalLight = !enableDirectionalLight; break;

        case GLFW_KEY_S: currentObj -> size(0.8); break;

        default: break;
    }
}

void Window::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    GLfloat dy = yoffset;
    std::cout << "dz " << dy << '\n';
    if(enableControlPointLight) {
        plsPoints->translate(0.0, 0.0, dy);
        pointLight->translate(0.0, 0.0, dy);
    } else currentObj->translate(0.0, 0.0, dy);
}

void Window::cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) {
    auto dx = (xpos - lastXpos) / 20, dy = (lastYpos - ypos) / 20;
    if(isMouseClicked[0]) {
//        std::cout << pointLight->position[0]  << "  " << pointLight->position[1] << "  " << pointLight->position[2] << '\n';
        if(enableControlPointLight) {
            auto degree = 100*(dx*dx+dy*dy)/(xpos+ypos+1);
//            auto axis = glm::cross(pointLight->position, pointLight->position + glm::vec3(dx,dy,0.0));
            plsPoints->rotate(degree, -dy, dx, 0.0f);
            auto pls_model = plsPoints->getModel();
            pointLight->position = glm::vec3(pls_model[3][0], pls_model[3][1], pls_model[3][2]);
        } else {
            auto degree = 100*(dx*dx+dy*dy)/(xpos+ypos+1);

//            auto axis = glm::cross(currentObj->getModel(), <#const vec<3, T, Q> &y#>)
//            plsPoints->rotate(degree, -dy, dx, 0.0);/
            currentObj->rotate(degree, -dy, dx, 0.0);


        }
    }
    else if(isMouseClicked[1]) {
        std::cout << "dx " << dx << "dy " << dy << '\n';
        if(enableControlPointLight) {
            plsPoints->translate(dx, dy, 0.0f);
            pointLight->translate(dx, dy, 0.0f);
            glUniform3fv(glGetUniformLocation(program, "PointLight.position"), 1, glm::value_ptr(pointLight->position));
        } else currentObj->translate(dx, dy, 0.0);
    }

    lastXpos = xpos;
    lastYpos = ypos;
}

void Window::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if(button < 0 || button >=  3) return;
    if(action == GLFW_PRESS) isMouseClicked[button] = 1;
    if(action == GLFW_RELEASE) isMouseClicked[button] = 0;
}
