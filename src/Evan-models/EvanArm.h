#pragma once

#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/common.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

#include "EvanRacket.h"

using namespace glm;

/*
 * A class which contains vertex info and transformation matrices which is then used to draw the lower and upper arm
 */
class EvanArm {
private:

    GLuint lowerArmVAO;
    GLuint upperArmVAO;


    vec3 modelScale;
    vec3 color;

public:
    mat4 worldMatrix;
    mat4 groupMatrix;
    mat4 bicepMatrix;
    mat4 wristMatrix;

    EvanRacket racket;

    /*
     * This constructor creates the VAOs & sets the initial group matrix
     */
    explicit EvanArm(vec3 initialPos, vec3 scale, GLuint lowerArmVAO, GLuint upperArmVAO, EvanRacket racket);

    /*
     * Functions which model and the draw the various part of the arm
     */
    void draw(GLuint armTexture, GLuint modelMatrixLocation, GLuint objectColorLocation, GLuint shaderProgram) const;

};