#pragma once

#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Material.h"
#include "SceneObjects.h"
class Racket
{

public:
	Racket(int cubeVao, glm::mat4 worldMatrix) ;
	Racket(int cubeVao, std::string racketName);
	void SetAttr(glm::mat4 groupMatrix, int renderAs, int shaderProgram, glm::mat4 partParent);
	void setPlastic(Material plastic) { plasticMaterial = plastic; }
	void setBall(SceneObjects ball) { this->ball = ball; }


	int cubeVao;
	int racketAngle;
	float jawnAngle;

	glm::mat4 rotationW;
	glm::mat4 partParent;
	glm::mat4 partChild;
	glm::mat4 partMatrix;
	glm::mat4 groupMatrix;

	int shaderProgram;
	int renderAs;
	glm::vec3 position;
	std::string racketName;
	SceneObjects ball;

	Material plasticMaterial;

	//methods like 
	bool Draw();
};

