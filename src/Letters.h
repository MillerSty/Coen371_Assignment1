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
    // CONSTRUCTORS
	Letters() = default;
	Letters(GLuint cubeVao, int shaderProgram, glm::mat4 worldMatrix, glm::vec3 numPosition, Material plastic, int renderAs);

    // PARAMETERS
	GLuint cubeVao;

	glm::mat4 partParent;
	glm::mat4 partMatrix;
	glm::mat4 groupMatrix;
	glm::mat4 boardChild;
	glm::mat4 boardParent;

	glm::vec3 position;
	glm::vec3 numPosition;
	glm::vec3 colour[3];

	Material plastic;

	int shaderProgram;
	int renderAs;

	// METHODS
	bool DrawNumber(int number);
	bool Scoreboard(int number, bool isClock, bool isBlue);
};
