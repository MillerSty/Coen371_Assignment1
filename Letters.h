#pragma once
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs
#include <glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <gtc/type_ptr.hpp>
class Letters
{
public:
	Letters(int cubeVao,
		glm::mat4 worldMatrix);
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
	bool DrawJ();
	bool DrawM();
	bool DrawI();
	bool DrawL();

};

