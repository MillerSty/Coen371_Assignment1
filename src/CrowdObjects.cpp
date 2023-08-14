#include "CrowdObjects.h"
#define cubeVao vaos[0]
#define sphereVao vaos[1]
#define colour glm::vec3(1,1,1)



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
			case(0): scale = 1.0f; break;
			case(1):scale = .85f; break;
			case(2):scale = .75f; break;
			case(3):scale = .6f; break;
			default: break;
			}

		for (float i = -.75; i < .75; i += .1) { //does one row
			crowd.push_back(glm::vec3(i, .25*scale, -.8*scale));
		}
		crowdy.push_back(crowd);
	}
	
	for (int i = 0; i < crowd.size(); i++) {
		if (i % 2 == 0) {
			drawSingle(crowd[i], glm::vec3(( crowdRotation.x),crowdRotation.y, crowdRotation.z / 20.0f), crowd.size(), i);
		}
		else {
			drawSingle(crowd[i], glm::vec3((.004f- crowdRotation.x), -1.0f*crowdRotation.y, -1.0f*crowdRotation.z/20.0f), crowd.size(), i);
		}
	}
}

void CrowdObjects::drawSingle(glm::vec3 position,glm::vec3 armrotate, int crowdSize, int crowdPosition) {
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

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

    glm::mat4 crowdParent;
    glm::mat4 crowdTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(position.x,position.y+ armrotate.x,position.z));
    glm::mat4 crowdScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    glm::mat4 crowdRotate = glm::rotate(glm::mat4(1.0f), glm::radians(armrotate.z), glm::vec3(.0f, 1.0f, .0f));

    //Note: No part matrices but we may want them eventually
    glm::mat4 partTrans= glm::translate(glm::mat4(1.0f), glm::vec3(0));
    glm::mat4 partSca = glm::scale(glm::mat4(1.0f), glm::vec3(.50f, 1.0f, .50f));
    //partRo is for bodyMovement
    glm::mat4 partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, 1.0f, .0f));
    glm::mat4 partMa= partSca*partRo;

    //armrotate.x is translation on y
    //armrotate.y is rotation of arm
    //armrotate.z is body movement
    crowdParent = crowdTranslate* crowdRotate;

    worldMatrix = groupMatrix * crowdParent* partMa;

    glBindVertexArray(sphereVao);
    glUniform3fv(colorLocation, 1, glm::value_ptr(col[crowdPosition]));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    //glDrawArrays(renderAs, 0, 36);
    glDrawElements(renderAs, sphereIndexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    //arms
    //left (facing +z ) arm
    glBindVertexArray(cubeVao);

    partTrans = glm::translate(glm::mat4(1.0f), glm::vec3(-0.03f, .015f, 0.01f));
    partRo = glm::rotate(glm::mat4(1.0f), glm::radians(armrotate.y), glm::vec3(1.0f, .0f, .0f));//arm rotate here
    partMa = crowdParent * partTrans* partRo ;

    partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.125f, .550f, .125f));
    partRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(1.0f, .0f, .0f));
    partParent =  partScale* partRotate;
    worldMatrix = groupMatrix * partMa * partParent;

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(renderAs, 0, 36);
    //right (facing +z ) arm

    partTrans = glm::translate(glm::mat4(1.0f), glm::vec3(0.03f, .015f, 0.01f));
    partRo = glm::rotate(glm::mat4(1.0f), glm::radians(armrotate.y), glm::vec3(1.0f, .0f, .0f));//arm rotate here
    partMa = crowdParent * partTrans * partRo;

    partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.125f, .550f, .125f));
    partRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(1.0f, .0f, .0f));
    partParent = partScale * partRotate;
    worldMatrix = groupMatrix * partMa * partParent;

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(renderAs, 0, 36);
    glBindVertexArray(0);
}

