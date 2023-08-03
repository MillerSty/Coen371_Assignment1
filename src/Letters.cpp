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
	J.bindTexture();
	J.loadToShader();
	glBindVertexArray(cubeVao);

	// J has little notch, long bottom, long Straight,little top 
	//y axis is effectively x axis

																// y   x  z
	letterTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.45f, .0f, .0f));
	letterRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)90), glm::vec3(1.0f, .0f, .0f));
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.3f, .3f, .3f));

	glm::mat4 letterParent = this->partParent * letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix =    letterParent;
	glm::vec4 col = colour[0];
	//long jawn triple Vert
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(7.0f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	//glDrawArrays(renderAs, 0, 36);
	glDrawArrays(renderAs, 0, 36);

	//long triple jawn Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[1])); // 1 is fron
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	//glDrawArrays(renderAs, 0, 36);
	glDrawArrays(renderAs, 0, 36);
	//long jawn triple Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, -.055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	//glDrawArrays(renderAs, 0, 36);
	glDrawArrays(renderAs, 0, 36);

		//long jawn triple Hor
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f, 4.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, .0f, -.235f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	//glDrawArrays(renderAs, 0, 36);
	glDrawArrays(renderAs, 0, 36);

	////***
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f, 4.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, .055f, -.235f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	
	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	//glDrawArrays(renderAs, 0, 36);
	glDrawArrays(renderAs, 0, 36);

	//***
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f,4.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, -.055f, -.235f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(1.0f, .0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	
	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	//glDrawArrays(renderAs, 0, 36);
	glDrawArrays(renderAs, 0, 36);

	//Short Bot Hook
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(2.5f, .250f, 1.f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.15f, .0f, -.375f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(renderAs, 0, 36);
	//triple jawn
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.15f, .055f, -.375f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(renderAs, 0, 36);
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.15f, -.055f, -.375f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(renderAs, 0, 36);
	//top hook
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f, 2.5f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.4f, .0f, -.07f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(renderAs, 0, 36);
	//top hook
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.4f, .055f, -.07f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(renderAs, 0, 36);
	//top hook
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.4f, -.0550f, -.07f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(renderAs, 0, 36);

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
	I.bindTexture();
	I.loadToShader();
	glBindVertexArray(cubeVao);

																// y   x  z
	letterTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.475f, .06f, .0f));
	letterRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)90), glm::vec3(1.0f, .0f, .0f));
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.3f, .3f, .3f));

	glm::mat4 letterParent = this->partParent * letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix =  letterParent;



	//long jawn triple Vert
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(7.0f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[0])); // colour 0 not top layer
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	//long triple jawn Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[1])); //this layer is fron
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//long jawn triple Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, -.055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

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
	M.bindTexture();
	M.loadToShader();
	glBindVertexArray(cubeVao);


																// y   x  z
	letterTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.475f, -.11f, .0f));
	letterRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)90), glm::vec3(1.0f, .0f,.0f));
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.5f, .3f, .3f));

	glm::mat4 letterParent = this->partParent * letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix = letterParent;

	//long jawn triple Hor
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f, 4.5f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.15f, .0f, .0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//tripled jawn
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.15f, -.055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//tripled
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.15f, .055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[1])); //colour 2 is fron
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

		//long jawn triple Vert
	//double foor loop for doing the three parallel lines of M
	for (float i = -.2f; i <= .2f; i += .2f) {
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(4.5f, .250f, 1.0f));
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.05f, .0f, i));
		partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
		partMatrix = partTranslate * partScale * partRo;
		worldMatrix = LetterGroupMatrix * partMatrix;

		glUniform4fv(colorLocation, 1, glm::value_ptr(colour[0]));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(renderAs, 0, 36);
		
		for (int j = 0; j < 2; j++) {
			if (j== 0) {
				partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.05f, -.055f, i));
				partMatrix = partTranslate * partScale * partRo;
				worldMatrix = LetterGroupMatrix * partMatrix;
				glUniform4fv(colorLocation, 1, glm::value_ptr(colour[0]));
				glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
				glDrawArrays(renderAs, 0, 36);
			}			
			else if (j==1) {
				partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.05f, .055, i));
				partMatrix = partTranslate * partScale * partRo;
				worldMatrix = LetterGroupMatrix * partMatrix;
				glUniform4fv(colorLocation, 1, glm::value_ptr(colour[1]));
				glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
				glDrawArrays(renderAs, 0, 36);
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

	L.bindTexture();
	L.loadToShader();												
																	// y   x  z
	letterTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.475f, -.02f, .0f));
	letterRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)90), glm::vec3(1.0f, .0f, .0f));
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.3f, .3f, .3f));

	glm::mat4 letterParent = this->partParent * letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix =  letterParent;

	//long jawn triple Vert
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(7.0f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//jawn Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[1])); //is front
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	// tripled Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, -.055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	//long jawn triple Hor
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f, 4.5f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, .0f, .25f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//tripled jawn
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, .055f, .25f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//tripled
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, -.055f, .25f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform4fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	return true;
}