#include "SceneObjects.h"

SceneObjects::SceneObjects(std::string name) : objectName(name) {
	Translate = glm::vec3(0.0f, 0.0f, 0.0f);
}

void SceneObjects::SetAttr(glm::mat4 groupMat, int as, int shaderProg) {
	groupMatrix = groupMat;
	renderAs = as;
	shaderProgram = shaderProg;
}

void SceneObjects::SetVAO(GLuint cube, GLuint grid) {
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

void SceneObjects::DrawScene(bool drawSkyBox) {
	bool check = DrawCourt();
	if (!check) printf("Draw Court failed");

	// Make sure not to use the skybox itself when rendering shadows
	if (drawSkyBox)
	{
		check = DrawSkyBox();
		if (!check) printf("Draw SkyBox failed");
	}
}

bool SceneObjects::DrawCourt() {
	// Bind the court shader
	courtTexture.bindTexture();
	courtTexture.loadToShader();

    // Get uniform locations
	GLint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	GLint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

    // Set the color
    glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 0.0, .0)));//red (x)

    // Bind cube VAO
	glBindVertexArray(cubeVao);

	glm::mat4 partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.2f, .2f, .2f));

    // Draw the white lines of the court and the green pitch
	for (int pitchx = -78; pitchx < 78; pitchx += 2) {
		for (int pitchz = -36; pitchz < 36; pitchz += 2) {

			glm::mat4 partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3((float) pitchx / 100.0f, -.01, (float) pitchz / 100.0f + 0.01f));
			glm::mat4 partMatrix = partTranslate * partScale;
			glm::mat4 worldMatrix =  groupMatrix* partMatrix;

            if (
                    (abs(pitchx) == 74 && abs(pitchz) < 31) ||
                    (abs(pitchz) == 32 && abs(pitchx) < 76) ||
                    (abs(pitchx) == 54 && abs(pitchz) <= 30) ||
                    (abs(pitchx) < 54 && abs(pitchz) == 0)
                    )
            {
                // Set the back horizontal line, length lines, inside horizontal line, and the inside horizontal line white
                glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
            }

            // Set pitch itself to green
			else
				glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.0, 0.522, .40)));

            // Draw geometry
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

    // Unbind VAO
	glBindVertexArray(0);

    // Draw the net too
    bool check = DrawNet();
    if (!check) printf("Draw DrawNet failed");

	return true;
}

bool SceneObjects::DrawNet() const {
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
	glm::mat4 partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, 1.0f, 1.0f));

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
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, 1.0f, 1.0f));
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

bool SceneObjects::DrawSkyBox() const {
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

bool SceneObjects::DrawLight(glm::vec3 pos, glm::vec3 rotation) const {
	// Get uniform locations
	GLint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	GLint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

    // Set color
    glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.00f, 1.0f, 1.0f))); // Sky box colour

    // Set up transformations
	glm::mat4 partTranslate = glm::translate(glm::mat4(1.0f), pos);
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

    // Play a male or female crowd sound, depending on which side scores
    if (isMale)
        audioEngine->play2D(MALE_CROWD_SOUNDS[whichSound]);
    else
        audioEngine->play2D(FEMALE_CROWD_SOUNDS[whichSound]);
}
