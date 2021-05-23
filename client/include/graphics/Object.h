#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

class Object {
 protected:
  glm::mat4 model;
  glm::vec3 color;

 public:
  glm::mat4 getModel() { return model; }
  glm::vec3 getColor() { return color; }

  void setModel(glm::mat4 m) { model = m; }

  // virtual void draw() = 0;
  // virtual void update() = 0;

  void translate(GLfloat dx, GLfloat dy, GLfloat dz) {
    model = glm::translate(glm::mat4(1), glm::vec3(dx, dy, dz)) * model;
  }

  void moveTo(GLfloat x, GLfloat y, GLfloat z) {
    model[3][0] = x;
    model[3][1] = y;
    model[3][2] = z;
    model[3][3] = 1;
  }

  void rotate(float angle, float dx, float dy, float dz) {
    model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(dx, dy, dz)) * model;
  }
  void spin(float angle, float dx, float dy, float dz) {
    model = glm::rotate(model, angle, glm::vec3(dx, dy, dz));
  }
  void scale(GLfloat dx) {
    model = glm::scale(getModel(), glm::vec3(dx, dx, dx));
  }

  // struct Material {
  //   glm::vec3 color;
  //   glm::vec3 ambient;
  //   glm::vec3 diffuse;
  //   glm::vec3 specular;
  //   float shininess;
  // } mat;
};

#endif
