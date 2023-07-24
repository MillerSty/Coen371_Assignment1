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
class SceneObjects
{
public:
	SceneObjects(int cubeVao,
		glm::mat4 worldMatrix);
	SceneObjects(int cubeVao, std::string objectName);
	SceneObjects(std::string objectName);
	//void setTranslation(glm::vec3 TranslateRandom, glm::vec3 TranslateModel);
	void SetAttr(glm::mat4 groupMatrix, int renderAs, int shaderProgram);
	void setTextures(GLuint court, GLuint rope, GLuint metal, GLuint cloth);
	
	int gridVao;
	int cubeVao;
	int reverseCubeVao;
	glm::mat4 partParent;
	//glm::mat4 partMatrix;
	glm::mat4 groupMatrix;
	glm::mat4 rotationWMatrix;
	//glm::vec3 TranslateRandom;
	//glm::vec3 TranslateModel;
	//attributes for grid
	glm::vec3 Translate;
	glm::vec3 lineArray[808];
	int gridCount;
	GLuint courtTexture;
	GLuint clothTexture;
	GLuint ropeTexture;
	GLuint metalTexture;
	//atrributes for court

	//attributes for net
	int shaderProgram;
	int textureProgram;
	int renderAs;
	glm::vec3 position;
	glm::vec3 colour[3];
	std::string objectName;

	void SetVAO(int cube, int reversecube, int grid);
	//methods like 
	void InitGrid();
	//bool DrawArm();
	bool DrawCourt();
	bool DrawNet();
	bool DrawSkyBox();
	bool DrawGrid();
	bool DrawCoord();
	void DrawScene();
};

