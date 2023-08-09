#include "SceneObjects.h"


SceneObjects::SceneObjects(int cube, glm::mat4 worldMat) : cubeVao(cube), groupMatrix(worldMat) {}

SceneObjects::SceneObjects(int cube, std::string name) : cubeVao(cube), objectName(name) {
	Translate = glm::vec3(0.0f, 0.0f, 0.0f);
}

SceneObjects::SceneObjects(std::string name) : objectName(name) {
	Translate = glm::vec3(0.0f, 0.0f, 0.0f);
}

void SceneObjects::SetAttr(glm::mat4 groupMat, int as, int shaderProg) {
	groupMatrix = groupMat;
	renderAs = as;
	shaderProgram = shaderProg;
}

void SceneObjects::SetVAO(int cube, int grid) {
	cubeVao = cube;
	gridVao = grid;
}

void SceneObjects::setMaterials(Material court, Material cloth,
	                            Material rope, Material metal,
	                            Material grass, Material plastic) {
		courtTexture =  court;
		ropeTexture   = rope ;
		metalTexture =  metal;
		clothTexture =  cloth;
		grassTexture =  grass;
		plasticTexture = plastic;
}

void SceneObjects::InitGrid() {
	gridCount = 0;

	for (int i = -100; i <= 100; i += 2) { //needs to be 100 x 100
		float f = (float) i / 100.0f;

        //this is for x z grid
		lineArray[gridCount + 0] = glm::vec3(f, .0f, -1.0f);
		lineArray[gridCount + 1] = glm::vec3(0.61f, .61f, .61f);//colour
		lineArray[gridCount + 2] = glm::vec3(f, .0f, 1.0f);
		lineArray[gridCount + 3] = glm::vec3(0.61f, .61f, .61f);//colour

		lineArray[gridCount + 4] = glm::vec3(-1.0f, 0.0f, f);
		lineArray[gridCount + 5] = glm::vec3(0.61f, .61f, .61f);//colour
		lineArray[gridCount + 6] = glm::vec3(1.0f, 0.0f, f);
		lineArray[gridCount + 7] = glm::vec3(0.61f, .61f, .61f);

        gridCount += 8; //400 for first 50
	}
}

void SceneObjects::DrawScene(bool drawSkyBox) {
	bool check = DrawCourt();
	if (!check) printf("Draw Court failed");

	// Make sure not to use the skybox itself when rendering shadows
	if (drawSkyBox)
	{
		check = DrawSkyBox();
		if (!check) printf("Draw SkyBox failed");
	}

	//check = DrawGrid();
	//if (!check) printf("Draw Grid failed");

	//check = DrawCoord();
	//if (!check) printf("Draw Coord failed");
}

bool SceneObjects::DrawCourt() {
	//set as texture shader and bind texture
	courtTexture.bindTexture();
	courtTexture.loadToShader();

	//glActiveTexture(GL_TEXTURE0);
	//GLuint textureLocation = glGetUniformLocation(shaderProgram, "textureSampler");
	//glBindTexture(GL_TEXTURE_2D, courtTexture);
	//glUniform1i(textureLocation, 0);

    // Get uniform locations
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 0.0, .0)));//red (x)

    // Bind cube VAO
	glBindVertexArray(cubeVao);

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

    // Unbind VAO
	glBindVertexArray(0);

    bool check = DrawNet();
    if (!check) printf("Draw DrawNet failed");

	return true;
}

