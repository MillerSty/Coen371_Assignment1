#include "EvanRacket.h"

using namespace glm;

EvanRacket::EvanRacket(vec3 initialPos, vec3 scale, GLuint handleVAO, GLuint topHandleVAO,
                       GLuint lowerFrameVAO, GLuint upperFrameVAO, GLuint leftFrameVAO, GLuint rightFrameVAO ) {
    vec3 FRAME_COLOR(0.98f, 0.23f, 0.12f);
    vec3 HANDLE_COLOR(0.04f, 0.12f, 0.61f);

    this->frameColor = FRAME_COLOR;
    this->handleColor = HANDLE_COLOR;

    this->groupMatrix = translate(mat4(1.0f), initialPos) ;
    this->worldMatrix = mat4(1.0f);
    this->wristMatrix = mat4(1.0f);
    this->modelScale = scale;

    // Create VAOs for each of the parts and set them
    this->handleVAO = handleVAO;
    this->topHandleVAO = topHandleVAO;
    this->lowerFrameVAO = lowerFrameVAO;
    this->upperFrameVAO = upperFrameVAO;
    this->leftFrameVAO = leftFrameVAO;
    this->rightFrameVAO = rightFrameVAO;
}

void EvanRacket::drawHandle(GLuint modelMatrixLocation, GLuint objectColorLocation, GLuint shaderProgram) const {
    glUniform3fv(objectColorLocation, 1, value_ptr(handleColor));

    // Transformations to manipulate the cube to make part of the model
    mat4 modelMatrix = this->worldMatrix * this->groupMatrix;
    modelMatrix = scale(modelMatrix,modelScale);
    modelMatrix = translate(modelMatrix,vec3(0.5f+ 1.5f,1.75f + 5.0f,0.0f));
    modelMatrix = scale(modelMatrix,vec3(0.5f,3.5f,0.5f));

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));

    // Render it
    glBindVertexArray(this->handleVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void EvanRacket::drawTopHandle(GLuint modelMatrixLocation, GLuint objectColorLocation, GLuint shaderProgram) const {
    glUniform3fv(objectColorLocation, 1, value_ptr(frameColor));

    // Transformations to manipulate the cube to make part of the model
    mat4 modelMatrix = this->worldMatrix * this->groupMatrix;
    modelMatrix = scale(modelMatrix,modelScale);
    modelMatrix = translate(modelMatrix,vec3(0.5f+ 1.5f,3.75f+ 5.0f,0.0f));
    modelMatrix = scale(modelMatrix,vec3(0.5f,0.5f,0.5f));

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));

    // Render it
    glBindVertexArray(this->topHandleVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void EvanRacket::drawLowerFrame(GLuint modelMatrixLocation, GLuint objectColorLocation, GLuint shaderProgram) const {
    glUniform3fv(objectColorLocation, 1, value_ptr(frameColor));

    // Transformations to manipulate the cube to make part of the model
    mat4 modelMatrix = this->worldMatrix * this->groupMatrix;
    modelMatrix = scale(modelMatrix,modelScale);
    modelMatrix = translate(modelMatrix,vec3(0.5f+ 1.5f,4.25f+ 5.0f,0.0f));
    modelMatrix = scale(modelMatrix,vec3(3.0f,0.5f,0.5f));

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));

    // Render it
    glBindVertexArray(this->lowerFrameVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void EvanRacket::drawUpperFrame(GLuint modelMatrixLocation, GLuint objectColorLocation, GLuint shaderProgram) const {
    glUniform3fv(objectColorLocation, 1, value_ptr(frameColor));

    // Transformations to manipulate the cube to make part of the model
    mat4 modelMatrix = this->worldMatrix * this->groupMatrix;
    modelMatrix = scale(modelMatrix,modelScale);
    modelMatrix = translate(modelMatrix,vec3(0.5f+ 1.5f,8.25f+ 5.0f,0.0f));
    modelMatrix = scale(modelMatrix,vec3(3.0f,0.5f,0.5f));

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));

    // Render it
    glBindVertexArray(this->upperFrameVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void EvanRacket::drawLeftFrame(GLuint modelMatrixLocation, GLuint objectColorLocation, GLuint shaderProgram) const {
    glUniform3fv(objectColorLocation, 1, value_ptr(frameColor));

    // Transformations to manipulate the cube to make part of the model
    mat4 modelMatrix = this->worldMatrix * this->groupMatrix;
    modelMatrix = scale(modelMatrix,modelScale);
    modelMatrix = translate(modelMatrix,vec3(-1.25f+ 1.5f,6.25f+ 5.0f,0.0f));
    modelMatrix = scale(modelMatrix,vec3(0.5f,4.0f,0.5f));

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));

    // Render it
    glBindVertexArray(this->leftFrameVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void EvanRacket::drawRightFrame(GLuint modelMatrixLocation, GLuint objectColorLocation, GLuint shaderProgram) const {
    glUniform3fv(objectColorLocation, 1, value_ptr(frameColor));

    // Transformations to manipulate the cube to make part of the model
    mat4 modelMatrix = this->worldMatrix * this->groupMatrix;
    modelMatrix = scale(modelMatrix,modelScale);
    modelMatrix = translate(modelMatrix,vec3(2.25f + 1.5f,6.25f+ 5.0f,0.0f));
    modelMatrix = scale(modelMatrix,vec3(0.5f,4.0f,0.5f));

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));

    // Render it
    glBindVertexArray(this->rightFrameVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void EvanRacket::draw(GLuint modelMatrixLocation, GLuint objectColorLocation, GLuint shaderProgram) const {
    drawHandle(modelMatrixLocation, objectColorLocation, shaderProgram);
    drawTopHandle(modelMatrixLocation, objectColorLocation, shaderProgram);
    drawLowerFrame(modelMatrixLocation, objectColorLocation, shaderProgram);
    drawUpperFrame(modelMatrixLocation, objectColorLocation, shaderProgram);
    drawLeftFrame(modelMatrixLocation, objectColorLocation, shaderProgram);
    drawRightFrame(modelMatrixLocation, objectColorLocation, shaderProgram);
}