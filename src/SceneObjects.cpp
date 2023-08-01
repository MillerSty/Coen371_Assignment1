#include "SceneObjects.h"


SceneObjects::SceneObjects(int cubeVao,	glm::mat4 worldMatrix) {
	this->cubeVao=cubeVao;
	this->groupMatrix = worldMatrix;
}
SceneObjects::SceneObjects(int cubeVao, std::string objectName) {
	this->cubeVao=cubeVao;
	this->objectName = objectName;
	this->Translate = glm::vec3(0.0f, 0.0f, 0.0f);
}
SceneObjects::SceneObjects(std::string objectName) {
	
	this->objectName = objectName;
	this->Translate = glm::vec3(0.0f, 0.0f, 0.0f);
}

void SceneObjects::SetAttr(glm::mat4 groupMatrix, int renderAs, int shaderProgram) { 
	this->groupMatrix = groupMatrix;
	this->renderAs = renderAs;
	this->shaderProgram = shaderProgram;
	 }
void SceneObjects::SetVAO(int cube, int grid) {
	this->cubeVao = cube;
	this->cubeVao = cube;
	this->gridVao = grid;
}
void SceneObjects::setMaterials(Material courtTexture,	Material clothTexture,
	Material ropeTexture,	Material metalTexture,
	Material grassTexture,	Material plasticTexture) {
		this->courtTexture =  courtTexture;
		this->ropeTexture   = ropeTexture ;
		this->metalTexture =  metalTexture;
		this->clothTexture =  clothTexture;
		this->grassTexture =  grassTexture;
		this->plasticTexture = plasticTexture;
}

void SceneObjects::InitGrid() {
	this->gridCount = 0;
	//int o = j;
	for (int i = -100; i <= 100; i += 2) { //needs to be 100 x 100
		// if (i != 0) {
		float f = i / 100.0f;
		//this is for x z grid
		lineArray[gridCount] = glm::vec3(f, .0f, -1.0f);
		lineArray[gridCount + 1] = glm::vec3(0.61f, .61f, .61f);//colour
		lineArray[(gridCount + 2)] = glm::vec3(f, .0f, 1.0f);
		lineArray[gridCount + 3] = glm::vec3(0.61f, .61f, .61f);//colour

		lineArray[gridCount + 4] = glm::vec3(-1.0f, 0.0f, f);
		lineArray[gridCount + 5] = glm::vec3(0.61f, .61f, .61f);//colour
		lineArray[(gridCount + 6)] = glm::vec3(1.0f, 0.0f, f);
		lineArray[gridCount + 7] = glm::vec3(0.61f, .61f, .61f);
		this->gridCount += 8; //400 for first 50	

	}
}
void SceneObjects::DrawScene(bool drawSkyBox) {
	bool check = DrawCourt();
	if (!check) printf("Draw Court failed");

	// Make sure not to use the skybox itself when rendering shadows
	if (drawSkyBox)
	{
		check = DrawSkyBox();
		if (!check) printf("Draw DrawSkyBox failed");
	}
	//check = DrawGrid();
	//if (!check) printf("Draw DrawGrid failed");
	//check = DrawCoord();
	if (!check) printf("Draw DrawCoord failed");
	//check = DrawBall();
	if (!check) printf("Draw DrawBall failed");
	check = DrawCoord();
	if (!check) printf("Draw DrawCoord failed");
	
}

bool SceneObjects::DrawBall() {
	grassTexture.bindTexture();
	grassTexture.loadToShader();
	
	//glBindTexture(GL_TEXTURE_2D, grassTexture);
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

	glm::mat4 ballScale = glm::scale(glm::mat4(1.0f), glm::vec3( .50f, .50f, .50f));
	glm::mat4 ballTranslate = glm::translate(glm::mat4(1.0f), glm::vec3( .0666f, .3f, .0f));
	glm::mat4 ballRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f)); //this rotates hand
	glm::mat4 ballParent = ballTranslate * ballScale * ballRotate;

	//partScale = glm::scale(glm::mat4(1.0f), glm::vec3(constArmScaler.x * .4878f, constArmScaler.y * 1.1236f, constArmScaler.z));
	//partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	//
	//partMatrix = partScale * partRo;
	glm::mat4 worldMatrix = groupMatrix * ballParent;

	glBindVertexArray(sphereVao);
	//translate
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.1180f, 1.0f, .122f)));
	glDrawElements(renderAs, sphereVertCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	return true;
}

