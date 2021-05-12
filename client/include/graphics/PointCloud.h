#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <string>
#include <vector>

#include "Object.h"

class PointCloud : public Object {
 private:
  std::vector<glm::vec3> points, vn;  // vs, vns
  std::vector<unsigned int> face, face_tex, face_n;
  std::vector<float> lightType;
  glm::vec3 spinrate;
  GLuint vao, vbo_v, vbo_vn, vbo_obey, ebo;
  GLfloat pointSize;

 public:
  PointCloud(std::string objFilename, GLfloat pointSize);
  ~PointCloud();
  void draw();
  void update();

  std::vector<glm::vec3>* getPoints() { return &points; }
  std::vector<glm::vec3>* getVn() { return &vn; }
  GLfloat getPointSize() { return pointSize; }

  void size(GLfloat dx) {
    setModel(glm::scale(getModel(), glm::vec3(dx, dx, dx)));
  }
};

#endif