glm::vec3 CrowdObjects::animateCrowd() {
	if ((crowdRotation.y + fingerFlex)>179.0f && reverse==false) {//so if arm rotation is greate then 90 
		crowdRotation.y -= fingerFlex;
		reverse = true;

	}
	else if ((crowdRotation.y + fingerFlex) < 0.0f && reverse == true) {
		crowdRotation.y += fingerFlex;
		reverse = false;

	}
	else if (reverse == true) {
		crowdRotation.y -= fingerFlex;
		fingerFlex -= .01f;
	}
	else {
		crowdRotation.y += fingerFlex;
		fingerFlex += .01f;
	}

	if ((crowdRotation.x + jumpFlex) > .004f && reverse2 == false) {//so if arm rotation is greate then 90 
		crowdRotation.x -= jumpFlex;
		reverse2 = true;

	}
	else if ((crowdRotation.x + jumpFlex) < 0.0f && reverse2 == true) {
		crowdRotation.x += jumpFlex;
		reverse2 = false;

	}
	else if (reverse2 == true) {
		crowdRotation.x -= jumpFlex;
		jumpFlex -= .00001f;
	}
	else {
		crowdRotation.x += jumpFlex;
		jumpFlex += .00001f;
	}

	if ((crowdRotation.z + turnFlex) > 110.0f && reverse3 == false) {
		crowdRotation.z -= turnFlex;
		reverse3 = true;
	
	}
	else if ((crowdRotation.z - turnFlex) < 75.0f && reverse3 == true) {
		crowdRotation.z += turnFlex;
		reverse3 = false;

	}
	else if (reverse3 == true) {
		crowdRotation.z -= turnFlex;
		turnFlex -= .01f;
	}
	else {
		crowdRotation.z += turnFlex;
		turnFlex += .01f;
	}
	return glm::vec3(1);
}

void CrowdObjects::walker() {
	double currentWorldTime = glfwGetTime();

	if (keyFrameTrackerWalker <= (sizeof(path) / sizeof(KeyFrame)) - 2)
	{
		// Handle time calculations
		double currentFrameTime = path[keyFrameTrackerWalker].time;
		double nextFrameTime = path[keyFrameTrackerWalker + 1].time;
		double frameDuration = nextFrameTime - currentFrameTime;
		double timeProportion = (currentWorldTime - currentFrameTime) / frameDuration;

		// Handle setting how much X should translate based on time proportion
		double currentCoordX = path[keyFrameTrackerWalker].translation.x;
		double nextCoordX = path[keyFrameTrackerWalker + 1].translation.x;
		currentCoordX += (nextCoordX - currentCoordX) * timeProportion;

		// Handle setting how much Y should translate based on time proportion
		double currentCoordY = path[keyFrameTrackerWalker].translation.y;
		double nextCoordY = path[keyFrameTrackerWalker + 1].translation.y;
		currentCoordY += (nextCoordY - currentCoordY) * timeProportion;

		// Handle setting how much Z should translate based on time proportion
		double currentCoordZ = path[keyFrameTrackerWalker].translation.z;
		double nextCoordZ = path[keyFrameTrackerWalker + 1].translation.z;
		currentCoordZ += (nextCoordZ - currentCoordZ) * timeProportion;

		//armrotate.x is translation on y 
	//armrotate.y is rotation of arm
	//armrotate.z is body movement
		//glm::linearRand(-75,-110)
		glm::vec3 singleVec(crowdRotation.x, crowdRotation.y, -1.0f*crowdRotation.z);
		// Set the model translation in world space
		drawSingle(glm::vec3(currentCoordX, currentCoordY, currentCoordZ), singleVec, 1, 1);


		singleVec=glm::vec3 (crowdRotation.x, crowdRotation.y, 180.0f-(-1*   crowdRotation.z));
		drawSingle(glm::vec3(currentCoordX+.1f, currentCoordY, currentCoordZ), singleVec, 1, 1);

		 singleVec=glm::vec3(crowdRotation.x, crowdRotation.y, -1.0f * crowdRotation.z);
		drawSingle(glm::vec3(currentCoordX+.2f, currentCoordY, currentCoordZ), singleVec, 1, 1);

		// If the realtime clock is beyond the next keyframes time parameter, move to the next keyframe
		if (currentWorldTime >= nextFrameTime)
			keyFrameTrackerWalker++;
	}
}
