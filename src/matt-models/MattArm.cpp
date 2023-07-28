#include "MattArm.h"

MattArm::MattArm()
{
	groupMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));
	worldMatrix = glm::mat4(1.0f);
	initialScale = glm::vec3(1.0f);
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
	glUseProgram(shaderProgram);

	glm::mat4 modelMat = worldMatrix * groupMatrix;

	modelMat = glm::scale(modelMat, initialScale);
	modelMat = glm::translate(modelMat, glm::vec3(2.0f, 3.0f, 0.0f));
	modelMat = glm::scale(modelMat, glm::vec3(1.0f, 4.0f, 1.0f));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(0.8f, 0.65f, 0.37f)));
	glBindVertexArray(unitCubeVAO);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void MattArm::drawLowerArm()
{
	glUseProgram(shaderProgram);

	glm::mat4 modelMat = worldMatrix * groupMatrix;

	modelMat = glm::scale(modelMat, initialScale);
	modelMat = glm::rotate(modelMat, glm::radians(60.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	modelMat = glm::scale(modelMat, glm::vec3(1.0f, 4.0f, 1.0f));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(0.8f, 0.65f, 0.37f)));
	glBindVertexArray(unitCubeVAO);
	
	glDrawArrays(GL_TRIANGLES, 0, 36);
}



