#include "EvanArm.h"

using namespace glm;

EvanArm::EvanArm(vec3 initialPos, vec3 scale) {
    vec3 ARM_COLOR = vec3(0.8f, 0.65f, 0.37f);

    this->groupMatrix = translate(mat4(1.0f), initialPos) ;
    this->worldMatrix = mat4(1.0f);
    this->modelScale = scale;

    // Create VAO for each part of the arm
    this->lowerArmVAO = createUnitCubeVAO(ARM_COLOR,
                                          ARM_COLOR,
                                          ARM_COLOR,
                                          ARM_COLOR,
                                          ARM_COLOR,
                                          ARM_COLOR);

    this->upperArmVAO = createUnitCubeVAO(ARM_COLOR,
                                          ARM_COLOR,
                                          ARM_COLOR,
                                          ARM_COLOR,
                                          ARM_COLOR,
                                          ARM_COLOR);

}

void EvanArm::drawLowerArm(GLuint modelMatrixLocation, GLuint shaderProgram) const {
    glUseProgram(shaderProgram);

    // Transformations to manipulate the cube to make part of the model
    mat4 modelMatrix=this->worldMatrix * this->groupMatrix;
    modelMatrix = scale(modelMatrix,modelScale);
    modelMatrix = rotate(modelMatrix,radians(60.0f),vec3(0.0f,0.0f,-1.0f));
    modelMatrix = scale(modelMatrix,vec3(1.0f,4.0f,1.0f));


    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));

    // Render it
    glBindVertexArray(this->upperArmVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void EvanArm::drawUpperArm(GLuint modelMatrixLocation, GLuint shaderProgram) const {
    glUseProgram(shaderProgram);

    // Transformations to manipulate the cube to make part of the model
    mat4 modelMatrix=this->worldMatrix * this->groupMatrix;
    modelMatrix = scale(modelMatrix,modelScale);
    modelMatrix = translate(modelMatrix,vec3(2.0f,3.0f,0.0f));
    modelMatrix = scale(modelMatrix,vec3(1.0f,4.0f,1.0f));

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));

    // Render it
    glBindVertexArray(this->lowerArmVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void EvanArm::draw(GLuint modelMatrixLocation, GLuint shaderProgram) const {
    drawLowerArm(modelMatrixLocation, shaderProgram);
    drawUpperArm(modelMatrixLocation, shaderProgram);
}

void EvanArm::setGroupMatrix(mat4 newGroupMatrix) {
    this->groupMatrix = newGroupMatrix;
}

void EvanArm::setWorldMatrix(mat4 newWorldMatrix) {
    this->worldMatrix = newWorldMatrix;
}

