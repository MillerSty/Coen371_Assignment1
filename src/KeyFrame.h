#pragma once

#include <glm/gtc/matrix_transform.hpp>

class KeyFrame {
public:
    glm::vec3 translation;
    glm::vec3 rotation;
    double time;

    KeyFrame(glm::vec3 trans, glm::vec3 rot, double t)  { translation = trans;
    rotation = rot;
    time = t; }

};


