#include "MattModel.h"

MattModel::MattModel(int num) : modelNum(num)
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

void MattModel::setVAO(GLuint vao) { unitCubeVAO = vao; }

GLuint MattModel::getVAO() { return unitCubeVAO; }

void MattModel::setModelNum(int num) { modelNum = num; }

int MattModel::getModelNum() { return modelNum; }

glm::vec3 MattModel::getTranslationRandom() { return translationRandom; }

void MattModel::setTranslationRandom(glm::vec3 trans) { translationRandom = trans; }

glm::vec3 MattModel::getTranslationModel() { return translationModel; }

void MattModel::setTranslationModel(glm::vec3 trans) { translationModel = trans; }

void MattModel::setRotationAngle(float angle) { rotationAngle = angle; }

float MattModel::getRotationAngle() { return rotationAngle; }

void MattModel::drawModel()
{
	glm::vec3 translation = translationModel + translationRandom;

	glBindVertexArray(unitCubeVAO);
	
	// DRAW ARM
	const glm::vec3 ARM_COLOR(0.8f, 0.65f, 0.37f);

	// Draw forearm
	glm::mat4 modelMat = worldMatrix * groupMatrix;

	glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.8f, 0.2f));
	glm::mat4 modelRotate;
	glm::mat4 modelTranslate;

	if (modelNum == 1)
	{
		modelRotate = glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x - 0.35f, translation.y + 0.20f, translation.z));
	}
	else if (modelNum == 2)
	{
		modelRotate = glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x + 0.35f, translation.y + 0.20f, translation.z));
	}

	modelMat *= modelTranslate * modelRotate * modelScale;

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);

	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, &ARM_COLOR[0]);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Draw bicep
	modelMat = worldMatrix * groupMatrix;

	modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.8f, 0.2f));

	if (modelNum == 1)
	{
		modelRotate = glm::rotate(glm::mat4(1.0f), glm::radians(60.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x - 0.4f, translation.y + 0.15f, translation.z));
	}
	else if (modelNum == 2)
	{
		modelRotate = glm::rotate(glm::mat4(1.0f), glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x + 0.4f, translation.y + 0.15f, translation.z));
	}

	modelMat *= modelTranslate * modelRotate * modelScale;

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	// DRAW RACKET
	const glm::vec3 RACKET_COLOR(0.42f, 0.3f, 0.0f);

	// Draw handle
	modelMat = worldMatrix * groupMatrix;

	modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 1.3f, 0.1f));

	if (modelNum == 1)
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x - 0.335f, translation.y + 0.30f, translation.z));
	
	else if (modelNum == 2)
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x + 0.335f, translation.y + 0.30f, translation.z));

	modelMat *= modelTranslate * modelScale;

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);

	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, &RACKET_COLOR[0]);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Draw paddle
	modelMat = worldMatrix * groupMatrix;

	modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.8f, 0.8f));

	if (modelNum == 1)
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x - 0.335f, translation.y + 0.40f, translation.z));
	
	else if (modelNum == 2)
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x + 0.335f, translation.y + 0.40f, translation.z));

	modelMat *= modelTranslate * modelScale;

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	// DRAW LETTERS

	if (modelNum == 1)
	{
		// Draw E
		const glm::vec3 E_COLOR(1.0f, 0.0f, 0.0f);
		
		// Left stick
		modelMat = worldMatrix * groupMatrix;

		modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.8f, 0.1f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x - 0.335f, translation.y + 0.50f, translation.z + 0.035f));

		modelMat *= modelTranslate * modelScale;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
		glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, &E_COLOR[0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Bottom stick
		modelMat = worldMatrix * groupMatrix;

		modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.8f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x - 0.335f, translation.y + 0.46f, translation.z));

		modelMat *= modelTranslate * modelScale;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Middle stick
		modelMat = worldMatrix * groupMatrix;

		modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.8f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x - 0.335f, translation.y + 0.5f, translation.z));

		modelMat *= modelTranslate * modelScale;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Top stick
		modelMat = worldMatrix * groupMatrix;

		modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.8f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x - 0.335f, translation.y + 0.54f, translation.z));

		modelMat *= modelTranslate * modelScale;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// Draw L
		const glm::vec3 L_COLOR(0.0f, 1.0f, 0.0f);

		// Left stick
		modelMat = worldMatrix * groupMatrix;

		modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.8f, 0.1f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x - 0.335f, translation.y + 0.60f, translation.z + 0.035f));

		modelMat *= modelTranslate * modelScale;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
		glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, &L_COLOR[0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Bottom stick
		modelMat = worldMatrix * groupMatrix;

		modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.8f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x - 0.335f, translation.y + 0.56f, translation.z));

		modelMat *= modelTranslate * modelScale;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	else if (modelNum == 2)
	{
		// Draw A
		const glm::vec3 A_COLOR(0.0f, 0.0f, 1.0f);

		// Left stick
		modelMat = worldMatrix * groupMatrix;

		modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.8f, 0.1f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x + 0.335f, translation.y + 0.50f, translation.z + 0.035f));

		modelMat *= modelTranslate * modelScale;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
		glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, &A_COLOR[0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Right stick
		modelMat = worldMatrix * groupMatrix;

		modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.8f, 0.1f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x + 0.335f, translation.y + 0.50f, translation.z - 0.035f));

		modelMat *= modelTranslate * modelScale;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Middle stick
		modelMat = worldMatrix * groupMatrix;

		modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.8f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x + 0.335f, translation.y + 0.5f, translation.z));

		modelMat *= modelTranslate * modelScale;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Top stick
		modelMat = worldMatrix * groupMatrix;

		modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.8f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x + 0.335f, translation.y + 0.54f, translation.z));

		modelMat *= modelTranslate * modelScale;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Draw S
		const glm::vec3 S_COLOR(1.0f, 1.0f, 0.0f);

		// Left-up stick
		modelMat = worldMatrix * groupMatrix;

		modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.4f, 0.1f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x + 0.335f, translation.y + 0.62f, translation.z + 0.035f));

		modelMat *= modelTranslate * modelScale;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
		glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, &S_COLOR[0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Left bottom stick
		modelMat = worldMatrix * groupMatrix;

		modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.4f, 0.1f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x + 0.335f, translation.y + 0.58f, translation.z - 0.035f));

		modelMat *= modelTranslate * modelScale;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
		glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, &S_COLOR[0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Bottom stick
		modelMat = worldMatrix * groupMatrix;

		modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.8f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x + 0.335f, translation.y + 0.56f, translation.z));

		modelMat *= modelTranslate * modelScale;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Middle stick
		modelMat = worldMatrix * groupMatrix;

		modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.8f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x + 0.335f, translation.y + 0.60f, translation.z));

		modelMat *= modelTranslate * modelScale;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Top stick
		modelMat = worldMatrix * groupMatrix;

		modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.8f));
		modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x + 0.335f, translation.y + 0.64f, translation.z));

		modelMat *= modelTranslate * modelScale;

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Unbind VAO
	glBindVertexArray(0);
}


