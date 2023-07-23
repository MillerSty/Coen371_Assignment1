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

class Arm
{
public:
	Arm(int cubeVao,
		glm::mat4 worldMatrix);
	Arm(int cubeVao, std::string letterName);
	void setTranslation(glm::vec3 TranslateRandom, glm::vec3 TranslateModel);
	void SetAttr(glm::mat4 groupMatrix, int renderAs, int shaderProgram);
	void setRotation(float rotate) {
		this->armRotate = rotate;
	}
	float getRotation() {
		return this->armRotate;
	}

	int cubeVao;
	glm::mat4 partParent;
	glm::mat4 partMatrix;
	glm::mat4 groupMatrix;

	glm::vec3 TranslateRandom;

	glm::vec3 TranslateModel;
	float armRotate,elbowRotate,wristRotate;

	int shaderProgram;
	int renderAs;
	glm::vec3 position;
	glm::vec3 colour[3];
	std::string letterName;


	//methods like 
	bool DrawArm();

};



