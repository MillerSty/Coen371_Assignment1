#include "MattArm.h"

MattArm::MattArm()
{
	//groupMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 0.0f, 0.0f));
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
	//drawUpperArm();
	drawLowerArm();

	//glm::mat4 worldMatrix;
	//glm::mat4 partScaleMatrix;
	//glm::mat4 partTranslateMatrix;
	//glm::mat4 partRotateMatrix;
	//glm::mat4 partMatrix;

	//glUseProgram(shaderProgram);

	//glm::mat4 upperArmParent;
	//glm::mat4 upperArmTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 1.0f, -5.0f));
	//glm::mat4 upperArmRotate = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//upperArmParent = upperArmTranslate * upperArmRotate;

	//glm::vec3 armScaler = glm::vec3(0.575f, 0.25f, 0.15f);

	//partRotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//partScaleMatrix = glm::scale(glm::mat4(1.0f), armScaler);

	//partMatrix = partScaleMatrix * partRotateMatrix;

	//worldMatrix = groupMatrix * upperArmParent * partMatrix;

	//glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &worldMatrix[0][0]);
	//glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(0.8f, 0.65f, 0.37f)));
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

//void MattArm::drawUpperArm()
//{
//	glUseProgram(shaderProgram);
//
//	glm::mat4 modelMat = worldMatrix * groupMatrix;
//
//	modelMat = glm::scale(modelMat, initialScale);
//	modelMat = glm::translate(modelMat, glm::vec3(2.0f, 10.0f, 0.0f));
//	modelMat = glm::scale(modelMat, glm::vec3(1.0f, 4.0f, 1.0f));
//
//	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
//	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(0.8f, 0.65f, 0.37f)));
//	glBindVertexArray(unitCubeVAO);
//
//	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//	//glDrawArrays(GL_TRIANGLES, 0, 36);
//}
//
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
	
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
}



