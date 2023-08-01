#include "Racket.h"

//using namespace EvanRacket;
//using namespace EvanRacket;
Racket::Racket(int cubeVao, glm::mat4 worldMatrix)   {
	this->cubeVao = cubeVao;
	this->groupMatrix = worldMatrix;
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->racketAngle = 90;
	this->rotationW= glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	this->jawnAngle = 0;

}
Racket::Racket(int cubeVao, std::string racketName) {
	this->cubeVao = cubeVao;
	this->groupMatrix;
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->racketAngle = 90;
	this->rotationW = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	//this->jawnAngle = 0;
	this->racketName = racketName;
}



void Racket::SetAttr(glm::mat4 groupMatrix, int renderAs, int shaderProgram, glm::mat4 partParent) {
	this->groupMatrix = groupMatrix;
	this->renderAs = renderAs;
	this->shaderProgram = shaderProgram;
	this -> partParent = partParent;
	
}
//Draw racket follows from code in Assignment 1 just placed in a class now
bool Racket::Draw() {

	//GLuint applyTexturesLocation = glGetUniformLocation(shaderProgram, "shouldApplyTexture");
	//glUniform1i(applyTexturesLocation, true);
	
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	plasticMaterial.bindTexture();
	plasticMaterial.loadToShader();

	glm::mat4 worldMatrix;
	glm::mat4 handleTranslate;
	glm::mat4 handleRotate;
	glm::mat4 handleScale;
	glm::mat4 RacketGroupMatrix;	
	
	glm::mat4 partScale;
	glm::mat4 partTranslate;
	glm::mat4 partRo;	
	glm::mat4 partMatrix;	
	
	glBindVertexArray(cubeVao);

	handleTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,.009f));
	handleRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)racketAngle), glm::vec3(.0f, .0f, 1.0f));
	//this float will determine individualt rotation
	handleRotate *= glm::rotate(glm::mat4(1.0f),glm::radians(this->jawnAngle), glm::vec3(1.0f, .0f, .0f));
	handleScale = glm::scale(glm::mat4(1.0f), glm::vec3(.40f, .40f, .40f));

	glm::mat4 handleParent = partParent*handleTranslate*handleScale* handleRotate;
	RacketGroupMatrix = groupMatrix * handleParent;
	glm::mat4 check= handleTranslate * handleRotate;
	//this->partParent = handleTranslate* handleRotate;

	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.75f, .25f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, .0f, 1.0f));
	worldMatrix = RacketGroupMatrix * partTranslate * partScale * partRo;

	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.f, .0f, .0f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);


	//racket hoop
		//bracket top Handle
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)35), glm::vec3(.0f, .0f, 1.0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.9f, .25f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.1f, .03f, -.0f));
	
	//groupMatrix*
	partMatrix = partTranslate * partRo * partScale;
	worldMatrix = RacketGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.61f, .61f, .65f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//
	//// straight top
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.3f, .25f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.19f, .055f, -.0f));
	//groupMatrix *
	partMatrix = partTranslate * partRo * partScale;
	worldMatrix = RacketGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.98f, .56f, .5f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//
	////bracket top Tip
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)-35), glm::vec3(.0f, .0f, 1.0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.6f, .25f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.27f, .045f, -.0f));
	
	partMatrix = partTranslate * partRo * partScale;
	worldMatrix = RacketGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.661f, .61f, .61)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//
	//////bracket bot Handle
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)35), glm::vec3(.0f, .0f, -1.0f));
	partRo *= glm::rotate(glm::mat4(1.0f), glm::radians((float)180), glm::vec3(1.0f, .0f, .0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.9f, .25f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.1f, -.03f, -.00f));
	
	partMatrix = partTranslate * partRo * partScale;
	worldMatrix = RacketGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.61f, .61f, .661f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//
	//
	//// straight bot
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.3f, .25f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.19f, -.058f, -.0f));
	
	partMatrix = partTranslate * partRo * partScale;
	worldMatrix = RacketGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.98f, .56f, .5f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//
	////bracket top Tip
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)-35), glm::vec3(.0f, .0f, -1.0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.6f, .25f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.27f, -.04f, -.0f));
	
	partMatrix = partTranslate * partRo * partScale;
	worldMatrix = RacketGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.61f, .616f, .61f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//
	////closing tip
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.24f, .65f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.29f, .00f, -.0f));
	
	partMatrix = partTranslate * partRo * partScale;
	worldMatrix = RacketGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.98f, .56f, .5f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//
	////closing Jawn
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.14f, .95f, .150f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.13f, -.0f, -.0f));
	
	partMatrix = partTranslate * partRo * partScale;
	worldMatrix = RacketGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.98f, .56f, .5f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	
	
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.0f, 1.0f, .0f)));
	for (float i = -.05; i <= .06; i += .01) {
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.375f, .08f, .080f));
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.202, i, -.0f));
		partMatrix = partTranslate * partRo * partScale;
		worldMatrix = RacketGroupMatrix * partMatrix;
	
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(renderAs, 0, 36);
		//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	}
	
	for (float i = .13; i <= .27; i += .01) {
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.08f, .95f, .080f));
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3((float)i, .0f, -.0f));
		partMatrix = partTranslate * partRo * partScale;
		worldMatrix = RacketGroupMatrix * partMatrix;
	
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(renderAs, 0, 36);
		//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	}
	plasticMaterial.resetShader();
	glBindVertexArray(0);
	return true;
}