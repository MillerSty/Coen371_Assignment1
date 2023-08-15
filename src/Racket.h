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
    // CONSTRUCTOR
	Racket(GLuint cubeVao, std::string racketName);

    // GETTERS/ SETTERS
    void SetAttr(glm::mat4 groupMatrix, int renderAs, int shaderProgram, glm::mat4 partParent);

    // PARAMETERS
	int racketAngle;
	float jawnAngle;
    int shaderProgram;
    int renderAs;
	std::string racketName;

	GLuint cubeVao;
	glm::mat4 rotationW;
	glm::mat4 partParent;
	glm::mat4 partChild;
	glm::mat4 partMatrix;
	glm::mat4 groupMatrix;
	glm::vec3 position;

	SceneObjects ball;
	Material plasticMaterial;

	// METHODS
	bool Draw();
};
