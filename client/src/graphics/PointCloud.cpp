#include "PointCloud.h"

#include <sstream>

#include "fstream"
#include "iostream"

PointCloud::PointCloud(std::string objFilename, GLfloat pointSize)
    : pointSize(pointSize) {
  std::cout << "\nreading file " << objFilename << " ...\n";
  std::ifstream ifs(objFilename);
  if (!ifs.is_open()) {
    std::cerr << "failed reading file\n";
    exit(1);
  }
  std::vector<glm::vec3> upscaled_points;
  std::string l, type;
  unsigned int v1, v2, v3, t1, t2, t3, n1, n2, n3;
  unsigned int x1, y1, z1, x2, y2, z2;
  float x, y, z;
  float maxx = FLT_MIN, maxy = FLT_MIN, maxz = FLT_MIN;
  float minx = FLT_MAX, miny = FLT_MAX, minz = FLT_MAX;
  while (std::getline(ifs, l)) {
    if (l.size() == 0 || l.at(0) == '#') continue;
    std::istringstream iss(l);
    iss >> type;
    if (type == "v") {
      iss >> x >> y >> z;
      if (maxx < x) maxx = x;
      if (maxy < y) maxy = y;
      if (maxz < z) maxz = z;
      if (minx > x) minx = x;
      if (miny > y) miny = y;
      if (minz > z) minz = z;
      upscaled_points.push_back(glm::vec3(x, y, z));
      lightType.push_back(pointSize);
    } else if (type == "vn") {
      iss >> x >> y >> z;
      vn.push_back(glm::vec3(x, y, z));
    } else if (type == "vt") {
    } else if (type == "f") {
      // iss >> x1;
      // iss.get();
      // iss.get();
      // iss >> x2;
      // iss >> y1;
      // iss.get();
      // iss.get();
      // iss >> y2;
      // iss >> z1;
      // iss.get();
      // iss.get();
      // iss >> z2;
      // face.push_back(--x1);
      // face.push_back(--y1);
      // face.push_back(--z1);
      iss >> v1;
      if (iss.get() == '/') iss >> t1;
      if (iss.get() == '/') iss >> n1;
      iss >> v2;
      if (iss.get() == '/') iss >> t2;
      if (iss.get() == '/') iss >> n2;
      iss >> v3;
      if (iss.get() == '/') iss >> t3;
      if (iss.get() == '/') iss >> n3;
      face.push_back(--v1);
      face.push_back(--v2);
      face.push_back(--v3);
      //            face_t.push_back(--t1); face_t.push_back(--t2);
      //            face_t.push_back(--t3);
      // face_n.push_back(--n1);
      // face_n.push_back(--n2);
      // face_n.push_back(--n3);
    }
  }
  ifs.close();

  model = glm::mat4(1);
  color = glm::vec3(1, 0, 0);

  // center all points
  auto disx = (maxx + minx) / 2, disy = (maxy + miny) / 2,
       disz = (maxz + minz) / 2;

  auto dx = maxx - minx, dy = maxy - miny, dz = maxz - minz;
  auto _temp = dx > dy ? dx : dy;
  auto max_d = _temp > dz ? _temp : dz;
  auto scalerate = 2 / max_d;

  // first move, then scale
  for (auto v : upscaled_points)
    points.push_back(glm::vec3((v[0] - disx) * scalerate,
                               (v[1] - disy) * scalerate,
                               (v[2] - disz) * scalerate));

  // std::vector<unsigned int> _face, _face_n;
  // std::vector<glm::vec3> _vs, _vns;
  // for (int i = 0; i < face.size(); i++) {
  //   _vs.push_back(points[face[i]]);
  //   _vns.push_back(vn[face_n[i]]);
  //   _face.push_back(i);
  //   _face_n.push_back(i);
  // }
  // points = _vs;
  // vn = _vns;
  // face = _face;
  // face_n = _face_n;

  // Generate a vertex array (VAO) and a vertex buffer objects (VBO).
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // vertex
  glGenBuffers(1, &vbo_v);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_v);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
               points.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

  // vertex normal
  glGenBuffers(1, &vbo_vn);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vn);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vn.size(), vn.data(),
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (GLvoid*)0);

  // obey rule?
  glGenBuffers(2, &vbo_obey);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_obey);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * lightType.size(),
               lightType.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);

  // faces
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLfloat) * face.size(),
               face.data(), GL_STATIC_DRAW);

  // textures

  // Unbind from the VBO / VAO.
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

PointCloud::~PointCloud() {
  // Delete the VBO and the VAO.
  // Failure to delete your VAOs, VBOs and other data given to OpenGL
  // is dangerous and may slow your program and cause memory leaks
  glDeleteBuffers(1, &vbo_v);
  glDeleteBuffers(1, &vbo_vn);
  glDeleteBuffers(1, &ebo);
  glDeleteVertexArrays(1, &vao);
}

void PointCloud::draw() {
  // Bind to the VAO.
  glBindVertexArray(vao);
  // Set point size.
  //	glPointSize(pointSize);
  // Draw points
  //	glDrawArrays(GL_POINTS, 0, points.size());
  // Draw faces
  glDrawElements(GL_TRIANGLES, face.size(), GL_UNSIGNED_INT, 0);
  // Unbind from the VAO.
  glBindVertexArray(0);
}

void PointCloud::update() {}
