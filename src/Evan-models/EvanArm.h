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
public:
    /*
     * This constructor creates the VAOs & sets the initial group matrix
     */

   // explicit EvanArm(std::string name);
    EvanArm(vec3 initialPos, vec3 scale, GLuint lowerArmVAO, GLuint upperArmVAO, EvanRacket racket);
    EvanArm();
    void setInitial(vec3 initialPos, vec3 scale, GLuint lowerArmVAO, GLuint upperArmVAO, EvanRacket racket);

    /*
     * Functions which model and the draw the various part of the arm
     */
    void draw(GLuint armTexture, GLuint modelMatrixLocation, GLuint objectColorLocation, GLuint shaderProgram) const;
  /*
  * Functions for controlley manipulations
  */
	void setRotation(float rotate) {
		this->armRotate = rotate;
	}
	float getRotation() {
		return this->armRotate;
	}
	void setWRotation(float rotate) {
		this->wristRotate = rotate;
	}
	float getWRotation() {
		return this->wristRotate;
	}
	void setERotation(float rotate) {
		this->elbowRotate = rotate;
	}
	float getERotation() {
		return this->elbowRotate;
	}
	void setFRotation(float rotate) {
		this->fingerRotate = rotate;
	}
	float getFRotation() {
		return this->fingerRotate;
	}
	glm::vec3 getTranslateRandom() { return this->TranslateRandom; }
	void setTranslateRandom(glm::vec3 TranslateRandom) { this->TranslateRandom = TranslateRandom; }
	glm::vec3 getTranslateModel() { return this->TranslateModel; }
	void setTranslateModel(glm::vec3 TranslateModel) { this->TranslateModel = TranslateModel; }
	void resetArm();
    std::string name;

	vec3 initialPosition;
	vec3 TranslateModel;
	vec3 TranslateRandom;
private:

    GLuint lowerArmVAO;
    GLuint upperArmVAO;
 

   
	float armRotate, elbowRotate, fingerRotate, wristRotate;

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
    //explicit EvanArm(vec3 initialPos, vec3 scale, GLuint lowerArmVAO, GLuint upperArmVAO, EvanRacket racket);


};