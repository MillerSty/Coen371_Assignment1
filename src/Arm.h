#pragma once

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Material.h"

class Arm
{
public:
	Arm(int cubeVao, glm::mat4 worldMatrix);
	Arm(int cubeVao, std::string letterName);
	Arm();

	void setTranslation(glm::vec3 TranslateRandom, glm::vec3 TranslateModel);
	void SetAttr(glm::mat4 groupMatrix, int renderAs, int shaderProgram);
	void setVAO(int vao);
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
	bool flexFingers();
	void InitArm(glm::vec3 position, int VAO, Material skinMaterial, Material clothMaterial);

	bool reverse;
	float spin;
	int cubeVao;
	glm::mat4 partParent;
	glm::mat4 partMatrix;
	glm::mat4 groupMatrix;

	glm::vec3 TranslateRandom;

	glm::vec3 TranslateModel;
	float armRotate,elbowRotate,wristRotate, fingerRotate;
	Material skinMaterial;
	Material clothMaterial;

	int shaderProgram;
	int renderAs;
	glm::vec3 position;
	std::string letterName;


	//methods like 
	bool DrawArm();

};



