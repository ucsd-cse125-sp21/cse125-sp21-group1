//
//  ModelMatrix.h
//  pa1
//
//  Created by Carl on 2021/5/20.
//  Copyright © 2021 apple. All rights reserved.
//

#ifndef ModelMatrix_h
#define ModelMatrix_h

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

class ModelMatrix {
    
    // Opengl coordinate system
    // up:                +y  down:            -y
    // right:             +x  left:            -x
    // out of the scren : +z  into the screen: -z
    
   public:
    glm::mat4 model;
    
    /* ctor, de-ctor */
    ModelMatrix() : model(glm::mat4(1.0f)) {}      // default ctor
    ModelMatrix(glm::mat4 model) : model(model) {} // init with a matrix
    ModelMatrix(float x, float y, float z) {       // init with a location
        model = glm::mat4(1.0f);
        model[3][0] = x; model[3][1] = y; model[3][2] = z;
    }
    ~Transform() {}
    
    
    /* translate to a relative loaction */
    void translate(GLfloat dx, GLfloat dy, GLfloat dz)
        { model = glm::translate(model, glm::vec3(dx, dy, dz)); }
    void translate(glm::vec3 d) { model = glm::translate(model, d);}
    
    /* scale */
    void scale(GLfloat dx) //scaling
        { model = glm::scale(model, glm::vec3(dx, dx, dx));}
    void scale(GLfloat dx, GLfloat dy, GLfloat dz) //scaling
    { model = glm::scale(model, glm::vec3(dx, dy, dz));}
    
    /* absolute transfotmations */
    void move_to(GLfloat x, GLfloat y, GLfloat z) {model[3][0] = x; model[3][1] = y; model[3][2] = z;}
    void move_to(glm::vec3 c) {model[3][0] = c[0]; model[3][1] = c[1]; model[3][2] = c[2];}
    
    /* Note all rotations are counterclockwise, angle is in radian */
    /* Use to convert glm::radians(90.0f) to convert from degree (90 in this case) to radians */
    /* rotate around the vector that starts from the origin */
    void rotate(float angle, float vx, float vy, float vz)
        {model = glm::rotate(model, angle, glm::vec3(vx, vy, vz));}
    /* TODO rotate around a vector at a given point */
    void rotate(float angle, float vx, float vy, float vz, float x, float y, float z) {
//        auto l = glm::mat4(1.0f); l[3][0] = x; l[3][1] = y; l[3][2] = z;
//        auto r = l; l[3][0] = -x; r[3][1] = -y; r[3][2] = -z;
//        model = glm::rotate(l, angle, glm::vec3(vx, vy, vz)) * r * model;
    }
    /* TODO spin around the vector that starts from the object center */
    void spin(float angle, float vx, float vy, float vz) {
//        auto l = glm::mat4(1.0f); l[3][0] = x; l[3][1] = y; l[3][2] = z;
//        auto r = l; l[3][0] = -x; r[3][1] = -y; r[3][2] = -z;
//        model = glm::rotate(l, angle, glm::vec3(vx, vy, vz)) * r * model;
    }
    
    /* value access */
    glm::vec3 cord() {return glm::vec3(model[3][0], model[3][1], model[3][2]);}
    /* TODO return where the model is facing */
    int facing() {
        return 1;
    }
};

#endif /* ModelMatrix_h */