bool SceneObjects::DrawCourt() {
	//set as textureshader and bind texture

	courtTexture.bindTexture();
	courtTexture.loadToShader();
	//glActiveTexture(GL_TEXTURE0);
	//GLuint textureLocation = glGetUniformLocation(shaderProgram, "textureSampler");
	//glBindTexture(GL_TEXTURE_2D, courtTexture);
	//glUniform1i(textureLocation, 0);


	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

	glBindVertexArray(cubeVao);
	//2d texture
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 0.0, .0)));//red (x)
	glm::mat4 partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.2f, .2f, .2f));
	int absX = 0;
	int absZ = 0;
	for (int pitchx = -78; pitchx < 78; pitchx += 2) {
		for (int pitchz = -36; pitchz < 36; pitchz += 2) {
			//now i need a way to change the colour 				
			glm::mat4 partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(pitchx / (float)100, -.01, pitchz / (float)100 + .01));
			glm::mat4 partMatrix = partTranslate * partScale;
			glm::mat4 worldMatrix =  groupMatrix* partMatrix;
			if (abs(pitchx) == 74 && (abs(pitchz) < 31)) {	 //back horizontal				 
				glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));//white
			}
			else if ((abs(pitchz) == 32) && abs(pitchx) < 76) {	//length lines			
				glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));//white
			}
			else if (abs(pitchx) == 54 && (abs(pitchz) <= 30)) { //inside horizontal	
				glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));//white
			}
			else if (abs(pitchx) < 54 && (abs(pitchz) == 0)) { //inside vertical			
				glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));//white
			}
			else { //pitch is green
				glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.0, 0.522, .40)));// 
			}
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
	}
	glBindVertexArray(0);
	bool check = DrawNet();
	return true;
}

bool SceneObjects::DrawNet() {

	//bind texture Metal

	metalTexture.bindTexture();
	metalTexture.loadToShader();
	//glBindTexture(GL_TEXTURE_2D, metalTexture);

	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

	//bind VAO
	glBindVertexArray(cubeVao);
	// Set colour of net poles
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.60f, .6f, .60f)));

	
	// Rotate net poles
	glm::mat4 partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, 1.0f));

	// Translate one net pole
	glm::mat4 partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0375f, -.34f));

	// Scale net poles
	glm::mat4 partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.25f, .75f, .25f));

	// Set part matrix based on translation, scaling, and rotation. Set world matrix based on part and group
	glm::mat4 partMatrix = partTranslate * partScale * partRo;
	glm::mat4 worldMatrix = groupMatrix * partMatrix;

	// Draw world matrix with indices
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);


	//+ve Z net
	// Translate only the new pole. Can reuse scale and translation matrices from first pole
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0375f, .34f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = groupMatrix * partMatrix;

	// Draw again
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	// Strings of net
	ropeTexture.bindTexture();
	ropeTexture.loadToShader();
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));  // Set net colour
	//glBindTexture(GL_TEXTURE_2D, ropeTexture); //Bind texture Ropes

	// Loop to draw net. i is our translation offset
	for (float i = .0; i <= .07f; i += .01) {
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.03f, .03f, 7.0f));  // Scale to beautiful net dimensions
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.005, i, -.0f));  // Translate with that i offset
		partMatrix = partTranslate * partScale; // Set part matrix
		worldMatrix = groupMatrix * partMatrix; // Set world matrix

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]); //comment
		glDrawArrays(renderAs, 0, 36);
		//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);//draw with index
	}

	// Same as above, but net offset is along z this time
	for (float i = -.34; i <= .34f; i += .01) {
		// The scale is also different to account for the new direction
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.03f, .7f, .03f));
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, .04, i));
		partMatrix = partTranslate * partScale;
		worldMatrix = groupMatrix * partMatrix;

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(renderAs, 0, 36);
		//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	}

	// Top bar of the net

	clothTexture.bindTexture();
	clothTexture.loadToShader();
	//glBindTexture(GL_TEXTURE_2D, clothTexture); //bind top cloth of net
	//glUniform1i(textureLocation, 0);

	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.00f, 1.0f, 1.0f)));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.003f, .071f, .0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.1f, .1f, 6.6f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = groupMatrix * partMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	// Unbinding the vertex array object 
	glBindVertexArray(0);
	return true;
}

