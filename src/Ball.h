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
	glm::mat4 worldMatrix;
	glm::vec3 translationModel;
	glm::vec3 initialPosition;
	int renderAS;
	int sphereVertCount;
	Material ballTexture;
	float velocity;
    glm:: vec3 position;
    irrklang::ISoundEngine* audioEngine;
    const char* BALL_SOUNDS[3] = {
            "../src/Assets/sounds/Ball1.wav",
            "../src/Assets/sounds/Ball2.wav",
            "../src/Assets/sounds/Ball3.wav"
    };
	const char* BIG_CROWD_SOUND = "../src/Assets/sounds/BigCrowd.wav";

public:
    // CONSTRUCTOR
	Ball();

    // GETTERS/ SETTERS
	void setPosition(glm::vec3 pos) { position = pos; }
    glm::vec3 getPosition() { return position; }
	glm::vec3 getSomething() { return translationModel + position; }
	void setGroupMatrix(glm::mat4 groupMat);
	glm::mat4 getGroupMatrix();
	void setWorldMatrix(glm::mat4 worldMat);
	glm::mat4 getWorldMatrix();
	void setShaderProgram(GLuint shaderProg);
	GLuint getShaderProgram();
	void setInitialPosition(glm::vec3 postion);
	glm::vec3 getInitialPosition();
	void setVAO(GLuint vao);
	GLuint getVAO();
	glm::vec3 getTranslationModel();
	void setTranslationModel(glm::vec3 trans);
	void setRenderAs(int as);
	int getRenderAs();
	void setSphereVertCount(int count);
	int getSphereVertCount();
	void setMaterial(Material material);
    void setSoundEngine(irrklang::ISoundEngine* engine);
    irrklang::ISoundEngine* getSoundEngine();

    // METHODS

    // Resets translations on the Ball model
	void resetModel();

    // Draws the Ball model's geometry to the scene
	void drawBall();

    // Plays a random ball sound from the set of possible ball sounds BALL_SOUNDS
    void playSound();
	void playBigCrowdSound();
};

