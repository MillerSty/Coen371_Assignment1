#pragma once

#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/common.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

#include "UnitCube.h"
#include "EvanRacket.h"

using namespace glm;

/*
 * A class which contains vertex info, the group matrix and the world matrix which is then used to draw the lower and upper arm
 */
class EvanArm {
private:
    GLuint lowerArmVAO;
    GLuint upperArmVAO;

    mat4 groupMatrix;
    mat4 worldMatrix;

    vec3 modelScale;

    /*
     * Functions which model and the draw the various part of the arm
     */
    void drawLowerArm(GLuint modelMatrixLocation, GLuint shaderProgram) const;
    void drawUpperArm(GLuint modelMatrixLocation, GLuint shaderProgram) const;
public:

    /*
     * This constructor creates the VAOs & sets the initial group matrix
     */
    explicit EvanArm(vec3 initialPos, vec3 scale);
    /*
     * Just calls all the draw functions above
     */
    void draw(GLuint modelMatrixLocation, GLuint shaderProgram) const;
    // Setters
    void setGroupMatrix(mat4 newGroupMatrix);
    void setWorldMatrix(mat4 newWorldMatrix);

};