bool SceneObjects::DrawNet() {
	// Bind metal texture when drawing poles
	metalTexture.bindTexture();
	metalTexture.loadToShader();

    // Get uniform locations
	GLint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	GLint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

    // Bind VAO
	glBindVertexArray(cubeVao);

    // Set colour of net poles to grey
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.6f, 0.6f, 0.6f)));
		
	// Rotate net poles
	glm::mat4 partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, 1.0f));

	// Translate one net pole
	glm::mat4 partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0375f, -.34f));

	// Scale net poles
	glm::mat4 partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.25f, .75f, .25f));

	// Set part matrix based on translation, scaling, and rotation. Set world matrix based on part and group
	glm::mat4 partMatrix = partTranslate * partScale * partRo;
	glm::mat4 worldMatrix = groupMatrix * partMatrix;

	// Send geometry to shader
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Positive Z net
	// Translate only the new pole. Can reuse scale and translation matrices from first pole
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0375f, .34f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = groupMatrix * partMatrix;

	// Draw again
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Bind rope texture when drawing net
	ropeTexture.bindTexture();
	ropeTexture.loadToShader();

    // Set net colour to white
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

	// Loop to draw net. i is the translation offset
	for (float i = .0; i <= .07f; i += .01f) {
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.03f, .03f, 7.0f));  // Scale to beautiful net dimensions
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.005, i, -.0f));  // Translate with that i offset
		partMatrix = partTranslate * partScale; // Set part matrix
		worldMatrix = groupMatrix * partMatrix; // Set world matrix

        // Send geometry to shader
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Same as above, but net offset is along Z axis this time
	for (float i = -.34; i <= .34f; i += .01f) {
		// The scale is also different to account for the new direction
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.03f, .7f, .03f));
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, .04, i));
		partMatrix = partTranslate * partScale;
		worldMatrix = groupMatrix * partMatrix;

        // Send geometry to shader
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Bind the clothe texture when drawing the top bar of the net
	clothTexture.bindTexture();
	clothTexture.loadToShader();

    // Set the color to white
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

    // Set transformations
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.003f, .071f, .0f));
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.1f, .1f, 6.6f));

    partMatrix = partTranslate * partScale * partRo;
	worldMatrix = groupMatrix * partMatrix;

    // Send geometry to shader
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Unbind the vertex array object
	glBindVertexArray(0);

	return true;
}

bool SceneObjects::DrawSkyBox() {
    // Bind textures
	skyTexture.bindTexture();
	skyTexture.loadToShader();

    // Get uniform locations
	GLint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	GLint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

    // Set color to a kind of blue
    glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.008f, 0.8f, 1.0f)));

	float skyBoxY = 160.0f; // Change this to make sky box closer to court

    // Set transformations
	glm::mat4 partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 partScale = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, skyBoxY, 50.0f));
    //glm::mat4 partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(0.0f, 1.0f, 1.0f)); // Unused but usable

	glm::mat4 partMatrix = partTranslate * partScale;  // Part matrix for sky box
	glm::mat4 worldMatrix = groupMatrix * partMatrix;  // World matrix for sky box

	// Bind the VAO
	glBindVertexArray(cubeVao);

    // Send geometry to shader
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);

    // Simulate having a cube VAO with reverse winding order by temporarily disabling back face culling
	glDisable(GL_CULL_FACE);
	glDrawArrays(renderAs, 0, 36);
	glEnable(GL_CULL_FACE);

    // Unbind VAO
	glBindVertexArray(0);

	return true;
}

bool SceneObjects::DrawGrid() {
    // Set uniform locations
	GLint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	GLint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

    // Bind the grid VAO
    glBindVertexArray(gridVao);

    // Set color to grey
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.61f, 0.61f, 0.61f)));

    // Send geometry to shader
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &groupMatrix[0][0]);
	glDrawArrays(GL_LINES, 0, gridCount);

    // Unbind grid VAO
    glBindVertexArray(0);

	return true;
}

