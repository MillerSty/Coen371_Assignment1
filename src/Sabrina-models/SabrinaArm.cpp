#include "SabrinaArm.h"

SabrinaArm::SabrinaArm()
{
	worldMatrix = glm::mat4(1.0f);
	initialScale = glm::vec3(0.2f);
	renderAs = GL_TRIANGLES;
	//mattRacket = racket;
}

void SabrinaArm::setArmColor(glm::vec3 color) { armColor = color; }

glm::vec3 SabrinaArm::getArmColor() { return armColor; }

void SabrinaArm::setGroupMatrix(glm::mat4 groupMat) { groupMatrix = groupMat; }

glm::mat4 SabrinaArm::getGroupMatrix() { return groupMatrix; }

void SabrinaArm::setWorldMatrix(glm::mat4 worldMat) { worldMatrix = worldMat; }

glm::mat4 SabrinaArm::getWorldMatrix() { return worldMatrix; }

void SabrinaArm::setShaderProgram(GLuint shaderProg) { shaderProgram = shaderProg; }

void SabrinaArm::setInitialPosition(glm::vec3 postion) { initialPosition = postion; }

glm::vec3 SabrinaArm::getInitialPosition() { return initialPosition; }

void SabrinaArm::setInitialScale(glm::vec3 scale) { initialScale = scale; }

glm::vec3 SabrinaArm::getInitialScale() { return initialScale; }

GLuint SabrinaArm::getShaderProgram() { return shaderProgram; }

void SabrinaArm::setVAO(int vao) { unitCubeVAO = vao; }

int SabrinaArm::getVAO() { return unitCubeVAO; }

void SabrinaArm::setRenderAs(int as) { renderAs = as; }

int SabrinaArm::getRenderAs() { return renderAs; }

void SabrinaArm::drawArm()
{
	drawUpperArm();
	drawLowerArm();
}

void SabrinaArm::drawUpperArm()//forearm
{
	//glm::mat4 modelMat = worldMatrix * groupMatrix;

	//glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.8f, 0.2f));
	//glm::mat4 modelRotate = glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	//glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(TranslateModel.x + -0.35f, TranslateModel.y + 0.20f, TranslateModel.z + -0.2f));

	//modelMat *= modelTranslate * modelRotate * modelScale;


	glm::mat4 upperArm = worldMatrix * groupMatrix;
	glm::mat4 upperArmTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.35f, 0.2f, -0.2f));
	glm::mat4 upperArmsSale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.8f, 0.2f));
	glm::mat4 upperArmTransform = upperArmTranslate * upperArmsSale;
	upperArm *= upperArmTransform;

	glBindVertexArray(unitCubeVAO);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &upperArm[0][0]);

	// Set to beige
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

	glDrawArrays(renderAs, 0, 36);
	glBindVertexArray(0);

}

void SabrinaArm::drawLowerArm()//bicep
{
	//glm::mat4 modelMat = worldMatrix * groupMatrix;

	//glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.8f, 0.2f));
	//glm::mat4 modelRotate = glm::rotate(glm::mat4(1.0f), glm::radians(60.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	//glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(TranslateModel.x + -0.4f, TranslateModel.y + 0.15f, TranslateModel.z + -0.2f));

	//modelMat *= modelTranslate * modelRotate * modelScale;

	glm::mat4 lowerArm = worldMatrix * groupMatrix;
	glm::mat4 lowerArmTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.38f, 0.15f, -0.2f));
	glm::mat4 lowerArmsSale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.8f, 0.2f));
	glm::mat4 lowerArmRotate = glm::rotate(glm::mat4(1.0f), glm::radians(-60.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	glm::mat4 lowerArmTransform = lowerArmTranslate * lowerArmRotate * lowerArmsSale;
	lowerArm *= lowerArmTransform;

	glBindVertexArray(unitCubeVAO);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &lowerArm[0][0]);
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(0.8f, 0.65f, 0.37f)));

	glDrawArrays(renderAs, 0, 36);

	glBindVertexArray(0);

	// Draw racket attached to arm
	//mattRacket.drawRacket();
}