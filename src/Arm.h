#pragma once

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Material.h"

class Arm
{
public:
    // Constructor
	Arm();

    // GETTERS/ SETTERS

    // Set attributes for the Arm
	void SetAttr(glm::mat4 groupMatrix, int renderAs, int shaderProgram);

    // Set Arm rotation
    void setRotation(float rotate) { armRotate = rotate; }

    // Set the model translation for the Arm
    void setTranslateModel(glm::vec3 TranslateModel) { this->TranslateModel = TranslateModel; }

    // Flex the Arms fingers automatically
    bool flexFingers();

    // Initialize the Arm
	void InitArm(glm::vec3 position, GLuint VAO, Material skinMaterial, Material clothMaterial);

    // PARAMETERS
	bool reverse;
	float fingerFlex;
	float armRotate, elbowRotate, wristRotate, fingerRotate;

    GLuint cubeVao;

    glm::mat4 partParent;
	glm::mat4 partMatrix;
	glm::mat4 groupMatrix;
	glm::vec3 TranslateRandom;
    glm::vec3 position;
	glm::vec3 TranslateModel;

    Material skinMaterial;
	Material clothMaterial;

    int shaderProgram;
    int renderAs;

	std::string letterName;

	// METHODS
    // Draw the arm
	bool DrawArm();
};
