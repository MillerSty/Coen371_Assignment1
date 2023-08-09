#pragma once

// System includes
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

// Third party includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <irrKlang.h>

// Source code includes
#include "Material.h"

class SceneObjects
{
public:
    // Constructors
	SceneObjects() = default;
	SceneObjects(int cubeVao,
		glm::mat4 worldMatrix);
	SceneObjects(int cubeVao, std::string objectName);
	SceneObjects(std::string objectName);
	void SetAttr(glm::mat4 groupMat, int as, int shaderProg);

    int shaderProgram;
    int renderAs;
    int gridCount;

    std::string objectName;

    // VAO parameters
	int gridVao;
	int cubeVao;	
	//int reverseCubeVao;
	int sphereVao;
	int sphereVertCount;

    // Sound engine parameter
    irrklang::ISoundEngine* audioEngine;

    // Sounds parameters
    const char* MALE_CROWD_SOUNDS[2] = {
            "../src/Assets/sounds/CrowdMale1.wav",
            "../src/Assets/sounds/CrowdMale2.wav",
    };

    const char* FEMALE_CROWD_SOUNDS[2] = {
            "../src/Assets/sounds/CrowdFemale1.wav",
            "../src/Assets/sounds/CrowdFemale2.wav",
    };

    // Matrix and Vector parameters
	glm::mat4 partParent;
	glm::mat4 groupMatrix;
	glm::mat4 rotationWMatrix;
	glm::vec3 Translate;
	glm::vec3 lineArray[808];
    glm::vec3 position;
    glm::vec3 colour[3];

    // Material parameters
	Material courtTexture;
	Material clothTexture;
	Material ropeTexture;
	Material metalTexture;
	Material grassTexture;
	Material plasticTexture;
	Material skyTexture;

    // Methods
	//void SetVAO(int cube, int reversecube, int grid);
	void SetVAO(int cube, int grid);
	void setMaterials(Material courtTexture, Material clothTexture,
		              Material ropeTexture, Material metalTexture,
		              Material grassTexture, Material plasticTexture);
	// Initialize the grid
    void InitGrid();
    // Draw the grid
    bool DrawGrid();
    // Draw the court
	bool DrawCourt();
    // Draw the net
	bool DrawNet();
    // Draw the skybox
	bool DrawSkyBox();
    // Draw the XYZ coordinate axes at the origin
	bool DrawCoord();
    // Draw the cube representing the cube
	bool DrawLight(glm::vec3 position, glm::vec3 rotation, float i);
    // Draw the various elements of the scene
	void DrawScene(bool drawSkyBox);
    /** Play a crowd sound
    @param isMale: Whether the sound should be male or female
    */
    void playCrowdSound(bool isMale);
};
