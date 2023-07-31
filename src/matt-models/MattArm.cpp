#include "MattArm.h"

MattArm::MattArm()
{
	worldMatrix = glm::mat4(1.0f);
	initialScale = glm::vec3(0.2f);
}

void MattArm::setArmColor(glm::vec3 color) { armColor = color; }

glm::vec3 MattArm::getArmColor() { return armColor; }

void MattArm::setGroupMatrix(glm::mat4 groupMat) { groupMatrix = groupMat; }

glm::mat4 MattArm::getGroupMatrix() { return groupMatrix; }

void MattArm::setWorldMatrix(glm::mat4 worldMat) { worldMatrix = worldMat; }

glm::mat4 MattArm::getWorldMatrix() { return worldMatrix; }

void MattArm::setShaderProgram(GLuint shaderProg) { shaderProgram = shaderProg; }

void MattArm::setInitialPosition(glm::vec3 postion) { initialPosition = postion; }

glm::vec3 MattArm::getInitialPosition() { return initialPosition; }

void MattArm::setInitialScale(glm::vec3 scale) { initialScale = scale; }

glm::vec3 MattArm::getInitialScale() { return initialScale; }

GLuint MattArm::getShaderProgram() { return shaderProgram; }

void MattArm::setVAO(int vao) { unitCubeVAO = vao; }

int MattArm::getVAO() { return unitCubeVAO; }

void MattArm::drawArm()
{
	drawUpperArm();
	drawLowerArm();
}

void MattArm::drawUpperArm()
{
	glm::mat4 modelMat = worldMatrix * groupMatrix;

	glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.8f, 0.2f));
	glm::mat4 modelRotate = glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.35f, 0.20f, -0.2f));

	modelMat *= modelTranslate * modelRotate * modelScale;

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(0.8f, 0.65f, 0.37f)));
	glBindVertexArray(unitCubeVAO);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}

void MattArm::drawLowerArm()
{
	glm::mat4 modelMat = worldMatrix * groupMatrix;

	glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.8f, 0.2f));
	glm::mat4 modelRotate = glm::rotate(glm::mat4(1.0f), glm::radians(60.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.4f, 0.15f, -0.2f));

	modelMat *= modelTranslate * modelRotate * modelScale;

	glBindVertexArray(unitCubeVAO);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(0.8f, 0.65f, 0.37f)));
	
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}
