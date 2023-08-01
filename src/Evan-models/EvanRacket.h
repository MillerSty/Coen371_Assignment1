#pragma once

#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/common.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

using namespace glm;

class EvanRacket {
private:
    GLuint handleVAO;
    GLuint topHandleVAO;
    GLuint lowerFrameVAO;
    GLuint upperFrameVAO;
    GLuint leftFrameVAO;
    GLuint rightFrameVAO;

    vec3 modelScale;

    vec3 frameColor;
    vec3 handleColor;

public:
    mat4 worldMatrix;
    mat4 groupMatrix;
    mat4 bicepMatrix;
    mat4 wristMatrix;

    /*
     * This constructor creates the VAOs & sets the initial group matrix
     */
    EvanRacket(vec3 initialPos, vec3 scale, GLuint handleVAO, GLuint topHandleVAO, GLuint lowerFrameVAO,
               GLuint upperFrameVAO, GLuint leftFrameVAO, GLuint rightFrameVAO);
    /*
    * Functions which model and the draw the various part of the tennis racket
    */
    void draw(GLuint racketTexutre, GLuint modelMatrixLocation, GLuint objectColorLocation, GLuint shaderProgram) const;
//    void draw(GLuint racketTexutre, mat4 worldMatrix, mat4 groupMatrix, mat4 bicepMatrix, GLuint modelMatrixLocation, GLuint objectColorLocation, GLuint shaderProgram) const;
};


