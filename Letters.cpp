#include "Letters.h"



Letters::Letters(int cubeVao,
	glm::mat4 worldMatrix) {
	this->cubeVao = cubeVao;
	this->groupMatrix = worldMatrix;
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);

	//colours should be set before while loops if we want 
	//random
//colour[0] = glm::vec3(1.0f, 0.0f, 0.0f);
//colour[1] = glm::vec3(0.0f, 1.0f, 0.0f);
//colour[2] = glm::vec3(0.0f, 0.0f, 1.0f);
}
Letters::Letters(int cubeVao, std::string letterName) {
	this->cubeVao = cubeVao;
	this->groupMatrix;
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	//this->racketAngle = 90;
	//this->rotationW = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	//this->jawnAngle = 0;
	this->letterName = letterName;
}
// Note for all Draw functions they follow the same format, declare matrice locations and mat4 variables. 
// then scale, rotate, translate x3 so we get triple ply effect on letters
bool Letters::DrawJ() {
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	glm::mat4 worldMatrix;
	glm::mat4 letterTranslate;
	glm::mat4 letterRotate;
	glm::mat4 letterScale;
	glm::mat4 LetterGroupMatrix;
	glm::mat4 partScale;
	glm::mat4 partTranslate;
	glm::mat4 partRo;
	glm::mat4 partMatrix;

	glBindVertexArray(cubeVao);

	// J has little notch, long bottom, long Straight,little top 
	letterTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.2f, .0f, .0f));
	letterRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.15f, .15f, .15f));

	glm::mat4 letterParent = this->partParent * letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix =   this->groupMatrix * letterParent;

	//long jawn triple Vert
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(7.0f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	//long triple jawn Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//long jawn triple Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, -.055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

		//long jawn triple Hor
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f, 7.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, .0f, -.35f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	////***
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f, 7.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, .055f, -.35f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	
	//***
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f, 7.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, -.055f, -.35f)); 
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(1.0f, .0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
			//Short Bot Hook

	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(2.5f, .250f, 1.f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.15f, .0f, -.65f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	//triple jawn
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.15f, .055f, -.65f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.15f, -.055f, -.65f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	//top hook
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f, 2.5f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.4f, .0f, -.07f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//top hook
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.4f, .055f, -.07f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//top hook
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.4f, -.0550f, -.07f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	return true;
}
bool Letters::DrawI() {
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	glm::mat4 worldMatrix;
	glm::mat4 letterTranslate;
	glm::mat4 letterRotate;
	glm::mat4 letterScale;
	glm::mat4 LetterGroupMatrix;
	glm::mat4 partScale;
	glm::mat4 partTranslate;
	glm::mat4 partRo;
	glm::mat4 partMatrix;

	glBindVertexArray(cubeVao);

	letterTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.2f, .0f, .0f));
	letterRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.15f, .15f, .15f));

	glm::mat4 letterParent = this->partParent * letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix = this->groupMatrix * letterParent;

	//long jawn triple Vert
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(7.0f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	//long triple jawn Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//long jawn triple Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, -.055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	return true;
}
bool Letters::DrawM() {
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	glm::mat4 worldMatrix;
	glm::mat4 letterTranslate;
	glm::mat4 letterRotate;
	glm::mat4 letterScale;
	glm::mat4 LetterGroupMatrix;
	glm::mat4 partScale;
	glm::mat4 partTranslate;
	glm::mat4 partRo;
	glm::mat4 partMatrix;

	glBindVertexArray(cubeVao);


	letterTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.2f, .0f, .0f));
	letterRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.15f, .15f, .15f));

	glm::mat4 letterParent = this->partParent * letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix = this->groupMatrix * letterParent;

	//long jawn triple Hor
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f, 4.5f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.15f, .0f, .0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//tripled jawn
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.15f, -.055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//tripled
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.15f, .055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

		//long jawn triple Vert
	//double foor loop for doing the three parallel lines of M
	for (float i = -.2f; i <= .2f; i += .2f) {
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(4.5f, .250f, 1.0f));
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.05f, .0f, i));
		partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
		partMatrix = partTranslate * partScale * partRo;
		worldMatrix = LetterGroupMatrix * partMatrix;

		glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
		
		for (int j = 0; j < 2; j++) {
			if (j== 0) {
				partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.05f, -.055f, i));
				partMatrix = partTranslate * partScale * partRo;
				worldMatrix = LetterGroupMatrix * partMatrix;
				glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
				glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
				glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
			}			
			else if (j==1) {
				partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.05f, .055, i));
				partMatrix = partTranslate * partScale * partRo;
				worldMatrix = LetterGroupMatrix * partMatrix;
				glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
				glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
				glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
			}
		}
	}

	return true;
}
bool Letters::DrawL() {
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	glm::mat4 worldMatrix;
	glm::mat4 letterTranslate;
	glm::mat4 letterRotate;
	glm::mat4 letterScale;
	glm::mat4 LetterGroupMatrix;
	glm::mat4 partScale;
	glm::mat4 partTranslate;
	glm::mat4 partRo;
	glm::mat4 partMatrix;

	glBindVertexArray(cubeVao);


	letterTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.2f, .0f, .0f));
	letterRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.15f, .15f, .15f));

	glm::mat4 letterParent = this->partParent * letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix = this->groupMatrix * letterParent;

	//long jawn triple Vert
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(7.0f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//jawn Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	// tripled Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, -.055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	//long jawn triple Hor
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f, 4.5f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, .0f, .25f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//tripled jawn
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, .055f, .25f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//tripled
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, -.055f, .25f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	return true;
}