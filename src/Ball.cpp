#include "Ball.h"

Ball::Ball()
{
	worldMatrix = glm::mat4(1.0f);
	translationModel = glm::vec3(0.0f);
}

void Ball::setGroupMatrix(glm::mat4 groupMat) { groupMatrix = groupMat; }

glm::mat4 Ball::getGroupMatrix() { return groupMatrix; }

void Ball::setWorldMatrix(glm::mat4 worldMat) { worldMatrix = worldMat; }

glm::mat4 Ball::getWorldMatrix() { return worldMatrix; }

void Ball::setShaderProgram(GLuint shaderProg) { shaderProgram = shaderProg; }

GLuint Ball::getShaderProgram() { return shaderProgram; }

void Ball::setInitialPosition(glm::vec3 postion) { initialPosition = postion; }

glm::vec3 Ball::getInitialPosition() { return initialPosition; }

void Ball::setVAO(GLuint vao) { VAO = vao; }

GLuint Ball::getVAO() { return VAO; }

glm::vec3 Ball::getTranslationModel() { return translationModel; }

void Ball::setTranslationModel(glm::vec3 trans) { translationModel = trans; }

void Ball::setRenderAs(int as) { renderAS = as; }

int Ball::getRenderAs() { return renderAS; }

void Ball::setSphereVertCount(int count) { sphereVertCount = count; }

int Ball::getSphereVertCount() { return sphereVertCount; }

void Ball::setMaterial(Material material) { ballTexture = material; }

void Ball::resetModel()
{
	translationModel -= translationModel;
}

void Ball::drawBall()
{
	// Bind sphere VAO
	glBindVertexArray(VAO);

	// Set color
	glm::vec3 SPHERE_COLOR(0.0f, 1.0f, 0.0f);
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, &SPHERE_COLOR[0]);

	// Apply texture
	ballTexture.bindTexture();
	ballTexture.loadToShader();

	// Set transformations
	glm::mat4 modelMat = worldMatrix * groupMatrix;

	glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.35f));
	
	// Translate only in the XY plane
	glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translationModel.x, translationModel.y + 0.15f, 0.0f));

	modelMat *= modelTranslate * modelScale;

	// Draw geometry
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
	glDrawElements(renderAS, sphereVertCount, GL_UNSIGNED_INT, 0);

	// Unbind sphere VAO
	glBindVertexArray(0);
}
