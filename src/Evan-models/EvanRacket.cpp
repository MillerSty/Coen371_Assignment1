#include "EvanRacket.h"

using namespace glm;

EvanRacket::EvanRacket(vec3 initialPos, vec3 scale) {
    vec3 FRAME_COLOR(0.98f, 0.23f, 0.12f);
    vec3 HANDLE_COLOR(0.04f, 0.12f, 0.61f);

    this->groupMatrix = translate(mat4(1.0f), initialPos) ;
    this->worldMatrix = mat4(1.0f);
    this->modelScale = scale;

    // Create VAOs for each of the parts and set them
    this->handleVAO = createUnitCubeVAO(HANDLE_COLOR, HANDLE_COLOR, HANDLE_COLOR,
                                        HANDLE_COLOR,HANDLE_COLOR, HANDLE_COLOR);

    this->topHandleVAO = createUnitCubeVAO(FRAME_COLOR, FRAME_COLOR, FRAME_COLOR,
                                           FRAME_COLOR,FRAME_COLOR, FRAME_COLOR);

    this->lowerFrameVAO = createUnitCubeVAO(FRAME_COLOR, FRAME_COLOR, FRAME_COLOR,
                                            FRAME_COLOR,FRAME_COLOR, FRAME_COLOR);

    this->upperFrameVAO = createUnitCubeVAO(FRAME_COLOR, FRAME_COLOR, FRAME_COLOR,
                                            FRAME_COLOR,FRAME_COLOR, FRAME_COLOR);

    this->leftFrameVAO = createUnitCubeVAO(FRAME_COLOR, FRAME_COLOR, FRAME_COLOR,
                                           FRAME_COLOR,FRAME_COLOR, FRAME_COLOR);

    this->rightFrameVAO = createUnitCubeVAO(FRAME_COLOR, FRAME_COLOR, FRAME_COLOR,
                                            FRAME_COLOR,FRAME_COLOR, FRAME_COLOR);
}

void EvanRacket::drawHandle(GLuint modelMatrixLocation, GLuint shaderProgram) const {
    glUseProgram(shaderProgram);

    // Transformations to manipulate the cube to make part of the model
    mat4 modelMatrix = this->worldMatrix * this->groupMatrix;
    modelMatrix = scale(modelMatrix,modelScale);
    modelMatrix = translate(modelMatrix,vec3(0.5f+ 1.5f,1.75f + 5.0f,0.0f));
    modelMatrix = scale(modelMatrix,vec3(0.5f,3.5f,0.5f));

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));

    // Render it
    glBindVertexArray(this->handleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void EvanRacket::drawTopHandle(GLuint modelMatrixLocation, GLuint shaderProgram) const {
    glUseProgram(shaderProgram);

    // Transformations to manipulate the cube to make part of the model
    mat4 modelMatrix = this->worldMatrix * this->groupMatrix;
    modelMatrix = scale(modelMatrix,modelScale);
    modelMatrix = translate(modelMatrix,vec3(0.5f+ 1.5f,3.75f+ 5.0f,0.0f));
    modelMatrix = scale(modelMatrix,vec3(0.5f,0.5f,0.5f));

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));

    // Render it
    glBindVertexArray(this->topHandleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void EvanRacket::drawLowerFrame(GLuint modelMatrixLocation, GLuint shaderProgram) const {
    glUseProgram(shaderProgram);

    // Transformations to manipulate the cube to make part of the model
    mat4 modelMatrix = this->worldMatrix * this->groupMatrix;
    modelMatrix = scale(modelMatrix,modelScale);
    modelMatrix = translate(modelMatrix,vec3(0.5f+ 1.5f,4.25f+ 5.0f,0.0f));
    modelMatrix = scale(modelMatrix,vec3(3.0f,0.5f,0.5f));

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));
    glBindVertexArray(this->lowerFrameVAO);

    // Render it
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void EvanRacket::drawUpperFrame(GLuint modelMatrixLocation, GLuint shaderProgram) const {
    glUseProgram(shaderProgram);

    // Transformations to manipulate the cube to make part of the model
    mat4 modelMatrix = this->worldMatrix * this->groupMatrix;
    modelMatrix = scale(modelMatrix,modelScale);
    modelMatrix = translate(modelMatrix,vec3(0.5f+ 1.5f,8.25f+ 5.0f,0.0f));
    modelMatrix = scale(modelMatrix,vec3(3.0f,0.5f,0.5f));

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));

    // Render it
    glBindVertexArray(this->upperFrameVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void EvanRacket::drawLeftFrame(GLuint modelMatrixLocation, GLuint shaderProgram) const {
    glUseProgram(shaderProgram);

    // Transformations to manipulate the cube to make part of the model
    mat4 modelMatrix = this->worldMatrix * this->groupMatrix;
    modelMatrix = scale(modelMatrix,modelScale);
    modelMatrix = translate(modelMatrix,vec3(-1.25f+ 1.5f,6.25f+ 5.0f,0.0f));
    modelMatrix = scale(modelMatrix,vec3(0.5f,4.0f,0.5f));

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));

    // Render it
    glBindVertexArray(this->leftFrameVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void EvanRacket::drawRightFrame(GLuint modelMatrixLocation, GLuint shaderProgram) const {
    glUseProgram(shaderProgram);

    // Transformations to manipulate the cube to make part of the model
    mat4 modelMatrix = this->worldMatrix * this->groupMatrix;
    modelMatrix = scale(modelMatrix,modelScale);
    modelMatrix = translate(modelMatrix,vec3(2.25f + 1.5f,6.25f+ 5.0f,0.0f));
    modelMatrix = scale(modelMatrix,vec3(0.5f,4.0f,0.5f));

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));

    // Render it
    glBindVertexArray(this->rightFrameVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void EvanRacket::draw(GLuint modelMatrixLocation, GLuint shaderProgram) const {
    drawHandle(modelMatrixLocation, shaderProgram);
    drawTopHandle(modelMatrixLocation, shaderProgram);
    drawLowerFrame(modelMatrixLocation, shaderProgram);
    drawUpperFrame(modelMatrixLocation, shaderProgram);
    drawLeftFrame(modelMatrixLocation, shaderProgram);
    drawRightFrame(modelMatrixLocation, shaderProgram);
}


void EvanRacket::setGroupMatrix(mat4 newGroupMatrix) {
    this->groupMatrix = newGroupMatrix;
}

void EvanRacket::setWorldMatrix(mat4 newWorldMatrix) {
    this->worldMatrix = newWorldMatrix;
}
