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
	explicit SceneObjects(std::string objectName);

    // GETTERS/ SETTERS
    void SetAttr(glm::mat4 groupMat, int as, int shaderProg);

    // PARAMETERS
    int shaderProgram;
    int renderAs;
    int gridCount;
    std::string objectName;
	GLuint gridVao;
	GLuint cubeVao;
    irrklang::ISoundEngine* audioEngine;

    // Matrix and vertex parameters
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

    // Sound lists
    const char* MALE_CROWD_SOUNDS[2] = {
            "../src/Assets/sounds/CrowdMale1.wav",
            "../src/Assets/sounds/CrowdMale2.wav",
    };

    const char* FEMALE_CROWD_SOUNDS[2] = {
            "../src/Assets/sounds/CrowdFemale1.wav",
            "../src/Assets/sounds/CrowdFemale2.wav",
    };

    // Methods
    // Set the VAOs for the scene
	void SetVAO(GLuint cube, GLuint grid);

    // Set the Materials for the objects in the scene
	void setMaterials(Material courtTexture, Material clothTexture,
		              Material ropeTexture, Material metalTexture,
		              Material grassTexture, Material plasticTexture);
    /**
     * Draw the court
     * @return Whether the court was drawn
     */
	bool DrawCourt();

    /**
     * Draw the net
     * @return Whether the net was drawn
     */
	bool DrawNet() const;

    /**
     * Draw the cube representing the cube
     * @return Whether the skybox was drawn
     */
	bool DrawSkyBox() const;

    /**
     * Draw the cube representing the light source
     * @param pos The position of the cube
     * @param rotation The rotation of the light around the scene
     * @return Whether the light was drawn
     */
	bool DrawLight(glm::vec3 pos, glm::vec3 rotation) const;

    /**
     * Draw the various elements of the scene
     * @param drawSkyBox Whether to draw the skybox. Don't want to draw in the shadow pass, because that will cast shadows on the scene
     */
	void DrawScene(bool drawSkyBox);

    /** Play a crowd sound
    @param isMale: Whether the sound should be male or female
    */
    void playCrowdSound(bool isMale);
};
