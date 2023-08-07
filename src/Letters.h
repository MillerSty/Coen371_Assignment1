#pragma once

#include <cstdlib>
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

class Letters
{
public:
	Letters() = default;
	Letters(int cubeVao,glm::mat4 worldMatrix);
	Letters(int cubeVao,int shaderProgram, glm::mat4 worldMatrix,glm::vec3 numPosition,Material plastic,int renderAs);

	int cubeVao;
	glm::mat4 partParent;
	glm::mat4 partMatrix;
	glm::mat4 groupMatrix;

	glm::mat4 boardChild;
	glm::mat4 boardParent;


	Material plastic;
	int shaderProgram;
	int renderAs;
	glm::vec3 position;
	glm::vec3 numPosition;
	glm::vec3 colour[3];
	//methods like 
	bool DrawNumber(int number);
	bool Scoreboard(int number,bool isClock,bool isBlue);
	bool DrawJ();
	bool DrawM();
	bool DrawI();
	bool DrawL();

};

