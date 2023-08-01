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

class Material
{
public:
	Material() = default;
	Material(float diff, float spec, float ambient, float shiny, GLuint texture, int shader);
	float diffuseStrength;
	float specularStrength;
	float ambientStrength;
	float shininessStrength;

	GLuint textureId;
	GLuint shaderProgram;
	void loadToShader();
	void bindTexture();
	void resetShader();
private:

};

