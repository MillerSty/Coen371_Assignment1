#pragma once

#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/common.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include "UnitCube.h"

class EvanRacket {
private:
    GLuint handleVAO;
    GLuint topHandleVAO;
    GLuint lowerFrameVAO;
    GLuint upperFrameVAO;
    GLuint leftFrameVAO;
    GLuint rightFrameVAO;

    mat4 groupMatrix;
    mat4 worldMatrix;

    vec3 modelScale;

    /*
    * Functions which model and the draw the various part of the tennis racket
    */
    void drawHandle(GLuint modelMatrixLocation, GLuint shaderProgram) const;
    void drawTopHandle(GLuint modelMatrixLocation, GLuint shaderProgram) const;
    void drawLowerFrame(GLuint modelMatrixLocation, GLuint shaderProgram) const;
    void drawUpperFrame(GLuint modelMatrixLocation, GLuint shaderProgram) const;
    void drawLeftFrame(GLuint modelMatrixLocation, GLuint shaderProgram) const;
    void drawRightFrame(GLuint modelMatrixLocation, GLuint shaderProgram) const;
public:

    /*
     * This constructor creates the VAOs & sets the initial group matrix
     */
    explicit EvanRacket(vec3 initialPos, vec3 scale);
    /*
     * Just calls all the draw functions above
     */
    void draw(GLuint modelMatrixLocation, GLuint shaderProgram) const;
    // Setters
    void setGroupMatrix(mat4 newGroupMatrix);
    void setWorldMatrix(mat4 newWorldMatrix);
};


