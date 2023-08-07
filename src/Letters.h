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

class Letters
{
public:
	Letters() = default;
	Letters(int cubeVao,glm::mat4 worldMatrix);
	Letters(int cubeVao, std::string letterName);
	int cubeVao;
	glm::mat4 partParent;
	glm::mat4 partMatrix;
	glm::mat4 groupMatrix;
	int shaderProgram;
	int renderAs;
	glm::vec3 position;
	glm::vec3 colour[3];
	std::string letterName;
	//methods like 
	bool DrawNumber(int number);
	bool DrawJ();
	bool DrawM();
	bool DrawI();
	bool DrawL();

};

