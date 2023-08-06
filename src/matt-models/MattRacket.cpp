#include "MattRacket.h"
#include <iostream>

MattRacket::MattRacket()
{
	worldMatrix = glm::mat4(1.0f);
	initialScale = glm::vec3(0.2f);
	renderAs = GL_TRIANGLES;
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

void MattRacket::setRenderAs(int as) { renderAs = as; }

int MattRacket::getRenderAs() { return renderAs; }

void MattRacket::drawRacket(glm::vec3 TranslateModel, float rotation)
{
	drawHandle(TranslateModel, rotation);
	drawPaddle(TranslateModel, rotation);
}

void MattRacket::drawHandle(glm::vec3 TranslateModel, float rotation)
{
	glm::mat4 modelMat = worldMatrix * groupMatrix;

	glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 1.3f, 0.1f));
	glm::mat4 modelRotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(TranslateModel.x+ -0.335f, TranslateModel.y + 0.30f, TranslateModel.z + -0.2f));

	modelMat *= modelTranslate * modelRotate * modelScale;

	glBindVertexArray(unitCubeVAO);
	// glBindTexture(GL_TEXTURE_2D, texture);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);

	// Set to brown
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(0.42f, 0.3f, 0.0f)));

	glDrawArrays(renderAs, 0, 36);

	glBindVertexArray(0);
}

void MattRacket::drawPaddle(glm::vec3 TranslateModel, float rotation)
{
	glm::mat4 modelMat = worldMatrix * groupMatrix;

	glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.8f, 0.8f));
	glm::mat4 modelRotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(TranslateModel.x + -0.335f, TranslateModel.y + 0.40f, TranslateModel.z + -0.2f));

	modelMat *= modelTranslate * modelRotate * modelScale;

	glBindVertexArray(unitCubeVAO);
	// glBindTexture(GL_TEXTURE_2D, texture);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);

	// Set to brown
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(0.42f, 0.3f, 0.0f)));

	glDrawArrays(renderAs, 0, 36);

	glBindVertexArray(0);
}
