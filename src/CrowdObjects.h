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
	CrowdObjects() = default;

	GLuint vaos[2];
	int sphereIndexCount;
	glm::mat4 groupMatrix;
	Material skinMaterial;
	Material clothMaterial;

	glm::vec3 crowdRotation; //idea is .x is for body movement .y is for arms ..... z for up down movement?? -> see later
	bool reverse;
	bool reverse2;
	bool reverse3;

	int shaderProgram;
	int renderAs;
	glm::vec3 position;

	void initializeCrowdRows(int rows);
	void test(int rows);
	void drawSingle(glm::vec3 position,glm::vec3 armrotate, int crowdSize, int crowdPosition);
	void loadObj();
	void walker();
	glm::vec3 animateCrowd();
	int job[2] = { 3,2 };


private:
	float fingerFlex;
	float jumpFlex;
	float turnFlex;

	int rows;
	int crowdCount = 0;
	bool initialized = false;
	glm::vec3* col;


	const KeyFrame path[2] = {
KeyFrame(glm::vec3(0.4, 0.02, -.4), glm::vec3(0.0), 0.0), // Initial key frame
KeyFrame(glm::vec3(-0.84, 0.02, -0.40), glm::vec3(0.0), 53.5), // Start moving for ball
//KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(0.0), 3.0), // Hit ball
//KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(0.0), 8.0), // Start moving away from ball
//KeyFrame(glm::vec3(0.0, 0.0, -0.005), glm::vec3(0.0), 9.0), // Be away from ball
//KeyFrame(glm::vec3(0.0, 0.0, -0.005), glm::vec3(0.0), 11.0), // Start moving back
//KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(0.0), 12.0), // Be in position
//KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(0.0), 39.0), // Start moving away from ball
//KeyFrame(glm::vec3(0.0, 0.0, -0.005), glm::vec3(0.0), 40.0), // Be away from ball
	};

	int keyFrameTrackerWalker = 0;



};

