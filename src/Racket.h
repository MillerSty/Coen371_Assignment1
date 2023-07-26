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

class Racket
{

public:
	Racket(int cubeVao,
               glm::mat4 worldMatrix) ;
	Racket(int cubeVao, std::string racketName);
	void SetAttr(glm::mat4 groupMatrix, int renderAs, int shaderProgram, glm::mat4 partParent);
	int cubeVao;
	int racketAngle;
	float jawnAngle;
	glm::mat4 rotationW;
	glm::mat4 partParent;
	glm::mat4 partMatrix;
	glm::mat4 groupMatrix;
	int shaderProgram;
	int renderAs;
	glm::vec3 position;
	std::string racketName;

	//methods like 
	bool Draw();
};