bool SceneObjects::DrawCoord() {
    // Get uniform locations
	GLint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	GLint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

    // Bind cube VAO
	glBindVertexArray(cubeVao);

    // Set up transformations for the X axis
	glm::mat4 cubeParent;
	glm::mat4 cubeRotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 cubeTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	glm::mat4 cubeScale = glm::scale(glm::mat4(1.0f), glm::vec3(.99f, .15f, .15f));
	cubeParent = groupMatrix * cubeTranslate * cubeRotate;

	glm::mat4 partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.043f, .0f, .0f));
	glm::mat4 partMatrix = partTranslate * cubeScale * partRo;

	glm::mat4 worldMatrix = cubeParent * partMatrix;

    // Set color to blue
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.0, 0.0, 1.0)));

    // Send geometry to shader
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Set up transformations for Z axis
	glm::mat4 cubeChild;
	glm::mat4 cubeChildRotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(.0f, 1.0f, .0f));
	glm::mat4 cubeChildTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .043f));
	glm::mat4 cubeChildScale = glm::scale(glm::mat4(1.0f), glm::vec3(.99f, .15f, .15f));

	cubeChild = cubeParent * cubeChildTranslate * cubeChildRotate * cubeChildScale;
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = cubeChild * partMatrix;

    // Set color to red
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));

    // Send geometry to shader
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Set up transformations for Y axis
	cubeChildRotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(.0f, 0.0f, 1.0f));
	cubeChildTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.0f, .043f, .001f));
	cubeChildScale = glm::scale(glm::mat4(1.0f), glm::vec3(.99f, .15f, .15f));
	cubeChild = cubeParent * cubeChildTranslate * cubeChildRotate * cubeChildScale;

	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = cubeChild * partMatrix;

    // Set color to green
	glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.0, 1.0, 0.0)));

    // Send geometry to shader
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

    // Bind texture
	plasticTexture.bindTexture();
	plasticTexture.loadToShader();

    // Unbind VAO
	glBindVertexArray(0);

	return true;
}

bool SceneObjects::DrawLight(glm::vec3 position, glm::vec3 rotation, float i) {
	// Get uniform locations
	GLint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	GLint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

    // Set color
    glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.00f, 1.0f, 1.0f))); // Sky box colour

    // Set up transformations
	glm::mat4 partTranslate = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), rotation);  // Unused but usable
	glm::mat4 partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.50f, .5f, .50f));

    glm::mat4 partMatrix = partTranslate * partScale * partRo;  // Part matrix for light cube
	glm::mat4 worldMatrix = groupMatrix * partMatrix;  // World matrix for light cube

	// Bind the vertex array object to be the cube VAO
	glBindVertexArray(cubeVao);

    // SEnd geometry to shader
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);

    // Simulate a cube with reverse winding order by disabling backface culling temporarily
	glDisable(GL_CULL_FACE);
	glDrawArrays(renderAs, 0, 36);
	glEnable(GL_CULL_FACE);

    // Unbind VAO
	glBindVertexArray(0);

	return true;
}

void SceneObjects::playCrowdSound(bool isMale) {
    // Get a random number between 1 and 2 for which ball sound to play.
    // Makes it sound more organic to have random different sounds
    // Code from https://stackoverflow.com/a/5891824
    int whichSound = (rand() % 2);

    if (isMale)
    {
        audioEngine->play2D(MALE_CROWD_SOUNDS[whichSound]);

        // This is how we would apply sound effects like reverb, in case we wanted to
        /*
        irrklang::ISound* sound = audioEngine->play2D(MALE_CROWD_SOUNDS[whichSound], false, false, true, irrklang::ESM_AUTO_DETECT, true);
        irrklang::ISoundEffectControl* fx = sound->getSoundEffectControl();
        fx->enableWavesReverbSoundEffect(0.0f, 0.0f, 1000.0f, 0.001f);

        // Since we're using sound effects, must manually drop the sounds to avoid memory leaks
        sound->drop();
         */
    } else
    {
        audioEngine->play2D(FEMALE_CROWD_SOUNDS[whichSound]);

        // This is how we would apply sound effects like reverb, in case we wanted to
        /*
        irrklang::ISound* sound = audioEngine->play2D(FEMALE_CROWD_SOUNDS[whichSound], false, false, true, irrklang::ESM_AUTO_DETECT, true);
        irrklang::ISoundEffectControl* fx = sound->getSoundEffectControl();
        fx->enableWavesReverbSoundEffect(0.0f, 0.0f, 1000.0f, 0.001f);

        // Since we're using sound effects, must manually drop the sounds to avoid memory leaks
        sound->drop();
         */
    }
}
