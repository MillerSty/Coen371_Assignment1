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

	int vaos[2];
	int sphereIndexCount;
	glm::mat4 groupMatrix;
	Material skinMaterial;
	Material clothMaterial;

	int shaderProgram;
	int renderAs;
	glm::vec3 position;
	void drawCrowd();
	void test(int rows);
	void drawSingle(glm::vec3 position,glm::vec3 armrotate);
	void loadObj();
};

