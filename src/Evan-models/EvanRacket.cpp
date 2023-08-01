#include "EvanRacket.h"

using namespace glm;

EvanRacket::EvanRacket(vec3 initialPos, vec3 scale, GLuint handleVAO, GLuint topHandleVAO,
                       GLuint lowerFrameVAO, GLuint upperFrameVAO, GLuint leftFrameVAO, GLuint rightFrameVAO ) {
    vec3 FRAME_COLOR(0.98f, 0.23f, 0.12f);
    vec3 HANDLE_COLOR(0.04f, 0.12f, 0.61f);

    this->frameColor = FRAME_COLOR;
    this->handleColor = HANDLE_COLOR;

    this->worldMatrix = mat4(1.0f);
    this->groupMatrix = translate(mat4(1.0f), initialPos) ;
    this->wristMatrix = mat4(1.0f);
    this->bicepMatrix = mat4(1.0f);
    this->modelScale = scale;

    // Create VAOs for each of the parts and set them
    this->handleVAO = handleVAO;
    this->topHandleVAO = topHandleVAO;
    this->lowerFrameVAO = lowerFrameVAO;
    this->upperFrameVAO = upperFrameVAO;
    this->leftFrameVAO = leftFrameVAO;
    this->rightFrameVAO = rightFrameVAO;
}

void EvanRacket::draw(GLuint racketTexutre, GLuint modelMatrixLocation, GLuint objectColorLocation, GLuint shaderProgram) const {

    glActiveTexture(GL_TEXTURE0);
    GLint textureLocation = glGetUniformLocation(shaderProgram, "textureSampler");
    glBindTexture(GL_TEXTURE_2D, racketTexutre);
    glUniform1i(textureLocation, 0);
    // ---------------------
    // Draw handle
    // ---------------------
    glUniform3fv(objectColorLocation, 1, value_ptr(handleColor));

    // Transformations to manipulate the cube to make part of the model
    mat4 handleMatrix = translate(mat4(1.0f),modelScale * vec3(0.2f,0.3 + 0.375f ,0.0f));
    handleMatrix = scale(handleMatrix,modelScale * vec3(0.5f ,3.5f,0.5f));
    handleMatrix = worldMatrix * groupMatrix  * bicepMatrix * wristMatrix * handleMatrix;

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(handleMatrix));

    // Render it
    glBindVertexArray(handleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // ---------------------
    // Draw top handle
    // ---------------------
    glUniform3fv(objectColorLocation, 1, value_ptr(frameColor));

    // Transformations to manipulate the cube to make part of the model
    mat4 topHandleMatrix = translate(mat4(1.0f), modelScale * vec3(0.2f, 0.3 + 0.375f + 0.2f,0.0f));
    topHandleMatrix = scale(topHandleMatrix,modelScale * vec3(0.5f,0.5f,0.5f));
    topHandleMatrix = worldMatrix * groupMatrix  * bicepMatrix * wristMatrix * topHandleMatrix;

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(topHandleMatrix));

    // Render it
    glBindVertexArray(topHandleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // ---------------------
    // Draw lower frame
    // ---------------------
    glUniform3fv(objectColorLocation, 1, value_ptr(frameColor));

    // Transformations to manipulate the cube to make part of the model
    mat4 lowerFrameMatrix = translate(mat4(1.0f),modelScale * vec3(0.2f, 0.3 + 0.375f + 0.2f + 0.05f,0.0f));
    lowerFrameMatrix = scale(lowerFrameMatrix,modelScale * vec3(3.0f,0.5f,0.5f));
    lowerFrameMatrix = worldMatrix * groupMatrix  * bicepMatrix * wristMatrix * lowerFrameMatrix;

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(lowerFrameMatrix));

    // Render it
    glBindVertexArray(lowerFrameVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // ---------------------
    // Draw Upper frame
    // ---------------------
    glUniform3fv(objectColorLocation, 1, value_ptr(frameColor));

    // Transformations to manipulate the cube to make part of the model
    mat4 upperFrameMatrix = translate(mat4(1.0f),modelScale * vec3(0.2f, 0.3 + 0.375f + 0.2f + 0.4f,0.0f));
    upperFrameMatrix = scale(upperFrameMatrix,modelScale * vec3(3.0f,0.5f,0.5f));
    upperFrameMatrix = worldMatrix * groupMatrix  * bicepMatrix * wristMatrix * upperFrameMatrix;

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(upperFrameMatrix));

    // Render it
    glBindVertexArray(upperFrameVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // ---------------------
    // Draw Left frame
    // ---------------------
    glUniform3fv(objectColorLocation, 1, value_ptr(frameColor));

    // Transformations to manipulate the cube to make part of the model
    mat4 leftFrameMatrix = translate(mat4(1.0f),modelScale * vec3(0.075, 0.3 + 0.375f + 0.2f + 0.225f,0.0f));
    leftFrameMatrix = scale(leftFrameMatrix,modelScale * vec3(0.5f,4.0f,0.5f));
    leftFrameMatrix = worldMatrix * groupMatrix  * bicepMatrix * wristMatrix * leftFrameMatrix;

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(leftFrameMatrix));

    // Render it
    glBindVertexArray(leftFrameVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // ---------------------
    // Draw Right frame
    // ---------------------
    glUniform3fv(objectColorLocation, 1, value_ptr(frameColor));

    // Transformations to manipulate the cube to make part of the model
    mat4 rightFrameMatrix = translate(mat4(1.0f),modelScale * vec3(0.325, 0.3 + 0.375f + 0.2f + 0.225f,0.0f));
    rightFrameMatrix = scale(rightFrameMatrix,modelScale * vec3(0.5f,4.0f,0.5f));
    rightFrameMatrix = worldMatrix * groupMatrix  * bicepMatrix * wristMatrix * rightFrameMatrix;

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(rightFrameMatrix));

    // Render it
    glBindVertexArray(rightFrameVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}