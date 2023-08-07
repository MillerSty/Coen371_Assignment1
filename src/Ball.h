#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Material.h"

class Ball
{
private:
	GLuint VAO;
	GLuint shaderProgram;
	glm::mat4 groupMatrix;
	glm::mat4 worldMatrix;
	glm::vec3 translationModel;
	glm::vec3 initialPosition;
	int renderAS;
	int sphereVertCount;
	Material ballTexture;
	float velocity;

public:
	Ball();

	void setGroupMatrix(glm::mat4 groupMat);
	glm::mat4 getGroupMatrix();
	void setWorldMatrix(glm::mat4 worldMat);
	glm::mat4 getWorldMatrix();
	void setShaderProgram(GLuint shaderProg);
	GLuint getShaderProgram();
	void setInitialPosition(glm::vec3 postion);
	glm::vec3 getInitialPosition();
	void setVAO(GLuint vao);
	GLuint getVAO();
	glm::vec3 getTranslationModel();
	void setTranslationModel(glm::vec3 trans);
	void setRenderAs(int as);
	int getRenderAs();
	void setSphereVertCount(int count);
	int getSphereVertCount();
	void setMaterial(Material material);
	
	void resetModel();
	void drawBall();
};

