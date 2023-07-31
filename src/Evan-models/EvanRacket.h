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
    /*
    * Functions which model and the draw the various part of the tennis racket
    */
    void drawLowerFrame(GLuint modelMatrixLocation, GLuint objectColorLocation, GLuint shaderProgram) const;
    void drawUpperFrame(GLuint modelMatrixLocation, GLuint objectColorLocation, GLuint shaderProgram) const;
    void drawLeftFrame(GLuint modelMatrixLocation, GLuint objectColorLocation, GLuint shaderProgram) const;
    void drawRightFrame(GLuint modelMatrixLocation, GLuint objectColorLocation, GLuint shaderProgram) const;
public:
    mat4 groupMatrix;
    mat4 wristMatrix;
    mat4 worldMatrix;
    /*
     * This constructor creates the VAOs & sets the initial group matrix
     */
    EvanRacket(vec3 initialPos, vec3 scale, GLuint handleVAO, GLuint topHandleVAO, GLuint lowerFrameVAO,
               GLuint upperFrameVAO, GLuint leftFrameVAO, GLuint rightFrameVAO);
    /*
     * Just calls all the draw functions above
     */
    void draw(GLuint racketTexutre, GLuint modelMatrixLocation, GLuint objectColorLocation, GLuint shaderProgram) const;

};


