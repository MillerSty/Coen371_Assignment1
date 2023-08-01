#include "MattRacket.h"
#include <iostream>

MattRacket::MattRacket()
{
	worldMatrix = glm::mat4(1.0f);
	initialScale = glm::vec3(0.2f);
}

void MattRacket::setRacketColor(glm::vec3 color) { racketColor = color; }

glm::vec3 MattRacket::getRacketColor() { return racketColor; }

void MattRacket::setGroupMatrix(glm::mat4 groupMat) { groupMatrix = groupMat; }

glm::mat4 MattRacket::getGroupMatrix() { return groupMatrix; }

void MattRacket::setWorldMatrix(glm::mat4 worldMat) { worldMatrix = worldMat; }

glm::mat4 MattRacket::getWorldMatrix() { return worldMatrix; }

void MattRacket::setShaderProgram(GLuint shaderProg) { shaderProgram = shaderProg; }

void MattRacket::setInitialPosition(glm::vec3 postion) { initialPosition = postion; }

glm::vec3 MattRacket::getInitialPosition() { return initialPosition; }

void MattRacket::setInitialScale(glm::vec3 scale) { initialScale = scale; }

glm::vec3 MattRacket::getInitialScale() { return initialScale; }

GLuint MattRacket::getShaderProgram() { return shaderProgram; }

void MattRacket::setTexture(GLuint tex) { texture = tex; }

GLuint MattRacket::getTexture() { return texture; }

void MattRacket::setVAO(int vao) { unitCubeVAO = vao; }

int MattRacket::getVAO() { return unitCubeVAO; }

void MattRacket::drawRacket()
{
	drawHandle();
	drawPaddle();
}

void MattRacket::drawHandle()
{
	glm::mat4 modelMat = worldMatrix * groupMatrix;

	glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 1.3f, 0.1f));
	glm::mat4 modelRotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.335f, 0.30f, -0.2f));

	modelMat *= modelTranslate * modelRotate * modelScale;

	glBindVertexArray(unitCubeVAO);
	// glBindTexture(GL_TEXTURE_2D, texture);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);

	// Set to brown
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(0.42f, 0.3f, 0.0f)));

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}

void MattRacket::drawPaddle()
{
	glm::mat4 modelMat = worldMatrix * groupMatrix;

	glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.8f, 0.8f));
	glm::mat4 modelRotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.335f, 0.40f, -0.2f));

	modelMat *= modelTranslate * modelRotate * modelScale;

	glBindVertexArray(unitCubeVAO);
	// glBindTexture(GL_TEXTURE_2D, texture);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);

	// Set to brown
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(0.42f, 0.3f, 0.0f)));

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}
