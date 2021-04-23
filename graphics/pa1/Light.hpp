//
//  Light.hpp
//  pa1
//
//  Created by Carl on 2020/2/13.
//  Copyright © 2020 apple. All rights reserved.
//

#ifndef Light_hpp
#define Light_hpp

#include "Object.h"

class Light {
public:
    glm::vec3 direction;
    glm::vec3 position;
    
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
    
    bool is_point;
    int degree;
    Light(bool ispt){
        is_point = ispt;
        
    }
    void setRotate() {
        
    }
    
    void translate(GLfloat dx, GLfloat dy, GLfloat dz)
    {position = glm::vec3(position[0]+dx, position[1]+dy, position[2]+dz);}
//
//    void rotate(float angle, float dx, float dy, float dz) {model = glm::rotate(model, glm::radians(angle), glm::vec3(dx, dy, dz));} //TODO
};

#endif /* Light_hpp */
