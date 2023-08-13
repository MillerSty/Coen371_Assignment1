#include "Arm.h"

Arm::Arm(int cubeVao, glm::mat4 worldMatrix) {
	this->cubeVao = cubeVao;
	this->groupMatrix = worldMatrix;
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->TranslateModel = glm::vec3(0.0f, 0.0f, 0.0f);
	this->TranslateRandom = glm::vec3(0.0f, 0.0f, 0.0f);

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
void Arm::InitArm(glm::vec3 position, GLuint VAO, Material skinMaterial, Material clothMaterial) {
	this->position = position;
	this->cubeVao = VAO;
	this->skinMaterial = skinMaterial;
	this->clothMaterial = clothMaterial;
    if (position.x < 0){
        this->armRotate = 90.0f;
    } else {
        this->armRotate = -90.0f;
    }
	fingerFlex = 0;
	reverse = false;
}
Arm::Arm() {}
void Arm::resetArm() {
	TranslateModel += -1.0f * TranslateModel;
	TranslateRandom += -1.0f * TranslateRandom;
	armRotate = 90.0f;
	wristRotate = 0.0f;
	elbowRotate = 0.0f;
	fingerRotate = 0.0f;
	armRotate = 90.0f;
	//Scale= 1.0f
}
void Arm::SetAttr(glm::mat4 groupMatrix, int renderAs, int shaderProgram) {
	this->groupMatrix = groupMatrix;
	this->renderAs = renderAs;
	this->shaderProgram = shaderProgram;
	float fingerFlex = 0;
	bool reverse = false;
}

void Arm::setVAO(int vao) { cubeVao = vao; }

void Arm::setTranslation(glm::vec3 TranslateRandom, glm::vec3 TranslateModel) {
	this->TranslateRandom = TranslateRandom;
	this->TranslateModel = TranslateModel;
}

bool Arm::DrawArm() {
	GLint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	glm::mat4 worldMatrix;

	glm::mat4 partScale;
	glm::mat4 partTranslate;
	glm::mat4 partRo;
	glm::mat4 partMatrix;
	//note this works
	skinMaterial.bindTexture();
	skinMaterial.loadToShader();

	glBindVertexArray(cubeVao);
	glm::mat4 bicepParent; //so for initial parent , it has local.global translate plus local rotate
	glm::mat4 biTranslate = glm::translate(glm::mat4(1.0f),
                                           glm::vec3(TranslateRandom.x + TranslateModel.x + position.x,
                                                     TranslateModel.y + TranslateRandom.y + position.y,
                                                     TranslateRandom.z + TranslateModel.z + position.z));
	glm::mat4 biRotate = glm::rotate(glm::mat4(1.0f), glm::radians(armRotate), glm::vec3(.0f, 1.0f, .0f));
	bicepParent = biTranslate * biRotate;

	glm::vec3  constArmScaler = glm::vec3(.575f, .25f, .150f); //trying to set up so the whole model is done programmatically

	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.575f, .25f, .150f));

	partMatrix = partScale * partRo;
	worldMatrix = groupMatrix * bicepParent * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.94f, .76f, .5f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// Forearm then for
	glm::mat4 forTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(constArmScaler.x *.0948f, .0f, .0f));
	glm::mat4 forRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)elbowRotate), glm::vec3(.0f, .0f, 1.0f)); //this rotates forearm
	glm::mat4 forarmParent = bicepParent * forTranslate * forRotate;

	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(constArmScaler.x - .02f, constArmScaler.y, constArmScaler.z));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));

	partMatrix = partScale * partRo;
	worldMatrix = groupMatrix * forarmParent * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.94f, .76f, .5f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// hand
	clothMaterial.bindTexture();
	clothMaterial.loadToShader();
	glm::mat4 handTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(constArmScaler.x *.0666f, .0f, .0f));
	glm::mat4 handRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)wristRotate), glm::vec3(.0f, 1.0f, .0f)); //this rotates hand
	glm::mat4 handParent = forarmParent * handTranslate * handRotate;

	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(constArmScaler.x *.4878f, constArmScaler.y*1.1236f, constArmScaler.z));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));

	partMatrix = partScale * partRo;
	worldMatrix = groupMatrix * handParent * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.0f, .36f, .3f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	partParent = handParent;

	////NOTE FINGERS WILL DIRECTLY INHERIT FROM HAND -> HAND IS THEIR PARENT
	skinMaterial.bindTexture();
	skinMaterial.loadToShader();
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.94f, .76f, .5f))); //al have the same colour
	glm::mat4 fingerTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.02f, -.012f, -.0f));
	glm::mat4 fingerRotater = glm::rotate(glm::mat4(1.0f), glm::radians(fingerRotate), glm::vec3(.0f, -1.0f, .0f));
	glm::mat4 fingerparent= handParent * fingerTranslate ;
	glm::mat4 fingerParent = handParent * fingerTranslate * fingerRotater;
	//// finger1a 
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.17f, 0.05f, .130f));//part scale is universal finger scale maybe put in finger parent?
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.1f, -.0f, -.0f));
	partMatrix = partScale;
	worldMatrix = groupMatrix * fingerParent * partMatrix;

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	////// finger2
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .008f, -.0f));
	partMatrix = partTranslate * partScale;
	worldMatrix = groupMatrix * fingerParent * partMatrix;

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//// finger3
	//partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.187f, .05f, .130f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .016f, -.0f));
	partMatrix = partTranslate * partScale;
	worldMatrix = groupMatrix * fingerParent * partMatrix;

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	////// finger4
	//partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.187f, .059f, .130f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .024f, -.0f));
	partMatrix = partTranslate * partScale;
	worldMatrix = groupMatrix * fingerParent * partMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	
	////// thumb //thumb rotates wrong
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(-.187f, -.075f, .10f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.02f, .029f, -.0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, .0f));
	partMatrix = partTranslate * partScale;
	worldMatrix = groupMatrix * fingerparent * partMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	//setting up finger inheritance
	//glm::mat4 fingerChild = handParent * fingerParent * glm::translate(glm::mat4(1.0f), glm::vec3(.017f, .0f, -.0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.17f, .05f, .130f));
	glm::mat4 fingerchildTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.015f, -.0f, -.0f));
	glm::mat4 fingerchildRotater = glm::rotate(glm::mat4(1.0f), glm::radians(fingerRotate / 2.0f), glm::vec3(.0f, -1.0f, .0f));

	glm::mat4 fingerChild = fingerParent* fingerchildTranslate *  fingerchildRotater;
	worldMatrix = groupMatrix * fingerChild * partMatrix;
	//finger 1b
	//partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.187f, 0.05f, .130f));//part scale is universal finger scale maybe put in finger parent?
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, -.0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, 1.0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = groupMatrix * fingerChild * partMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//finger 2b
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .008f, -.0f));
	partRo = glm::mat4(1.0f);
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = groupMatrix * fingerChild * partMatrix;

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//finger 3b
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .016f, -.0f));
	partRo = glm::mat4(1.0f);
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = groupMatrix * fingerChild * partMatrix;

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//finger 4b
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .024f, -.0f));
	partRo = glm::mat4(1.0f);
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = groupMatrix * fingerChild * partMatrix;

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	//partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.17f, 0.05f, .130f));//part scale is universal finger scale maybe put in finger parent?
	//partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.1f, -.0f, -.0f));
	//partMatrix = partScale;
	//worldMatrix = groupMatrix * fingerParent * partMatrix;

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	glBindVertexArray(0);
	
	return true;
}

bool Arm::flexFingers() {
	
	if ((this->getFRotation() + fingerFlex) > 90.0f && reverse == false) {

		this->setFRotation(this->getFRotation() - fingerFlex);
		reverse = true;
	}
	else if (this->getFRotation() + fingerFlex < 0.0f && reverse == true) {
		this->setFRotation(this->getFRotation() + fingerFlex);
		reverse = false;
	}
	else if (reverse == true) {
		this->setFRotation(this->getFRotation() - fingerFlex);
		fingerFlex -= .01f;
	}
	else {
		this->setFRotation(this->getFRotation() + fingerFlex);
		fingerFlex += .01f;
	}

	return reverse;
}