#include "CrowdObjects.h"
#define cubeVao vaos[0]
#define sphereVao vaos[1]
#define colour glm::vec3(1,1,1)
void CrowdObjects::drawCrowd() {
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	GLuint applyTexturesLocation = glGetUniformLocation(shaderProgram, "shouldApplyTexture");


	skinMaterial.loadToShader();
	skinMaterial.bindTexture();

	glm::mat4 worldMatrix;

	glm::mat4 partParent;
	glm::mat4 partTranslate;
	glm::mat4 partScale;
	glm::mat4 partRotate;


	for (float i = -.75; i < .75; i += .3) {
		glm::mat4 crowdParent;
		glm::mat4 crowdTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(i, .25f, -0.666f));
		glm::mat4 crowdScale = glm::scale(glm::mat4(1.0f), glm::vec3(.50f, 1.0f, .50f));
		glm::mat4 crowdRotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		//Note: No part matrices but we may want them eventually
		crowdParent = crowdTranslate * crowdScale;
		worldMatrix = groupMatrix * crowdParent;

		glBindVertexArray(sphereVao);
		glUniform3fv(colorLocation, 1, glm::value_ptr(colour));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		//glDrawArrays(renderAs, 0, 36);
		glDrawElements(renderAs, sphereIndexCount, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		//arms
		glBindVertexArray(cubeVao);
		//left (facing +z ) arm	
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.03f, .0f, 0.0f));
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.125f, .550f, .125f));
		partRotate = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0f, .0f, .0f));
		partParent = crowdTranslate * partTranslate * partRotate * partScale;
		worldMatrix = groupMatrix * partParent;

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(renderAs, 0, 36);
		//right (facing +z ) arm
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.03f, .0f, 0.0f));
		partParent = crowdTranslate * partTranslate * partRotate * partScale;
		worldMatrix = groupMatrix * partParent;

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(renderAs, 0, 36);
		glBindVertexArray(0);
	}

	for (float i = -.75; i < .75; i += .3) {
		glm::mat4 crowdParent;
		glm::mat4 crowdTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(i, .2f, -0.566f));
		glm::mat4 crowdScale = glm::scale(glm::mat4(1.0f), glm::vec3(.50f, 1.0f, .50f));
		glm::mat4 crowdRotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		//Note: No part matrices but we may want them eventually
		crowdParent = crowdTranslate * crowdScale;
		worldMatrix = groupMatrix * crowdParent;

		glBindVertexArray(sphereVao);
		glUniform3fv(colorLocation, 1, glm::value_ptr(colour));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		//glDrawArrays(renderAs, 0, 36);
		glDrawElements(renderAs, sphereIndexCount, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		//arms
		glBindVertexArray(cubeVao);
		//left (facing +z ) arm	
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.03f, .0f, 0.0f));
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.125f, .550f, .125f));
		partRotate = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(-1.0f, .0f, .0f));
		partParent = crowdTranslate * partTranslate * partRotate * partScale;
		worldMatrix = groupMatrix * partParent;

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(renderAs, 0, 36);
		//right (facing +z ) arm
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.03f, .0f, 0.0f));
		partParent = crowdTranslate * partTranslate * partRotate * partScale;
		worldMatrix = groupMatrix * partParent;

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(renderAs, 0, 36);
		glBindVertexArray(0);
	}
	








}

void CrowdObjects::initializeCrowdRows(int rows)
{
	if (!initialized)
	{
		col = new glm::vec3[15*rows];
		std::cout << 15 * rows << std::endl;
		initialized = true;
	}
}

void CrowdObjects::test(int crowdRows) {
	float scale = 1;
	std::vector<glm::vec3> crowd;
	rows = crowdRows;
	initializeCrowdRows(rows);

	std::vector<std::vector<glm::vec3>> crowdy;
	for (int rows = 0; rows < crowdRows; rows++) {
		switch (rows) {
			case(0): scale = 1; break;
			case(1):scale = .85f; break;
			case(2):scale = .8f; break;
			case(3):scale = .75f; break;
			default: break;
			}

		for (float i = -.75; i < .75; i += .1) { //does one row
			crowd.push_back(glm::vec3(i, .25*scale, -.7*scale));
		}
		//crowdy.push_back(crowd);
	}
	for (int i = 0; i < crowd.size(); i++) {
		drawSingle(crowd[i],glm::vec3(0), crowd.size(), i);
	}


}

void CrowdObjects::drawSingle(glm::vec3 position,glm::vec3 armrotate, int crowdSize, int crowdPosition) {
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	GLuint applyTexturesLocation = glGetUniformLocation(shaderProgram, "shouldApplyTexture");


	skinMaterial.loadToShader();
	skinMaterial.bindTexture();

	glm::mat4 worldMatrix;

	glm::mat4 partParent;
	glm::mat4 partTranslate;
	glm::mat4 partScale;
	glm::mat4 partRotate;

	if (crowdCount <= crowdSize) {
		std::cout << crowdPosition << std::endl;
		col[crowdPosition] = glm::vec3(glm::linearRand(0.0f, 1.0f),
			glm::linearRand(0.0f, 1.0f),
			glm::linearRand(0.0f, 1.0f));
		crowdCount++;
	}
	/*glm::vec3 col = glm::vec3(glm::linearRand(0.0f, 1.0f),
			glm::linearRand(0.0f, 1.0f),
			glm::linearRand(0.0f, 1.0f));*/

		glm::mat4 crowdParent;
		glm::mat4 crowdTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(position));
		glm::mat4 crowdScale = glm::scale(glm::mat4(1.0f), glm::vec3(.50f, 1.0f, .50f));
		glm::mat4 crowdRotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		//Note: No part matrices but we may want them eventually
		crowdParent = crowdTranslate * crowdScale;
		worldMatrix = groupMatrix * crowdParent;

		glBindVertexArray(sphereVao);
		glUniform3fv(colorLocation, 1, glm::value_ptr(col[crowdPosition]));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		//glDrawArrays(renderAs, 0, 36);
		glDrawElements(renderAs, sphereIndexCount, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		//arms
		glBindVertexArray(cubeVao);
		//left (facing +z ) arm	
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.03f, .0f, 0.0f));
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.125f, .550f, .125f));
		partRotate = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0f, .0f, .0f));
		partParent = crowdTranslate * partTranslate * partRotate * partScale;
		worldMatrix = groupMatrix * partParent;

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(renderAs, 0, 36);
		//right (facing +z ) arm
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.03f, .0f, 0.0f));
		partParent = crowdTranslate * partTranslate * partRotate * partScale;
		worldMatrix = groupMatrix * partParent;

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(renderAs, 0, 36);
		glBindVertexArray(0);
	


}