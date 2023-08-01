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
	Material();
	Material(float diff, float spec, float ambient, float shiny, GLint texture, GLint shader);
	float diffuseStrength;
	float specularStrength;
	float ambientStrength;
	float shininessStrength;

	GLint textureId;
	GLint shaderProgram;
	void loadToShader();
	void bindTexture();
	void resetShader();
private:

};

