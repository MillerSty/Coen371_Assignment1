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
class SceneObjects
{
public:
	SceneObjects() = default;
	SceneObjects(int cubeVao,
		glm::mat4 worldMatrix);
	SceneObjects(int cubeVao, std::string objectName);
	SceneObjects(std::string objectName);
	void SetAttr(glm::mat4 groupMatrix, int renderAs, int shaderProgram);
	//void setTextures(GLuint court, GLuint rope, GLuint metal, GLuint cloth, GLuint grass);
	
	int skyVao;
	int gridVao;
	int cubeVao;	
	//int reverseCubeVao;
	int sphereVao;
	int sphereVertCount;

	glm::mat4 partParent;
	glm::mat4 groupMatrix;
	glm::mat4 rotationWMatrix;
	glm::vec3 Translate;
	glm::vec3 lineArray[808];
	int gridCount;
	Material courtTexture;
	Material clothTexture;
	Material ropeTexture;
	Material metalTexture;
	Material ballTexture;
	Material grassTexture;
	Material plasticTexture;
	Material skyTexture;
	//atrributes for court

	//attributes for net
	int shaderProgram;
	int renderAs;
	glm::vec3 position;
	glm::vec3 colour[3];
	std::string objectName;

	//void SetVAO(int cube, int reversecube, int grid);
	void SetVAO(int cube, int grid);
	void setMaterials(Material courtTexture,
		Material clothTexture,
		Material ropeTexture,
		Material metalTexture,
		Material ballTexture,
		Material plasticTexture);
	//methods like 
	
	void InitGrid();
	bool DrawBall();
	bool DrawCourt();
	bool DrawNet();
	bool DrawSkyBox();
	bool DrawGrid();
	bool DrawCoord();
	bool DrawLight(glm::vec3 position,glm::vec3 rotation,float i);
	void DrawScene(bool drawSkyBox);
};

