#include "MattModel.h"

MattModel::MattModel()
{
	worldMatrix = glm::mat4(1.0f);
	initialScale = glm::vec3(0.2f);
	translationModel = glm::vec3(0.0f);
	translationRandom = glm::vec3(0.0f);
}

void MattModel::setGroupMatrix(glm::mat4 groupMat) { groupMatrix = groupMat; }

glm::mat4 MattModel::getGroupMatrix() { return groupMatrix; }

void MattModel::setWorldMatrix(glm::mat4 worldMat) { worldMatrix = worldMat; }

glm::mat4 MattModel::getWorldMatrix() { return worldMatrix; }

void MattModel::setShaderProgram(GLuint shaderProg) { shaderProgram = shaderProg; }

GLuint MattModel::getShaderProgram() { return shaderProgram; }

void MattModel::setInitialPosition(glm::vec3 postion) { initialPosition = postion; }

glm::vec3 MattModel::getInitialPosition() { return initialPosition; }

void MattModel::setInitialScale(glm::vec3 scale) { initialScale = scale; }

glm::vec3 MattModel::getInitialScale() { return initialScale; }

void MattModel::setVAO(GLuint cubeVao, GLuint sphereVao) { unitCubeVAO = cubeVao; sphereVAO = sphereVao; }

GLuint MattModel::getCubeVAO() { return unitCubeVAO; }

GLuint MattModel::getSphereVAO() { return sphereVAO; }

glm::vec3 MattModel::getTranslationRandom() { return translationRandom; }

void MattModel::setTranslationRandom(glm::vec3 trans) { translationRandom = trans; }

glm::vec3 MattModel::getTranslationModel() { return translationModel; }

void MattModel::setTranslationModel(glm::vec3 trans) { translationModel = trans; }

void MattModel::setRotationAngle(float angle) { rotationAngle = angle; }

float MattModel::getRotationAngle() { return rotationAngle; }

void MattModel::setRenderAs(int as) { renderAs = as; }

int MattModel::getRenderAs() { return renderAs; }

void MattModel::setSphereVertCount(int count) { sphereVertCount = count; }

int MattModel::getSphereVertCount() { return sphereVertCount; }

void MattModel::setMaterials(Material racket, Material arm, Material ball)
{
	racketTexture = racket;
	armTexture = arm;
	ballTexture = ball;
}

void MattModel::drawModel()
{
	// The total translation this frame is the translation of the model, plus any random translation from SPACE
	glm::vec3 translation = translationModel + translationRandom;

	// Bind cube VAO
	glBindVertexArray(unitCubeVAO);

	// DRAW ARM
	const glm::vec3 ARM_COLOR(0.8f, 0.65f, 0.37f);  // Set arm color
	armTexture.bindTexture();  // Activate texture
	armTexture.loadToShader();

	// Draw forearm
	glm::mat4 modelMat = worldMatrix * groupMatrix;

	glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.8f, 0.2f));
	glm::mat4 modelRotate = glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x - 0.35f, translation.y + 0.2f, translation.z - 0.2f));

	// Create combined MVP matrix from scaling, rotation, and translation
	modelMat *= modelTranslate * modelRotate * modelScale;

	// Send to shader
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);

	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, &ARM_COLOR[0]);

	glDrawArrays(renderAs, 0, 36); // Draw

	// Draw bicep
	modelMat = worldMatrix * groupMatrix;

	modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.8f, 0.2f));
	modelRotate = glm::rotate(glm::mat4(1.0f), glm::radians(60.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x - 0.4f, translation.y + 0.15f, translation.z - 0.2f));
	
	modelMat *= modelTranslate * modelRotate * modelScale;

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);

	glDrawArrays(renderAs, 0, 36);

	// DRAW RACKET
	const glm::vec3 RACKET_COLOR(0.42f, 0.3f, 0.0f);
	racketTexture.bindTexture();
	racketTexture.loadToShader();

	// Draw handle
	modelMat = worldMatrix * groupMatrix;

	modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 1.3f, 0.1f));
	modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x - 0.335f, translation.y + 0.3f, translation.z - 0.2f));

	modelMat *= modelTranslate * modelScale;

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);

	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, &RACKET_COLOR[0]);

	glDrawArrays(renderAs, 0, 36);

	// Draw paddle
	modelMat = worldMatrix * groupMatrix;

	modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.8f, 0.8f));
	modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x - 0.335f, translation.y + 0.4f, translation.z - 0.2f));

	modelMat *= modelTranslate * modelScale;

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);

	glDrawArrays(renderAs, 0, 36);

	// Unbind VAO
	glBindVertexArray(0);

	// Draw ball
	
	// Bind sphere VAO
	glBindVertexArray(sphereVAO);
	
	glm::vec3 SPHERE_COLOR(0.6f, 0.23f, 0.004f);

	ballTexture.bindTexture();
	ballTexture.loadToShader();

	modelMat = worldMatrix * groupMatrix;

	modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.45f));
	modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x - 0.2f, translation.y + 0.4f, translation.z - 0.2f));

	modelMat *= modelTranslate * modelScale;

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);

	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, &SPHERE_COLOR[0]);

	glDrawElements(renderAs, sphereVertCount, GL_UNSIGNED_INT, 0);

	// Unbind geometry
	glBindVertexArray(0);
}

void MattModel::resetModel()
{
	translationModel -= translationModel;
	translationRandom -= translationRandom;
}
