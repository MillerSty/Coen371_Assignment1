#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include "../src/Material.h"
#include "KeyFrame.h"

class CrowdObjects
{
public:
    // CONSTRUCTOR
	CrowdObjects() = default;

    // PARAMETERS
	GLuint vaos[2];
	int sphereIndexCount;
	bool reverse;
	bool reverse2;
	bool reverse3;
	int shaderProgram;
	int renderAs;
	int job[2] = { 3,2 };
	glm::vec3 position;
    glm::mat4 groupMatrix;
    glm::vec3 crowdRotation; //idea is .x is for body movement .y is for arms ..... z for up down movement?? -> see later
    Material skinMaterial;

    // METHODS
    // Initialize the crowd rows by filling up the col parameter
	void initializeCrowdRows(int rows);

    // Draws the crowd
	void drawCrowd(int rows);

    // Draw a single person
	void drawSingle(glm::vec3 pos, glm::vec3 armrotate, int crowdSize, int crowdPosition);

    // Draw the walking people
	void walker();

    // Animate the crowd
	glm::vec3 animateCrowd();

private:
    // PRIVATE PARAMETERS
	float fingerFlex;
	float jumpFlex;
	float turnFlex;
	int keyFrameTrackerWalker = 0;
	int crowdCount = 0;
	bool initialized = false;
	glm::vec3* col;

    // Keyframes for walking people
	const KeyFrame path[2] = {
KeyFrame(glm::vec3(0.4, 0.02, -.4), glm::vec3(0.0), 0.0), 
KeyFrame(glm::vec3(-0.84, 0.02, -0.40), glm::vec3(0.0), 53.5), 
	};
};
