#pragma once

// Third party includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "irrKlang.h"

// Source code includes
#include "Material.h"

class Ball
{
private:
	GLuint VAO;
	GLuint shaderProgram;
	glm::mat4 groupMatrix;
	glm::vec3 translationModel;
	glm::vec3 initialPosition;
	int renderAS;
	int sphereVertCount;
	Material ballTexture;
    glm:: vec3 position;
	irrklang::ISoundEngine* audioEngine;
    const char* BALL_SOUNDS[3] = {
            "../src/Assets/sounds/Ball1.wav",
            "../src/Assets/sounds/Ball2.wav",
            "../src/Assets/sounds/Ball3.wav"
    };
	const char* BOUNCE_SOUND[4] = {
		"../src/Assets/sounds/TennisBounceA.wav",
		"../src/Assets/sounds/TennisBounceB.wav",
		"../src/Assets/sounds/TennisBounceC.wav",
		"../src/Assets/sounds/TennisBounceD.wav"
	};
	const char* BIG_CROWD_SOUND = "../src/Assets/sounds/BigCrowd.wav";

public:
    // CONSTRUCTOR
	Ball();

    // GETTERS/ SETTERS
	void setGroupMatrix(glm::mat4 groupMat);
	void setShaderProgram(GLuint shaderProg);
	void setVAO(GLuint vao);
	void setTranslationModel(glm::vec3 trans);
	void setRenderAs(int as);
	void setSphereVertCount(int count);
	void setMaterial(Material material);
    void setSoundEngine(irrklang::ISoundEngine* engine);

    // METHODS

    // Draws the Ball model's geometry to the scene
	void drawBall();

    // Plays a random ball sound from the set of possible ball sounds BALL_SOUNDS
    void playSound();

    // Plays a random ball bounce sound from BOUNCE_SOUND
	void playBounceSound();
};

