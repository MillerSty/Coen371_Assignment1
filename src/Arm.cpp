#include "Arm.h"

Arm::Arm(int cubeVao,
                 glm::mat4 worldMatrix) {
	this->cubeVao = cubeVao;
	this->groupMatrix = worldMatrix;
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->TranslateModel = glm::vec3(0.0f, 0.0f, 0.0f);
	this->TranslateRandom = glm::vec3(0.0f, 0.0f, 0.0f);
	//colours should be set before while loops if we want 
	//random
//colour[0] = glm::vec3(1.0f, 0.0f, 0.0f);
//colour[1] = glm::vec3(0.0f, 1.0f, 0.0f);
//colour[2] = glm::vec3(0.0f, 0.0f, 1.0f);
}
Arm::Arm(int cubeVao, std::string letterName) {
	this->cubeVao = cubeVao;
	this->groupMatrix;
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->renderAs = GL_TRIANGLES;
	armRotate = 0;
	elbowRotate = 0;
	wristRotate = 0;
	this->TranslateModel = glm::vec3(0.0f, 0.0f, 0.0f);
	this->TranslateRandom = glm::vec3(0.0f, 0.0f, 0.0f);
	this->letterName = letterName;
}
void Arm::SetAttr(glm::mat4 groupMatrix, int renderAs, int shaderProgram) {
	this->groupMatrix = groupMatrix;
	this->renderAs = renderAs;
	this->shaderProgram = shaderProgram;
}

void Arm::setTranslation(glm::vec3 TranslateRandom, glm::vec3 TranslateModel) {
	this->TranslateRandom = TranslateRandom;
	this->TranslateModel = TranslateModel;
}

bool Arm::DrawArm() {
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	glm::mat4 worldMatrix;

	glm::mat4 partScale;
	glm::mat4 partTranslate;
	glm::mat4 partRo;
	glm::mat4 partMatrix;
	//note this works

	glBindVertexArray(cubeVao);
	glm::mat4 bicepParent; //so for initiali parent , it has local.global translate plus local rotate 
	glm::mat4 biTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(TranslateRandom.x + TranslateModel.x + -.0f, TranslateModel.y + TranslateRandom.y + -.0f, TranslateRandom.z + -.0f));
	glm::mat4 biRotate = glm::rotate(glm::mat4(1.0f), glm::radians(armRotate), glm::vec3(.0f, 1.0f, .0f));
	bicepParent = biTranslate * biRotate;

	glm::vec3  constArmScaler = glm::vec3(.575f, .25f, .150f); //trying to set up so the whole model is done programatically

	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.575f, .25f, .150f));

	partMatrix = partScale * partRo;
	worldMatrix = groupMatrix * bicepParent * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.94f, .76f, .5f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	// Forearm then for
	glm::mat4 forTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(constArmScaler.x *.0948f, .0f, .0f));
	glm::mat4 forRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)elbowRotate), glm::vec3(.0f, .0f, 1.0f)); //this rotates forearm
	glm::mat4 forarmParent = bicepParent * forTranslate * forRotate;

	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(constArmScaler.x - .02f, constArmScaler.y, constArmScaler.z));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));

	partMatrix = partScale * partRo;
	worldMatrix = groupMatrix * forarmParent * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.94f, .76f, .5f)));
	//glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.98f, .56f, .5f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	// hand
	glm::mat4 handTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(constArmScaler.x *.0666f, .0f, .0f));
	glm::mat4 handRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)wristRotate), glm::vec3(.0f, .0f, 1.0f)); //this rotates hand
	glm::mat4 handParent = forarmParent * handTranslate * handRotate;

	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(constArmScaler.x *.4878f, constArmScaler.y*1.1236f, constArmScaler.z));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));

	partMatrix = partScale * partRo;
	worldMatrix = groupMatrix * handParent * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.0f, .36f, .3f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	partParent = handParent;

	////NOTE FINGERS WILL DIRECTLY INHERIT FROM HAND -> HAND IS THEIR PARENT
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.94f, .76f, .5f))); //al have the same colour
	//
	//// finger1 
	glm::mat4 fingerTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.02f, -.012f, -.0f));
	glm::mat4 fingerRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, .0f)); //finger rotation needs work
	//probably due to its translation?
	glm::mat4 fingerParent = handParent * fingerTranslate * fingerRotate;

	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.187f, 0.05f, .130f));//part scale is universal finger scale maybe put in finger parent?
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.1f, -.0f, -.0f));
	partMatrix = partScale;
	worldMatrix = groupMatrix * fingerParent * partMatrix;

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);


	//could probably for loop it for multiDraw
	////// finger2
	//partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.187f, .05f, .130f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .008f, -.0f));
	partMatrix = partTranslate * partScale;
	worldMatrix = groupMatrix * fingerParent * partMatrix;

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//
	//// finger3
	//partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.187f, .05f, .130f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .016f, -.0f));
	partMatrix = partTranslate * partScale;
	worldMatrix = groupMatrix * fingerParent * partMatrix;

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//
	////// finger4
	//partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.187f, .059f, .130f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .024f, -.0f));
	partMatrix = partTranslate * partScale;
	worldMatrix = groupMatrix * fingerParent * partMatrix;

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//
	////// thumb
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.187f, .075f, .10f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.02f, .029f, -.00f));
	partMatrix = partTranslate * partScale;
	worldMatrix = groupMatrix * fingerParent * partMatrix;

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	return true;
}