bool SceneObjects::DrawSkyBox() {
	plasticTexture.bindTexture();
	plasticTexture.loadToShader();
	//glBindTexture(GL_TEXTURE_2D, plasticTexture);
	//glUseProgram(shaderProgram);
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	float skyBoxY = 160.0f; // Change this to make sky box closer to court
	
	// Sets sky box position
	glm::mat4 partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// Unused but usable
	//glm::mat4 partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(0.0f, 1.0f, 1.0f));

	// Sets sky box scale 
	glm::mat4 partScale = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, skyBoxY, 50.0f));

	glm::mat4 partMatrix = partTranslate * partScale;  // Part matrix for sky box
	glm::mat4 worldMatrix = groupMatrix * partMatrix;  // World matrix for sky box

	// Bind the vertex array object to be the cube VAO with reverse winding order
	glBindVertexArray(cubeVao);
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.008f, .8f, .9999999f))); // Sky box colour
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]); // Send to shader
	glDisable(GL_CULL_FACE);
	glDrawArrays(renderAs, 0, 36);
	//glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0); // Draw sky box
	glEnable(GL_CULL_FACE);
	glBindVertexArray(0); // Unbind vertex array object
	return true;
}

bool SceneObjects::DrawGrid() {
	glUseProgram(shaderProgram);
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	//glm::mat4 grid = glm::scale(groupMatrix, glm::vec3(1.0f, 1.0f, 1.0f)); probably uneeded
	//glm::mat4 gridWorld =groupMatrix ;

	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.61, 0.610, 0.610)));
	glBindVertexArray(gridVao);
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &groupMatrix[0][0]);
	glDrawArrays(GL_LINES, 0, gridCount);
	glBindVertexArray(0);
	return true;
}

bool SceneObjects::DrawCoord() {
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

	glBindVertexArray(cubeVao);
	glm::mat4 cubeParent;
	glm::mat4 cubeRotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 cubeTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	glm::mat4 cubeScale = glm::scale(glm::mat4(1.0f), glm::vec3(.99f, .15f, .15f));
	cubeParent = groupMatrix*cubeTranslate * cubeRotate;

	glm::mat4 partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.043f, .0f, .0f));
	glm::mat4 partMatrix = partTranslate * cubeScale * partRo;

	glm::mat4 worldMatrix = cubeParent * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.0, 0.0, 1.0)));//blue (x)
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//child z 
	glm::mat4 cubeChild;
	glm::mat4 cubeChildRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)90), glm::vec3(.0f, 1.0f, .0f));
	glm::mat4 cubeChildTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .043f));
	glm::mat4 cubeChildScale = glm::scale(glm::mat4(1.0f), glm::vec3(.99f, .15f, .15f));

	cubeChild = cubeParent * cubeChildTranslate * cubeChildRotate * cubeChildScale;
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = cubeChild * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 0.0, 0.0))); //red (z)
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//child y
	cubeChildRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)90), glm::vec3(.0f, 0.0f, 1.0f));
	cubeChildTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.0f, .043f, .001f));
	cubeChildScale = glm::scale(glm::mat4(1.0f), glm::vec3(.99f, .15f, .15f));
	cubeChild = cubeParent * cubeChildTranslate * cubeChildRotate * cubeChildScale;

	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = cubeChild * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.0, 1.0, 0.0))); //green (y)
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	plasticTexture.bindTexture();
	plasticTexture.loadToShader();
	glBindVertexArray(0); // Unbind unit cube VAO

	return true;
}

bool SceneObjects::DrawLight(glm::vec3 position,glm::vec3 rotation,float i) {
	//glBindTexture(GL_TEXTURE_2D, plasticTexture);
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

	glm::mat4 partTranslate = glm::translate(glm::mat4(1.0f), position);
	// Unused but usable
	glm::mat4 partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), rotation);

	glm::mat4 partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.50f, .5f, .50f));
	glm::mat4 partMatrix = partTranslate * partScale* partRo;  // Part matrix for sky box
	glm::mat4 worldMatrix = groupMatrix * partMatrix;  // World matrix for sky box

	// Bind the vertex array object to be the cube VAO with reverse winding order
	glBindVertexArray(cubeVao);
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.00f, 1.0f, 1.0f))); // Sky box colour
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]); // Send to shader
	glDisable(GL_CULL_FACE);
	glDrawArrays(renderAs, 0, 36);
	glEnable(GL_CULL_FACE);
	glBindVertexArray(0); // Unbind vertex array object
	return true;
}
