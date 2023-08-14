#include "Arm.h"

Arm::Arm() {}

void Arm::InitArm(glm::vec3 pos, GLuint VAO, Material skin, Material cloth) {
	position = pos;
	cubeVao = VAO;
	skinMaterial = skin;
	clothMaterial = cloth;
    if (position.x < 0){
        armRotate = 90.0f;
    } else {
        armRotate = -90.0f;
    }
	fingerFlex = 0;
	reverse = false;
}

void Arm::SetAttr(glm::mat4 groupMat, int as, int shaderProg) {
	groupMatrix = groupMat;
	renderAs = as;
	shaderProgram = shaderProg;
}

bool Arm::DrawArm() {
    // Get necessary uniform locations
	GLint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

    // Set up necessary matrices
	glm::mat4 worldMatrix;
	glm::mat4 partScale;
	glm::mat4 partTranslate;
	glm::mat4 partRo;
	glm::mat4 partMat;

	// Bind the skin texture
	skinMaterial.bindTexture();
	skinMaterial.loadToShader();

    // Load the cube VAO
	glBindVertexArray(cubeVao);

    // BICEP
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

    partMat = partScale * partRo;
	worldMatrix = groupMatrix * bicepParent * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.94f, .76f, .5f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// FOREARM
	glm::mat4 forTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(constArmScaler.x *.0948f, .0f, .0f));
	glm::mat4 forRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)elbowRotate), glm::vec3(.0f, .0f, 1.0f)); //this rotates forearm
	glm::mat4 forarmParent = bicepParent * forTranslate * forRotate;

	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(constArmScaler.x - .02f, constArmScaler.y, constArmScaler.z));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));

    partMat = partScale * partRo;
	worldMatrix = groupMatrix * forarmParent * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.94f, .76f, .5f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// HAND
    // Bind the cloth texture
	clothMaterial.bindTexture();
	clothMaterial.loadToShader();

	glm::mat4 handTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(constArmScaler.x *.0666f, .0f, .0f));
	glm::mat4 handRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)wristRotate), glm::vec3(.0f, 1.0f, .0f)); //this rotates hand
	glm::mat4 handParent = forarmParent * handTranslate * handRotate;

	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(constArmScaler.x *.4878f, constArmScaler.y*1.1236f, constArmScaler.z));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));

    partMat = partScale * partRo;
	worldMatrix = groupMatrix * handParent * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.0f, .36f, .3f)));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	partParent = handParent;

    // FINGERS
	// NOTE: FINGERS WILL DIRECTLY INHERIT FROM HAND -> HAND IS THEIR PARENT
    // Load skin textures for fingers
	skinMaterial.bindTexture();
	skinMaterial.loadToShader();
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.94f, .76f, .5f))); //al have the same colour
	glm::mat4 fingerTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.02f, -.012f, -.0f));
	glm::mat4 fingerRotater = glm::rotate(glm::mat4(1.0f), glm::radians(fingerRotate), glm::vec3(.0f, -1.0f, .0f));
	glm::mat4 fingerparent= handParent * fingerTranslate ;
	glm::mat4 fingerParent = handParent * fingerTranslate * fingerRotater;

    // FINGER 1A
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.17f, 0.05f, .130f));//part scale is universal finger scale maybe put in finger parent?
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.1f, -.0f, -.0f));
    partMat = partScale;
	worldMatrix = groupMatrix * fingerParent * partMat;

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// FINGER 2
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .008f, -.0f));
    partMat = partTranslate * partScale;
	worldMatrix = groupMatrix * fingerParent * partMat;

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

    // FINGER 3
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .016f, -.0f));
    partMat = partTranslate * partScale;
	worldMatrix = groupMatrix * fingerParent * partMat;

	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

    // FINGER 4
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .024f, -.0f));
    partMat = partTranslate * partScale;
	worldMatrix = groupMatrix * fingerParent * partMat;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	
	// THUMB
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(-.187f, -.075f, .10f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.02f, .029f, -.0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, .0f));
    partMat = partTranslate * partScale;
	worldMatrix = groupMatrix * fingerparent * partMat;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// Setting up finger inheritance
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.17f, .05f, .130f));
	glm::mat4 fingerchildTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.015f, -.0f, -.0f));
	glm::mat4 fingerchildRotater = glm::rotate(glm::mat4(1.0f), glm::radians(fingerRotate / 2.0f), glm::vec3(.0f, -1.0f, .0f));

	glm::mat4 fingerChild = fingerParent* fingerchildTranslate *  fingerchildRotater;

	// FINGER 1B
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, -.0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, 1.0f, .0f));
    partMat = partTranslate * partScale * partRo;
	worldMatrix = groupMatrix * fingerChild * partMat;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// FINGER 2B
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .008f, -.0f));
	partRo = glm::mat4(1.0f);
    partMat = partTranslate * partScale * partRo;
	worldMatrix = groupMatrix * fingerChild * partMat;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// FINGER 3B
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .016f, -.0f));
	partRo = glm::mat4(1.0f);
    partMat = partTranslate * partScale * partRo;
	worldMatrix = groupMatrix * fingerChild * partMat;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// FINGER 4B
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .024f, -.0f));
	partRo = glm::mat4(1.0f);
    partMat = partTranslate * partScale * partRo;
	worldMatrix = groupMatrix * fingerChild * partMat;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	// glDrawArrays(renderAs, 0, 36);
	glBindVertexArray(0);
	
	return true;
}

bool Arm::flexFingers() {
	
	if ((fingerRotate + fingerFlex) > 90.0f && !reverse) {
        fingerRotate -= fingerFlex;
		reverse = true;
	}
	else if ((fingerRotate + fingerFlex) < 0.0f && reverse) {
        fingerRotate += fingerFlex;
		reverse = false;
	}
	else if (reverse) {
        fingerRotate -= fingerFlex;
		fingerFlex -= .01f;
	}
	else {
        fingerRotate += fingerFlex;
		fingerFlex += .01f;
	}
	return reverse;
}
