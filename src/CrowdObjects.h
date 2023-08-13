#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include "../src/Material.h"
class CrowdObjects
{
public:
	CrowdObjects() = default;

	GLuint vaos[2];
	int sphereIndexCount;
	glm::mat4 groupMatrix;
	Material skinMaterial;
	Material clothMaterial;

	int shaderProgram;
	int renderAs;
	glm::vec3 position;
	void drawCrowd();
	void initializeCrowdRows(int rows);
	void test(int rows);
	void drawSingle(glm::vec3 position,glm::vec3 armrotate, int crowdSize, int crowdPosition);

private:
	int rows;
	int crowdCount = 0;
	bool initialized = false;
	glm::vec3* col;
};