//note this works as intended
//bool Arm::DrawArm() {
//	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
//	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
//	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
//	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
//
//	glm::mat4 worldMatrix;
//
//	glm::mat4 partScale;
//	glm::mat4 partTranslate;
//	glm::mat4 partRo;
//	glm::mat4 partMatrix;
//	//note this works
//
//	glBindVertexArray(cubeVao);
//	glm::mat4 bicepParent; //so for initiali parent , it has local.global translate plus local rotate 
//	glm::mat4 biTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(TranslateRandom.x + TranslateModel.x + -.0f, TranslateModel.y + TranslateRandom.y + -.0f, TranslateRandom.z + -.0f));
//	glm::mat4 biRotate = glm::rotate(glm::mat4(1.0f), glm::radians(armRotate), glm::vec3(.0f, 1.0f, .0f));
//	bicepParent = biTranslate * biRotate;
//
//	glm::vec3  constArmScaler = glm::vec3(.575f, .25f, .150f); //trying to set up so the whole model is done programatically
//
//	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
//	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.575f, .25f, .150f));
//
//	partMatrix = partScale * partRo;
//	worldMatrix = groupMatrix * bicepParent * partMatrix;
//	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.94f, .76f, .5f)));
//	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
//	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
//
//	// Forearm then for
//	glm::mat4 forTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(constArmScaler.x / 10.0f - .003, .0f, .0f));
//	glm::mat4 forRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)25), glm::vec3(.0f, .0f, 1.0f));
//	glm::mat4 forarmParent = bicepParent * forTranslate * forRotate;
//
//	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(constArmScaler.x - .02f, constArmScaler.y, constArmScaler.z));
//	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
//
//	partMatrix = partScale * partRo;
//	worldMatrix = groupMatrix * forarmParent * partMatrix;
//	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.94f, .76f, .5f)));
//	//glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.98f, .56f, .5f)));
//	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
//	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
//
//	// hand
//	glm::mat4 handTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(constArmScaler.x / 15.0f, .0f, .0f));
//	glm::mat4 handRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
//	glm::mat4 handParent = forarmParent * handTranslate * handRotate;
//
//	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(constArmScaler.x / 2.05, constArmScaler.y / .89f, constArmScaler.z));
//	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
//
//	partMatrix = partScale * partRo;
//	worldMatrix = groupMatrix * handParent * partMatrix;
//	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.0f, .36f, .3f)));
//	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
//	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
//
//
//	////NOTE FINGERS WILL DIRECTLY INHERIT FROM HAND -> HAND IS THEIR PARENT
//	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.94f, .76f, .5f))); //al have the same colour
//	//
//	//// finger1 
//	glm::mat4 fingerTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.02f, -.012f, -.0f));
//	glm::mat4 fingerRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, -1.0f, .0f));
//	glm::mat4 fingerParent = handParent * fingerTranslate * fingerRotate;
//
//	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.187f, 0.05f, .130f));
//	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.1f, -.0f, -.0f));
//	partMatrix = partScale;
//	worldMatrix = groupMatrix * fingerParent * partMatrix;
//
//	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
//	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
//	//
//	////// finger2
//	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.187f, .05f, .130f));
//	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .008f, -.0f));
//	partMatrix = partTranslate * partScale;
//	worldMatrix = groupMatrix * fingerParent * partMatrix;
//
//	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
//	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
//	//
//	//// finger3
//	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.187f, .05f, .130f));
//	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .016f, -.0f));
//	partMatrix = partTranslate * partScale;
//	worldMatrix = groupMatrix * fingerParent * partMatrix;
//
//	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
//	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
//	//
//	////// finger4
//	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.187f, .059f, .130f));
//	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .024f, -.0f));
//	partMatrix = partTranslate * partScale;
//	worldMatrix = groupMatrix * fingerParent * partMatrix;
//
//	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
//	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
//	//
//	////// thumb
//	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.187f, .075f, .10f));
//	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.02f, .029f, -.00f));
//	partMatrix = partTranslate * partScale;
//	worldMatrix = groupMatrix * fingerParent * partMatrix;
//
//	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
//	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//	return true;
//}