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
	Letters(int cubeVao,
		glm::mat4 worldMatrix);
	Letters(int cubeVao, std::string letterName);

	int cubeVao;
	glm::mat4 partParent;
	glm::mat4 partChild;
	glm::mat4 partMatrix;
	glm::mat4 groupMatrix;

	int shaderProgram;
	int renderAs;
	glm::vec3 position;
	glm::vec4 colour[3];
	std::string letterName;
	Material J;
	Material I;
	Material M;
	Material L;

	//methods like 
	bool DrawJ();
	bool DrawM();
	bool DrawI();
	bool DrawL();

};

