#include "Racket.h"

Racket::Racket(GLuint vao, std::string racket) : cubeVao(vao), racketName(racket) {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	racketAngle = 90;
	rotationW = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	jawnAngle = 0;
}

void Racket::SetAttr(glm::mat4 groupMat, int as, int shaderProg, glm::mat4 parent) {
	groupMatrix = groupMat;
	renderAs = as;
	shaderProgram = shaderProg;
	partParent = parent;
}

//Draw racket follows from code in Assignment 1 just placed in a class now
bool Racket::Draw() {
	// Get necessary uniform locations
	GLint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

    // Bind the plastic texture
	plasticMaterial.bindTexture();
	plasticMaterial.loadToShader();

    // Set necessary matrices
	glm::mat4 worldMatrix;
	glm::mat4 handleTranslate;
	glm::mat4 handleRotate;
	glm::mat4 handleScale;
	glm::mat4 RacketGroupMatrix;	
	glm::mat4 partScale;
	glm::mat4 partTranslate;
	glm::mat4 partRo;	
	glm::mat4 partMat;

    // Bind VAO
	glBindVertexArray(cubeVao);

	handleTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,.009f));
	handleRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)racketAngle), glm::vec3(.0f, .0f, 1.0f));
	//this float will determine individual rotation
	handleRotate *= glm::rotate(glm::mat4(1.0f),glm::radians(jawnAngle), glm::vec3(1.0f, .0f, .0f));
	handleScale = glm::scale(glm::mat4(1.0f), glm::vec3(.40f, .40f, .40f));

	glm::mat4 handleParent = partParent * handleTranslate*handleScale * handleRotate;
	RacketGroupMatrix = groupMatrix * handleParent;
	partChild = RacketGroupMatrix;
	glm::mat4 check= handleTranslate * handleRotate;

	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.75f, .25f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, .0f, 1.0f));
	worldMatrix = RacketGroupMatrix * partTranslate * partScale * partRo;

	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.f, .0f, .0f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// RACKET HOOPS
    // Bracket top handle
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(35.0f), glm::vec3(.0f, .0f, 1.0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.9f, .25f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.1f, .03f, -.0f));
	partMat = partTranslate * partRo * partScale;
	worldMatrix = RacketGroupMatrix * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.61f, .61f, .65f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// Straight top
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, .0f, 1.0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.3f, .25f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.19f, .055f, -.0f));
	partMat = partTranslate * partRo * partScale;
	worldMatrix = RacketGroupMatrix * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.98f, .56f, .5f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// Bracket top tip
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(-35.0f), glm::vec3(.0f, .0f, 1.0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.6f, .25f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.27f, .045f, -.0f));
    partMat = partTranslate * partRo * partScale;
	worldMatrix = RacketGroupMatrix * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.661f, .61f, .61)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// Bracket bottom handle
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(35.0f), glm::vec3(.0f, .0f, -1.0f));
	partRo *= glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, .0f, .0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.9f, .25f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.1f, -.03f, -.00f));
    partMat = partTranslate * partRo * partScale;
	worldMatrix = RacketGroupMatrix * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.61f, .61f, .661f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// Straight bottom
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, .0f, 1.0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.3f, .25f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.19f, -.058f, -.0f));
    partMat = partTranslate * partRo * partScale;
	worldMatrix = RacketGroupMatrix * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.98f, .56f, .5f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// Bracket top tip
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(-35.0f), glm::vec3(.0f, .0f, -1.0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.6f, .25f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.27f, -.04f, -.0f));
    partMat = partTranslate * partRo * partScale;
	worldMatrix = RacketGroupMatrix * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.61f, .616f, .61f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// Closing tip
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, .0f, 1.0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.24f, .65f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.29f, .00f, -.0f));
    partMat = partTranslate * partRo * partScale;
	worldMatrix = RacketGroupMatrix * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.98f, .56f, .5f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// Closing part
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, .0f, 1.0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.14f, .95f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.13f, -.0f, -.0f));
    partMat = partTranslate * partRo * partScale;
	worldMatrix = RacketGroupMatrix * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.98f, .56f, .5f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.0f, 1.0f, .0f)));

    // RACKET NETTING
	for (float i = -.05; i <= .06; i += .01) {
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.375f, .08f, .080f));
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.202, i, -.0f));
        partMat = partTranslate * partRo * partScale;
		worldMatrix = RacketGroupMatrix * partMat;
	
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(renderAs, 0, 36);
	}
	
	for (float i = .13; i <= .27; i += .01) {
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.08f, .95f, .080f));
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3((float)i, .0f, -.0f));
        partMat = partTranslate * partRo * partScale;
		worldMatrix = RacketGroupMatrix * partMat;
	
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(renderAs, 0, 36);
	}

    // Reset diffuse, specular, ambient, and shininess parameters for plastic
	plasticMaterial.resetShader();

    // Unbind VAO
	glBindVertexArray(0);

	//boolean if shadows check
	return true;
}
