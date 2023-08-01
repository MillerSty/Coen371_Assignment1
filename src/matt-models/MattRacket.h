#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MattRacket
{
private:
	glm::vec3 racketColor;
	glm::mat4 groupMatrix;
	glm::mat4 worldMatrix;
	glm::vec3 initialPosition;
	glm::vec3 initialScale;
	GLuint shaderProgram;
	GLuint texture;
	int unitCubeVAO;

	void drawHandle();
	void drawPaddle();

public:
	MattRacket();

	void setRacketColor(glm::vec3 color);
	glm::vec3 getRacketColor();
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
	void setTexture(GLuint tex);
	GLuint getTexture();
	void setVAO(int vao);
	int getVAO();

	void drawRacket();
};
