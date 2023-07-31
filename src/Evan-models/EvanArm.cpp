#include "EvanArm.h"

using namespace glm;

EvanArm::EvanArm(vec3 initialPos, vec3 scale, GLuint lowerArmVAO, GLuint upperArmVAO, EvanRacket racket): racket(racket) {
    vec3 ARM_COLOR = vec3(0.8f, 0.65f, 0.37f);

    this->bicepMatrix = mat4(1.0f);
    this->groupMatrix = translate(mat4(1.0f), initialPos) ;
    this->worldMatrix = mat4(1.0f);

    this->modelScale = scale;
    this->color = ARM_COLOR;

    this->wristMatrix = mat4(1.0f);

    // Create VAO for each part of the arm
    this->lowerArmVAO = lowerArmVAO;
    this->upperArmVAO = upperArmVAO;

}

void EvanArm::draw(GLuint armTexture, GLuint modelMatrixLocation, GLuint objectColorLocation,  GLuint shaderProgram) const {
    // ---------------------
    // Draw lower arm
    // ---------------------
    glUniform3fv(objectColorLocation, 1, value_ptr(color));

    glActiveTexture(GL_TEXTURE0);
    GLint textureLocation = glGetUniformLocation(shaderProgram, "textureSampler");
    glBindTexture(GL_TEXTURE_2D, armTexture);
    glUniform1i(textureLocation, 0);

    // Transformations to manipulate the cube to make part of the model
    mat4 lowerArmMatrix = rotate(mat4(1.0f),radians(-60.0f),vec3(0.0f,0.0f,1.0f));
    lowerArmMatrix = scale(lowerArmMatrix,modelScale * vec3(1.0f,4.0f,1.0f ));

    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(lowerArmMatrix));
    // Render it
    glBindVertexArray(this->lowerArmVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // ---------------------
    // Draw upper arm
    // ---------------------

    // Transformations to manipulate the cube to make part of the model
    mat4 upperArmMatrix = translate(mat4(1.0f),modelScale * vec3(0.2f,0.3f,0.0f));
    upperArmMatrix = scale(upperArmMatrix, modelScale * vec3(1.0f,4.0f,1.0f ));
    // Load model matrix into the shader program
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(upperArmMatrix));

    // Render it
    glBindVertexArray(this->upperArmVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // ---------------------
    // Draw tennis racket
    // ---------------------
    racket.draw(armTexture, modelMatrixLocation, objectColorLocation, shaderProgram);
}

