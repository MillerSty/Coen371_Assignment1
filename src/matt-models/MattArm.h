#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MattRacket.h"

class MattArm
{
private:
	glm::vec3 armColor;
	glm::mat4 groupMatrix;
	glm::mat4 worldMatrix;
	glm::vec3 initialPosition;
	glm::vec3 initialScale;
	GLuint shaderProgram;
	int unitCubeVAO;
	MattRacket mattRacket;

	void drawUpperArm(glm::vec3 TranslateModel,float rotation=0.0f);
	void drawLowerArm(glm::vec3 TranslateModel,  float rotation = 0.0f);

public:

	//MattArm(MattRacket racket);
	MattArm();
	
	void setArmColor(glm::vec3 color);
	glm::vec3 getArmColor();
	void setGroupMatrix(glm::mat4 groupMat);
	glm::mat4 getGroupMatrix();
	void setWorldMatrix(glm::mat4 worldMat);
	glm::mat4 getWorldMatrix();
	void setShaderProgram(GLuint shaderProg);
	void setInitialPosition(glm::vec3 postion);
	glm::vec3 getInitialPosition();
	void setInitialScale(glm::vec3 scale);
	glm::vec3 getInitialScale();
	GLuint getShaderProgram();
	void setVAO(int vao);
	int getVAO();


	void drawArm(glm::vec3 TranslateModel, float rotation = 0.0f);
};
