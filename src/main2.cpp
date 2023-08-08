// Comp 371 - Assignment 2
// System includes
#pragma once
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

// Dependency includes
#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Source code includes
#include "Arm.h"
#include "Racket.h"
#include "SceneObjects.h"
#include "Material.h"
#include "Letters.h"
#include "SpotLight.h"
//#include "Camera.h"
using namespace glm;

// Set the shader paths
const char* vertex = "../src/shaders/unifiedVertex.glsl";
const char* fragment = "../src/shaders/unifiedFragment.glsl";
void updateCamera(Arm arm, vec3 Translation);
void doRandomColour(Letters& letterJ, Letters& letterM, Letters& letterI, Letters& letterL);

// Declare some functions for later use
int compileAndLinkShaders(const char* vertex, const char* fragment);
GLuint loadTexture(const char* filename);
void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                const GLchar* message, const void* userParam);
void setProjectionMatrix(int shaderProgram, glm::mat4 projectionMatrix);
void setViewMatrix(int shaderProgram, glm::mat4 viewMatrix);
void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCursorPostionCallback(GLFWwindow* window, double xPos, double yPos);
//void windowSizeCallback(GLFWwindow* window, int width, int height);
bool loadOBJ2(const char* path, std::vector<int>& vertexIndices, std::vector<glm::vec3>& temp_vertices,
	          std::vector<glm::vec3>& out_normals, std::vector<glm::vec2>& out_uvs);

/**
Create a vertex array object for the grid
@param vertexArray: A pointer to the vertex array to use
@param arraySize: How many entries are in the vertex array
@return The VAO
*/
int createVertexArrayObject(const glm::vec3* vertexArray, int arraySize)
{    // Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, arraySize, vertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return vertexArrayObject;
}

/**
A struct to contain the position, normal, and UV coordinates for a vertex
*/
struct TexturedNormaledVertex
{
	TexturedNormaledVertex(glm::vec3 _position, glm::vec3 _normals, glm::vec2 _uv) : position(_position), normals(_normals), uv(_uv) {}

	glm::vec3 position;
	glm::vec3 normals;
	glm::vec2 uv;
};
void updateLight(glm::vec3 newPosition,glm::vec3 newFocus,SceneObjects SceneObj,GLuint shaderProgram,float i,bool applyShadow,bool follow) {
	// light parameters
	//glm::vec3 lightPosition(-.30f, .30f, .0f); // the location of the light in 3D space
	glm::vec3 lightFocus(newPosition.x+.01f, newPosition.y-3.0f* newPosition.y, newPosition.z+.01f); 
     // the point in 3D space the light "looks" at
	glm::vec3 lightDirection =normalize(lightFocus - newPosition);

	GLint lightPositionLoc = glGetUniformLocation(shaderProgram, "spotLights[0].lightPosition");
	GLint lightDirectionLoc = glGetUniformLocation(shaderProgram, "spotLights[0].lightDirection");
	//GLint lightViewProjMatrixLoc = glGetUniformLocation(shaderProgram, "lightViewProjMatrix");
	GLint viewMatrix = glGetUniformLocation(shaderProgram, "viewMatrix");
	// Set light position on scene shader
	glUniform3fv(lightPositionLoc, 1, &newPosition[0]);
	// Set light direction on scene shader
	glUniform3fv(lightDirectionLoc, 1, &lightDirection[0]);



	//splightPositio = vec3(-.0f, .50f, 3.0f); blue light location
	//this is just for location not focus
	////vec3 lightPos = vec3(newPosition.x + .01f, newPosition.y - .50f * newPosition.y, 1.0f);
	//lightFocus = vec3(newPosition.x, newPosition.y, -1.0f);
	//// the point in 3D space the light "looks" at
	//lightDirection = normalize(lightFocus - vec3(-.0f, .0f, 3.0f));
	//
	//lightPositionLoc = glGetUniformLocation(shaderProgram, "spotLights[1].lightPosition");
	//lightDirectionLoc = glGetUniformLocation(shaderProgram, "spotLights[1].lightDirection");
	////GLint lightViewProjMatrixLoc = glGetUniformLocation(shaderProgram, "lightViewProjMatrix");
	////GLint viewMatrix = glGetUniformLocation(shaderProgram, "viewMatrix");
	//// Set light position on scene shader
	////glUniform3fv(lightPositionLoc, 1, &lightPos[0]);
	//// Set light direction on scene shader
	//glUniform3fv(lightDirectionLoc, 1, &lightDirection[0]);

	//float lightNearPlane = 0.01f;
	//float lightFarPlane = 180.0f;

	//glm::mat4 lightProjectionMatrix = glm::ortho(-1.5f, 1.5f, -1.5f, 1.5f, lightNearPlane, lightFarPlane);
	//glm::mat4 splViewMatrix = glm::lookAt(newPosition, lightFocus, glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 lightSpaceMatrix = lightProjectionMatrix * splViewMatrix;

	//glUniformMatrix4fv(lightViewProjMatrixLoc, 1, GL_FALSE, &lightSpaceMatrix[0][0]);

	if (!applyShadow) {
		SceneObj.DrawLight(newPosition, glm::vec3(0.0f, 1.0f, 0.0f), i);
	}


	if (follow) {
		glm::mat4 InitviewMatrix = glm::lookAt(vec3(newPosition.x,  newPosition.y+.3,  newPosition.z),  // eye
			vec3(.01f,.01f,.01f),  // center
			vec3(0,1,0));// 
		setViewMatrix(shaderProgram, InitviewMatrix);

		//glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, &InitviewMatrix[0][0]);
	
	}
}
// Textured Cube model
TexturedNormaledVertex texturedCubeVertexArray[] = {
	// LEFT
	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(-1, 0, 0), glm::vec2(0.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(-1, 0, 0), glm::vec2(0.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1, 0, 0), glm::vec2(1.0f, 1.0f)),

	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(-1, 0, 0), glm::vec2(0.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1, 0, 0), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(-1, 0, 0), glm::vec2(1.0f, 0.0f)),

	// FAR
	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(0, 0, -1), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0, 0, -1), glm::vec2(0.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0, 0, -1), glm::vec2(0.0f, 1.0f)),

	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(0, 0, -1), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(0, 0, -1), glm::vec2(1.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0, 0, -1), glm::vec2(0.0f, 0.0f)),

	// BOTTOM
	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(0, -1, 0), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0, -1, 0), glm::vec2(0.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(0, -1, 0), glm::vec2(1.0f, 0.0f)),

	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(0, -1, 0), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(0, -1, 0), glm::vec2(0.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0, -1, 0), glm::vec2(0.0f, 0.0f)),

	// NEAR
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(0.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(0.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(1.0f, 0.0f)),

	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(0.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(1.0f, 0.0f)),

	// RIGHT
	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(1, 0, 0), glm::vec2(0.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(1, 0, 0), glm::vec2(1.0f, 0.0f)),

	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f,-0.5f), glm::vec3(1, 0, 0), glm::vec2(0.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec2(0.0f, 1.0f)),

	// TOP
	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f,-0.5f), glm::vec3(0, 1, 0), glm::vec2(1.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0, 1, 0), glm::vec2(0.0f, 0.0f)),

	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f,-0.5f), glm::vec3(0, 1, 0), glm::vec2(0.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec2(0.0f, 1.0f))
};

/**
Create a vertex array object with positions, normals, and UVs
@return The VAO
*/
int createVertexArrayObject2()
{
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedNormaledVertex), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedNormaledVertex), (void*) sizeof(glm::vec3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedNormaledVertex), (void*) (2 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return vertexArrayObject;
}

TexturedNormaledVertex texturedCubeVertexArraySky[] = {
	// LEFT
	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f,-0.5f),-1.0f* glm::vec3(-1, 0, 0), glm::vec2(0.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f, 0.5f),-1.0f* glm::vec3(-1, 0, 0), glm::vec2(0.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f, 0.5f),-1.0f* glm::vec3(-1, 0, 0), glm::vec2(1.0f, 1.0f)),
														
	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f,-0.5f),-1.0f* glm::vec3(-1, 0, 0), glm::vec2(0.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f, 0.5f),-1.0f* glm::vec3(-1, 0, 0), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f,-0.5f),-1.0f* glm::vec3(-1, 0, 0), glm::vec2(1.0f, 0.0f)),
														  
	// FAR												-1.0f
	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f,-0.5f), -1.0f* glm::vec3(0, 0, -1), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f,-0.5f),-1.0f* glm::vec3(0, 0, -1), glm::vec2(0.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f,-0.5f),-1.0f* glm::vec3(0, 0, -1), glm::vec2(0.0f, 1.0f)),
														
	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f,-0.5f), -1.0f* glm::vec3(0, 0, -1), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f,-0.5f), -1.0f* glm::vec3(0, 0, -1), glm::vec2(1.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f,-0.5f),-1.0f* glm::vec3(0, 0, -1), glm::vec2(0.0f, 0.0f)),
														  
	// BOTTOM											-1.0f*
	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f, 0.5f), -1.0f* glm::vec3(0, -1, 0), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f,-0.5f),-1.0f* glm::vec3(0, -1, 0), glm::vec2(0.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f,-0.5f), -1.0f* glm::vec3(0, -1, 0), glm::vec2(1.0f, 0.0f)),
														
	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f, 0.5f), -1.0f* glm::vec3(0, -1, 0), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f, 0.5f),-1.0f* glm::vec3(0, -1, 0), glm::vec2(0.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f,-0.5f),-1.0f* glm::vec3(0, -1, 0), glm::vec2(0.0f, 0.0f)),
														  
	// NEAR												-1.0f*
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f, 0.5f),-1.0f* glm::vec3(0, 0, 1), glm::vec2(0.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f,-0.5f, 0.5f),-1.0f* glm::vec3(0, 0, 1), glm::vec2(0.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f, 0.5f), -1.0f* glm::vec3(0, 0, 1), glm::vec2(1.0f, 0.0f)),
														  
	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f, 0.5f), -1.0f* glm::vec3(0, 0, 1), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f, 0.5f),-1.0f* glm::vec3(0, 0, 1), glm::vec2(0.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f, 0.5f), -1.0f* glm::vec3(0, 0, 1), glm::vec2(1.0f, 0.0f)),
														  
	// RIGHT											-1.0f*
	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f, 0.5f), -1.0f*glm::vec3(1, 0, 0), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f,-0.5f), -1.0f*glm::vec3(1, 0, 0), glm::vec2(0.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f,-0.5f), -1.0f*glm::vec3(1, 0, 0), glm::vec2(1.0f, 0.0f)),
														  
	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f,-0.5f), -1.0f*glm::vec3(1, 0, 0), glm::vec2(0.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f, 0.5f), -1.0f*glm::vec3(1, 0, 0), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f,-0.5f, 0.5f), -1.0f*glm::vec3(1, 0, 0), glm::vec2(0.0f, 1.0f)),
														  
	// TOP												-1.0f*
	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f, 0.5f), -1.0f*glm::vec3(0, 1, 0), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f,-0.5f), -1.0f*glm::vec3(0, 1, 0), glm::vec2(1.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f,-0.5f),-1.0f* glm::vec3(0, 1, 0), glm::vec2(0.0f, 0.0f)),
														
	TexturedNormaledVertex(glm::vec3(0.5f, 0.5f, 0.5f), -1.0f*glm::vec3(0, 1, 0), glm::vec2(1.0f, 1.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f,-0.5f),-1.0f* glm::vec3(0, 1, 0), glm::vec2(0.0f, 0.0f)),
	TexturedNormaledVertex(glm::vec3(-0.5f, 0.5f, 0.5f),-1.0f* glm::vec3(0, 1, 0), glm::vec2(0.0f, 1.0f))
};

/**
Create a vertex array object with positions, normals, and UVs
@return The VAO
*/
int createVertexArrayObjectSky()
{
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedNormaledVertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedNormaledVertex), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedNormaledVertex), (void*)(2 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return vertexArrayObject;
}


GLuint IBO;
bool follow = false;
/**
Create a index buffer object for the sphere
@param vertexIndices: The indices for the vertices of the sphere
@param vertices: The vertices of the sphere
@param normals: The normals of the sphere
@UVs: The UVs of the sphere
@return The IBO
*/
int createVertexArrayElementObject2(std::vector<int> vertexIndices, std::vector<glm::vec3> vertices, 
	                                std::vector<glm::vec3> normals, std::vector<glm::vec2> UVs)
{
	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(unsigned int), &vertexIndices.front(), GL_STATIC_DRAW);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	//int check = arraySize;
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0,    // attribute 0 matches aPos in Vertex Shader
		3,                      // size
		GL_FLOAT,               // type
		GL_FALSE,               // normalized?
		1 * sizeof(glm::vec3),  // stride - each vertex contain 2 vec3 (position, color)
		(void*)0                // array buffer offset
	);
	glEnableVertexAttribArray(0);

	//Normals VBO setup taken from lab
	GLuint normals_VBO;
	glGenBuffers(1, &normals_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	//UVs VBO setup
	GLuint uvs_VBO;
	glGenBuffers(1, &uvs_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec3), &UVs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}

// Set aspect ratio of the window
const int WIDTH = 1024, HEIGHT = 768;

// Set/ declare some variables
glm::vec3 eye(.7650f, .250f, .7650f);
glm::vec3 RESETeye(.7650f, .250f, .7650f);
glm::vec3 center(.00f, .0f, 0.0f);
glm::vec3 RESETcenter(.00f, .0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 translateWSAD(0.0f, 0.0f, 0.0f);
glm::vec3 Translate(.0f, .0f, .0f);
glm::vec3 GroupMatrixScale(1.0f, 1.0f, 1.0f);
glm::mat4 groupMatrix;
glm::mat4 rotationMatrixW = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

int renderAs = GL_TRIANGLES;
int shaderProgram;
double lastMousePosX, lastMousePosY, lastMousePosZ;
float FOV = 70, AR = (float)(WIDTH / HEIGHT), near = .01, far = 50;
float translateW = 0, translateY = 0, translateZ = 0;

// Toggles for shadows and textures
bool shouldApplyShadows = true;
bool shouldApplyTextures = true;
bool shouldApplySpLight = true;
bool shouldApplyMLight = true;

Arm jonArm;
Arm micahArm;
//Camera camera;
//bool reset view;
int camereSelect = -1;
int selectModel = 0; //we can se to 0 but then user has to toggle to before any thing
int selectJoint =0;
float lightDepth = .50f;

float lightRotation = -1;



int main(int argc, char* argv[])
{
	// Initialize GLFW and OpenGL version
	if (!glfwInit())
		return -1;

	// Scale down the unit cube vertices
	for (size_t i = 0; i < 36; i++) {
		TexturedNormaledVertex thisOne = texturedCubeVertexArray[i];
		texturedCubeVertexArray[i].position *= 0.1f;
		texturedCubeVertexArray[i].uv *= 0.1f;
	}

	// Set some GLFW window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

	// Create Window and rendering context using GLFW, resolution is 800x600
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Comp371 - Assignment 2", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glViewport(0, 0, WIDTH, HEIGHT);

	// Enable depth test with the GL_LESS function
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Enable face culling with the GL_BACK function
	glEnable(GL_CULL_FACE);
	glEnable(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Print OpenGL version
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

#if defined(__APPLE__) // NOTE Rez: Youll need to path the textures
#else
	// Enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(messageCallback, 0);
#endif

	// Load the textures
	GLuint courtTextureID	 = loadTexture("../src/Assets/clay2.jpg");
	GLuint ropeTextureID	 = loadTexture("../src/Assets/rope.jpg");
	GLuint clothTextureID	= loadTexture("../src/Assets/cloth.jpg");
	GLuint metalTextureID	= loadTexture("../src/Assets/metal.jpg");
	GLuint grassTextureID     = loadTexture("../src/Assets/grass4.jpg");
	GLuint plasticTextureID  = loadTexture("../src/Assets/plastic.jpg");
	GLuint woodTextureID       = loadTexture("../src/Assets/wood1.jpg");
	GLuint tattooTextureID = loadTexture("../src/Assets/tattoo.png");
	GLuint skyTextureID = loadTexture("../src/Assets/sky.jpg");
	GLuint cloudTextureID = loadTexture("../src/Assets/cloud.png");
	
	// Black background	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);

	// Set frame rate to refresh rate of monitor
	glfwSwapInterval(1);

	// Compile and link shaders here
	shaderProgram = compileAndLinkShaders(vertex, fragment);

	// Initialize uniform locations
	glUseProgram(shaderProgram);
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    GLint viewPositionLocation = glGetUniformLocation(shaderProgram, "viewPosition");
    GLint applyTexturesLocation = glGetUniformLocation(shaderProgram, "shouldApplyTexture");
    GLint applyShadowsLocation = glGetUniformLocation(shaderProgram, "shouldApplyShadows");
	//maybe update this for shadows
    glUniform3fv(viewPositionLocation, 1, &eye[0]);

	//Scene Jawn
	SceneObjects SceneObj("scene");
	SceneObj.InitGrid();
	SceneObjects Ball;
	
	//SceneObj.plasticTexture = plasticTextureID;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
	lastMousePosZ = lastMousePosY;

	// Initialize projection and view matrices
	glm::mat4 projectionMatrix = glm::perspective(FOV, AR, near, far);

	glm::mat4 InitviewMatrix = glm::lookAt(eye, center, up);

	setProjectionMatrix(shaderProgram, projectionMatrix);
	setViewMatrix(shaderProgram, InitviewMatrix);

    std::vector<int> vertexIndicessphere;
	std::vector<glm::vec3> verticessphere, normalssphere;
	std::vector<glm::vec2> UVscube, UVssphere;
	std::string pathSphere = "../src/Assets/mesh/unitSphere.obj";
	loadOBJ2(pathSphere.c_str(), vertexIndicessphere, verticessphere, normalssphere, UVssphere);
  
	// Scale the vertex positions of the sphere
	for (int i = 0; i < verticessphere.size(); i++) {
		verticessphere[i] = verticessphere[i] * .05f;
	}
	
	// Create VAOs
	int gridAO = createVertexArrayObject(SceneObj.lineArray, sizeof(SceneObj.lineArray));
	int unitSphereAO = createVertexArrayElementObject2(vertexIndicessphere, verticessphere, normalssphere, UVssphere);
	int unitCubeAO = createVertexArrayObject2();
	int unitCubeSkyAO = createVertexArrayObjectSky();

	jonArm.setVAO(unitCubeAO);
	jonArm.position = glm::vec3(-.20f,0.0f,0.0f);
	micahArm.setVAO(unitCubeAO);
	micahArm.position = glm::vec3(0.20f, 0.0f, 0.0f);
		//jonArm ref position:glm::vec3(-.5f, 0.0f, .2f);


	//TEXTURE DEFINITION
	vec4 generalMat;
						
						//diff spec ambient shiny
	Material courtMaterial(.2f, .20f, 1.0f, .1f, courtTextureID, shaderProgram); //court shouldnt reflect
	Material ropeMaterial(.5f, .60f, .5f, .9f, ropeTextureID, shaderProgram); // ropes are just ropes
	Material clothMaterial(.5f, .60f, .5f, .9f, clothTextureID, shaderProgram); //cloth should have a little reflection?
	Material metalMaterial(.6f, .90f, .6f, .00012f, metalTextureID, shaderProgram); //metal should shine
	Material grassMaterial(.1f, .0f, .1f, .00f, grassTextureID, shaderProgram); //just bright, thats all it needs
	Material plasticMaterial(.5f, .30f, .5f, .002f, plasticTextureID, shaderProgram); //needs to be glossy! This is our racket
	Material woodMaterial(.5f, .60f, .5f, .002f, woodTextureID, shaderProgram); //this is you matt
	Material tattooMaterial(.5f, .60f, .5f, .002f, tattooTextureID, shaderProgram); //this is you matt
	Material cloudMaterial(.5f, .60f, .5f, .002f, skyTextureID, shaderProgram); //this is you matt

	Material skyMaterial(.2f, .20f, .5f, .002f, plasticTextureID, shaderProgram); //this is you matt
	// Material plasticSkinMaterial...  we need a dull material for skin probaly a copy of the plastic texture with different diff/spec/amb

	Ball.grassTexture = grassMaterial;
	Ball.shaderProgram = shaderProgram;
	Ball.sphereVao = unitSphereAO;
	Ball.sphereVertCount=vertexIndicessphere.size();
	SceneObj.setMaterials(courtMaterial, clothMaterial, ropeMaterial, metalMaterial, grassMaterial, plasticMaterial);
	SceneObj.skyVao = unitCubeSkyAO;
	SceneObj.skyTexture = cloudMaterial;
	Racket racket(unitCubeAO, "racket");
	racket.setBall(Ball);
	racket.jawnAngle = 0;
	racket.plasticMaterial = metalMaterial;

	jonArm.tattoo = tattooMaterial;
	jonArm.skin = plasticMaterial;
	micahArm.tattoo = tattooMaterial;
	micahArm.skin = plasticMaterial;
	Racket racket2(unitCubeAO, "racketMicah");
	racket2.setBall(Ball);
	racket2.jawnAngle = 0;
	racket2.plasticMaterial = metalMaterial;

	// Set mouse and keyboard callbacks
	glfwSetKeyCallback(window, keyPressCallback);
	glfwSetCursorPosCallback(window, mouseCursorPostionCallback);
	//so directional light has colour, ambInt,diffInt, direction
	//      Plight has color, ambInt,diffInt,position, constants
	//      Splight has color, ambInt,diffInt, direction, edge?
	vec3 lightColor(1.0f, 1.0f, 1.0f);

	//directional
	float dAmbInt = .3f;
	float dDiffInt = .4f;
	vec3 dDir = vec3(0, 0, -1);
	glUniform1f(glGetUniformLocation(shaderProgram, "dAmb"), dAmbInt);
	glUniform1f(glGetUniformLocation(shaderProgram, "dDiff"), dDiffInt);
	//Splight
	vec3 spColor(1.0f,0.0f,0.0f);
	float spAmbInt = .10f;
	float spDiffInt = 1.0f;
	vec3 spDir = vec3(0, 0, -1);
	vec3 splightPositio=vec3(-.0f,.5f, .0f);
	vec3 splightDirectio(0, -1, 0);
	splightDirectio = normalize(splightDirectio-splightPositio );

	//SPOTLIGHT 1
	SpotLight sp[2];
	sp[0].lightColor = spColor;
	sp[0].AmbInt = .10f;
	sp[0].DiffInt = 1.0f;
	sp[0].lightPosition = splightPositio;
	sp[0].lightDirection = splightDirectio; //always going to be normalized?
	sp[0].lightCutoffInner = .01f;
	sp[0].lightCutoffOuter = 32.0f;

	int count = sizeof(sp)/sizeof(SpotLight);
	glUniform1i(glGetUniformLocation(shaderProgram, "spotlightCount"), count);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLights[0].Amb"), sp[0].AmbInt);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLights[0].Diff"), sp[0].DiffInt);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLights[0].lightCutoffInner"), (float)cos(radians(sp[0].lightCutoffInner)));
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLights[0].lightCutoffOuter"), (float)cos(radians(sp[0].lightCutoffOuter)));
	
	glUniform3fv(glGetUniformLocation(shaderProgram, "spotLights[0].lightColor"), 1, glm::value_ptr(sp[0].lightColor));
	glUniform3fv(glGetUniformLocation(shaderProgram, "spotLights[0].lightPosition"), 1, &sp[0].lightPosition[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram, "spotLights[0].lightDirection"), 1, &sp[0].lightDirection[0]);	
	//SPOTLIGHT @
	spColor=vec3(.0f, 0.0f, 1.0f);
	splightPositio = vec3(-.0f, .50f, 3.0f);
	splightDirectio=vec3(0, -0.0f, -1.0f);
	splightDirectio = normalize(  splightDirectio-splightPositio);
	sp[1].lightColor = spColor;
	sp[1].AmbInt = .1f;
	sp[1].DiffInt = 5.01f;
	sp[1].lightPosition = splightPositio;
	sp[1].lightDirection = splightDirectio; //always going to be normalized?
	sp[1].lightCutoffInner = .01f;
	sp[1].lightCutoffOuter = 5.0f;
	//struct jawn spotlight
		//int count = sizeof(sp)/sizeof(SpotLight);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLights[1].Amb"), sp[1].AmbInt);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLights[1].Diff"), sp[1].DiffInt);
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLights[1].lightCutoffInner"), (float)cos(radians(sp[1].lightCutoffInner)));
	glUniform1f(glGetUniformLocation(shaderProgram, "spotLights[1].lightCutoffOuter"), (float)cos(radians(sp[1].lightCutoffOuter)));

	glUniform3fv(glGetUniformLocation(shaderProgram, "spotLights[1].lightColor"), 1, glm::value_ptr(sp[1].lightColor));
	glUniform3fv(glGetUniformLocation(shaderProgram, "spotLights[1].lightPosition"), 1, &splightPositio[0]);
	glUniform3fv(glGetUniformLocation(shaderProgram, "spotLights[1].lightDirection"), 1, &splightDirectio[0]);


	//MAIN LIGHT
    GLint lightColorLoc = glGetUniformLocation( shaderProgram, "mlightColor");
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
    // light parameters
    glm::vec3 lightPosition(.0f, 3.50f, 3.0f); // the location of the light in 3D space
	glm::vec3 lightFocus(0.0, 0.0, -1.0);      // the point in 3D space the light "looks" at
	glm::vec3 lightDirection = glm::normalize(lightFocus - lightPosition);

	//set position
	GLint lightPositionLoc = glGetUniformLocation(shaderProgram, "mlightPosition");
	glUniform3fv(lightPositionLoc, 1, &lightPosition[0]);

	// Get lighting-related uniform locations
	float lightNearPlane = 0.01f;
	float lightFarPlane = 180.0f;
	glm::mat4 lightProjectionMatrix = glm::ortho(-1.5f, 1.5f, -1.5f, 1.5f, lightNearPlane, lightFarPlane);
	glm::mat4 lightViewMatrix = glm::lookAt(lightPosition, lightFocus, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;
    GLint lightViewProjMatrixLoc = glGetUniformLocation( shaderProgram, "lightViewProjMatrix");

	// Set light position and direction on scene shader
	GLint lightDirectionLoc = glGetUniformLocation(shaderProgram, "mlightDirection");
	glUniform3fv(lightDirectionLoc, 1, &lightDirection[0]);

	// Set light far and near planes on scene shader
	GLint lightNearPlaneLoc = glGetUniformLocation(shaderProgram, "lightNearPlane");
	GLint lightFarPlaneLoc = glGetUniformLocation(shaderProgram, "lightFarPlane");
	glUniform1f(lightNearPlaneLoc, lightNearPlane);
	glUniform1f(lightFarPlaneLoc, lightFarPlane);
	// Set light view projection matrix
    glUniformMatrix4fv(lightViewProjMatrixLoc, 1, GL_FALSE, &lightSpaceMatrix[0][0]);


    // Dimensions of the shadow texture, which should cover the viewport window size and shouldn't be over-sized and waste resources
    const unsigned int DEPTH_MAP_TEXTURE_SIZE = 1024;
	GLuint depth_map_texture;
	glGenTextures(1, &depth_map_texture);
	glBindTexture(GL_TEXTURE_2D, depth_map_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	GLuint depth_map_fbo;
	glGenFramebuffers(1, &depth_map_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map_texture, 0);
	glReadBuffer(GL_NONE);
	glDrawBuffer(GL_NONE);
	glBindTexture(GL_TEXTURE_2D, 0);
	GLuint kdepthMap = glGetUniformLocation(shaderProgram, "shadowMap");
	glUniform1i(kdepthMap, 2);

	float spin = 0;
	bool reverse = false;
	Letters letterJ(unitCubeAO, "letterJ"); //letters need material
	Letters letterM(unitCubeAO, "letterM");
	Letters letterI(unitCubeAO, "letterI");
	Letters letterL(unitCubeAO, "letterL");
	doRandomColour(letterJ, letterM, letterI, letterL);
	letterJ.shaderProgram = shaderProgram;
	letterM.shaderProgram = shaderProgram;
	letterI.shaderProgram = shaderProgram;
	letterL.shaderProgram = shaderProgram;
	letterJ.J = metalMaterial;
	letterM.M = ropeMaterial;
	letterI.I = woodMaterial;
	letterL.L = grassMaterial;
	glm::vec4 jawn;
	jawn = glm::vec4(1.0f);


	//camera=Camera(RESETeye,up, -60.0f, 0.0f, 5.0f, 0.5f);

	float AMPz = (rand()) / (float)(RAND_MAX);
	float AMPx = (rand()) / (float)(RAND_MAX);
	float PHASEz = (rand()) / (float)(RAND_MAX);
	float PHASEx = (rand()) / (float)(RAND_MAX);
	//updateLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 0, 0), SceneObj, shaderProgram, 0, false, false);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		groupMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f)) *
			glm::scale(glm::mat4(1.0f), GroupMatrixScale) *
			rotationMatrixW;

		 //we can do 30, but it works better lower because the scale?
		bool noshowLightBox = false;
		float x = AMPx * sin(lightRotation + PHASEx); //AMPx*
			float z = AMPz * cos(lightRotation + 10);	// AMPz*
		if (lightRotation >= 5.235f) {
			lightRotation = -1.0f;
			AMPz = (rand()) / (float)(RAND_MAX);
			AMPx = (rand()) / (float)(RAND_MAX);
			PHASEz = (rand()) / 10;
			PHASEx = (rand()) / 10;			
		}

		lightRotation += .03;
			glm::scale(glm::mat4(1.0f), GroupMatrixScale) *
			rotationMatrixW;
	
		float lastFrameTime = glfwGetTime();
		letterJ.groupMatrix = groupMatrix;
		letterM.groupMatrix = groupMatrix;
		letterI.groupMatrix = groupMatrix;
		letterL.groupMatrix = groupMatrix;
		letterJ.renderAs=renderAs;
		letterM.renderAs=renderAs;
		letterI.renderAs=renderAs;
		letterL.renderAs=renderAs;

		bool draw;

		// Must draw scene in 2 passes: once for shadows, and another normally
		// 1st pass
		{
			glUniform1i(applyTexturesLocation, false);
			glUniform1i(applyShadowsLocation, shouldApplyShadows);
			// Use proper image output size
			glViewport(0, 0, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE);
			// Bind depth map texture as output frame buffer
			glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
			// Clear depth data on the frame buffer
			glClear(GL_DEPTH_BUFFER_BIT);

			//Note .8 != 30 for the height so that we can visually see the affects of rotating
			//updateLight(glm::vec3(x, lightDepth, z), glm::vec3(0, 0, 0), SceneObj, shaderProgram, i, true,follow);
			

			// Draw geometry
			jonArm.SetAttr(groupMatrix, renderAs, shaderProgram);
			jonArm.DrawArm();
			micahArm.SetAttr(groupMatrix, renderAs, shaderProgram);
			micahArm.DrawArm();
			//need second racket for micaharm
			racket.SetAttr(groupMatrix, renderAs, shaderProgram, jonArm.partParent);
			racket.Draw();
			racket2.SetAttr(groupMatrix, renderAs, shaderProgram, micahArm.partParent);
			racket2.Draw();
			
			letterJ.partParent = racket.partChild;
			letterM.partParent = racket.partChild;
			letterI.partParent = racket2.partChild;
			letterL.partParent = racket2.partChild;
			draw = letterJ.DrawJ();
			draw = letterM.DrawM();
			draw = letterI.DrawI();
			draw = letterL.DrawL();
			
            SceneObj.sphereVao = unitSphereAO;
            SceneObj.sphereVertCount = vertexIndicessphere.size();
            SceneObj.SetAttr(rotationMatrixW, renderAs, shaderProgram);
            SceneObj.SetVAO(unitCubeAO, gridAO);
            SceneObj.DrawScene(false);  // Draw scene without the skybox, so it can't be used to make shadows on the scene
		}
		{ // 2nd pass  
			//reset 
			glUniform1i(applyTexturesLocation, !shouldApplyTextures);
			glUniform1i(applyShadowsLocation, false);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			glViewport(0, 0, width, height);
			// Clear color and depth data on frame buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glActiveTexture(GL_TEXTURE0 + 2);
			glBindTexture(GL_TEXTURE_2D, depth_map_texture);

			// Draw geometry
			jonArm.SetAttr(groupMatrix, renderAs, shaderProgram);
			jonArm.DrawArm();
			micahArm.SetAttr(groupMatrix, renderAs, shaderProgram);
			micahArm.DrawArm();
			
			racket.SetAttr(groupMatrix, renderAs, shaderProgram, jonArm.partParent);
			racket.Draw();
			racket2.SetAttr(groupMatrix, renderAs, shaderProgram, micahArm.partParent);
			racket2.Draw();
			
			letterJ.partParent = racket.partChild;
			letterM.partParent = racket.partChild;
			letterI.partParent = racket2.partChild;
			letterL.partParent = racket2.partChild;
			draw = letterJ.DrawJ();
			draw = letterM.DrawM();
			draw = letterI.DrawI();
			draw = letterL.DrawL();
			
			
			SceneObj.sphereVao = unitSphereAO;
			SceneObj.sphereVertCount = vertexIndicessphere.size();
			SceneObj.SetAttr(rotationMatrixW, renderAs, shaderProgram);
			SceneObj.SetVAO(unitCubeAO, gridAO);
			SceneObj.DrawScene(true);  // Draw scene with the skybox
			
			updateLight(glm::vec3(clamp(x,-.8f,.8f), lightDepth, clamp(z,-.4f,.4f)), glm::vec3(0, 0, 0), SceneObj, shaderProgram, lightRotation, noshowLightBox,follow);

			//flex arm function?
		}
		


		//SceneObj.DrawLight(cameraPos, vec3(0,1,0), 0);
		//finger manipulation works add one for micah arm

		float check = jonArm.getFRotation();
		if ((jonArm.getFRotation() + spin) > 90.0f && reverse == false) { 
			micahArm.setFRotation(micahArm.getFRotation() - spin);
			jonArm.setFRotation(jonArm.getFRotation() - spin); 
			reverse = true;
		}
		else if (jonArm.getFRotation() + spin < 0.0f && reverse == true) {
			micahArm.setFRotation(micahArm.getFRotation() + spin);
			jonArm.setFRotation(jonArm.getFRotation() + spin); 
			reverse = false; 
		}
		else if (reverse == true){
			micahArm.setFRotation(micahArm.getFRotation() - spin);
			jonArm.setFRotation(jonArm.getFRotation() - spin);
			spin -= .01f;
			}
		else {
			micahArm.setFRotation(micahArm.getFRotation() + spin);
			jonArm.setFRotation(jonArm.getFRotation() + spin);
			spin += .01f;
		}		



		//printf("%f\n", spin);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Shutdown GLFW
	glfwTerminate();
	return 0;
}


/***
Gets the given shaders source code from a file and returns it as a string.
Taken from https://stackoverflow.com/a/11590855
@param PATH: the constant path to the shader to read
@return The source code for the given shader as a string
*/
const std::string getShaderSource(const char* PATH)
{
	std::ifstream stream(PATH); // Create an input file stream from the file path
	std::string content((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));  // Read file content
	stream.close();
	return content;
}

/**
Compile and link the shader program
@return The shader program ID
*/
int compileAndLinkShaders(const char* vertex, const char* fragment)
{
	// Vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const std::string vss = getShaderSource(vertex);
	const char* vertexShaderSource = vss.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "Error. Vertex shader compilation failed!\n" << infoLog << std::endl;
	}

	// Fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const std::string fss = getShaderSource(fragment);
	const char* fragmentShaderSource = fss.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "Error. Fragment shader compilation failed!\n" << infoLog << std::endl;
	}

	// Link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "Error. Shader program linking failed!\n" << infoLog << std::endl;
	}

	// Delete shaders, as they're already been linked to the shader program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

/// An error callback function
/// Courtesy of https://www.khronos.org/opengl/wiki/OpenGL_Error
void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
		<< " type = 0x" << type << ", severity = 0x" << severity << ", message = " << message << std::endl;
}

/**
loadTexture from lab04
@param filename: The file name to load the texture from
@return The texture ID
*/
GLuint loadTexture(const char* filename)
{
	// Step1 Create and bind textures
	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	assert(textureId != 0);

	glBindTexture(GL_TEXTURE_2D, textureId);

	// Step2 Set filter parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
 
	// Step3 Load Textures with dimension data
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
		return 0;
	}

	// Step4 Upload the texture to the PU
	GLenum format = 0;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;
	
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	// Step5 Free resources
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}

//from lab 04
void setProjectionMatrix(int shaderProgram, glm::mat4 projectionMatrix)
{
	glUseProgram(shaderProgram);
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void setViewMatrix(int shaderProgram, glm::mat4 viewMatrix)
{
	glUseProgram(shaderProgram);
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}
void updateCamera(Arm arm,vec3 Translation) {
	//camereSelect = 1;
	vec3 cameraPos = vec3(arm.getTranslation().x + .15f, arm.getTranslation().y + .08, arm.getTranslation().z + .01f);//update position

	glm::mat4 InitviewMatrix = glm::lookAt(cameraPos,  // eye
		Translation,  // center//update view
		up);// 

	setViewMatrix(shaderProgram, InitviewMatrix);

}
/**
GLFW callback function for handling keyboard inputs
*/
void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Get states of each relevant key
	int state_ESC = glfwGetKey(window, GLFW_KEY_ESCAPE);
	int state_SPACE = glfwGetKey(window, GLFW_KEY_SPACE);
	int state_TAB = glfwGetKey(window, GLFW_KEY_TAB);
	int state_U = glfwGetKey(window, GLFW_KEY_U);
	int state_J = glfwGetKey(window, GLFW_KEY_J);
	int state_W = glfwGetKey(window, GLFW_KEY_W);
	int state_A = glfwGetKey(window, GLFW_KEY_A);
	int state_S = glfwGetKey(window, GLFW_KEY_S);
	int state_D = glfwGetKey(window, GLFW_KEY_D);

	int state_M = glfwGetKey(window, GLFW_KEY_M);
	int state_R = glfwGetKey(window, GLFW_KEY_R);
	int state_F = glfwGetKey(window, GLFW_KEY_F);
	//int state_L = glfwGetKey(window, GLFW_KEY_L);
	int state_1 = glfwGetKey(window, GLFW_KEY_1);
	int state_2 = glfwGetKey(window, GLFW_KEY_2);
	int state_3 = glfwGetKey(window, GLFW_KEY_3);

	int state_UP = glfwGetKey(window, GLFW_KEY_UP);
	int state_DOWN = glfwGetKey(window, GLFW_KEY_DOWN);
	int state_LEFT = glfwGetKey(window, GLFW_KEY_LEFT);
	int state_RIGHT = glfwGetKey(window, GLFW_KEY_RIGHT);
	int state_HOME = glfwGetKey(window, GLFW_KEY_HOME);
	int state_PAGEDOWN = glfwGetKey(window, GLFW_KEY_PAGE_DOWN);
	int state_DEL = glfwGetKey(window, GLFW_KEY_DELETE);
	int state_END = glfwGetKey(window, GLFW_KEY_END);

	int state_P = glfwGetKey(window, GLFW_KEY_P);
	int state_L = glfwGetKey(window, GLFW_KEY_L);
	int state_T = glfwGetKey(window, GLFW_KEY_T);
	int state_B = glfwGetKey(window, GLFW_KEY_B);
	int state_X = glfwGetKey(window, GLFW_KEY_X);
	//int state_1 = glfwGetKey(window, GLFW_KEY_1);
	//int state_2 = glfwGetKey(window, GLFW_KEY_2);
	//global random
	float number1 = (rand()) / (float)(RAND_MAX);
	float number2 = (rand()) / (float)(RAND_MAX);
	float number3 = (rand()) / (float)(RAND_MAX);
	// Constrain to visible grid locations
	if (number1 >= .75f)
		number1 = number1 / (float)(RAND_MAX);
	if (number2 >= .25f)
		number2 = number2 / (float)(RAND_MAX);
	if (number3 >= .75f)
		number3 = number3 / (float)(RAND_MAX);
	
	



	// ACTUALLY BRING BACK SWITCH FOR THIGNS
	switch (selectModel) {//prints twice per button press maybe this is okay?
	case(0)://Jon's Model		
		if (state_W == GLFW_PRESS) 
			jonArm.setTranslateModel(glm::vec3(jonArm.getTranslateModel().x, (jonArm.getTranslateModel().y + .005f), jonArm.getTranslateModel().z));
		
		
		else if (state_S == GLFW_PRESS)
			jonArm.setTranslateModel(glm::vec3(jonArm.getTranslateModel().x, (jonArm.getTranslateModel().y - .005f), jonArm.getTranslateModel().z));
		else if ((state_D == GLFW_PRESS) && mods == GLFW_MOD_SHIFT)
			jonArm.setTranslateModel(glm::vec3((jonArm.getTranslateModel().x - .005f), jonArm.getTranslateModel().y, jonArm.getTranslateModel().z));
		else if ((state_A == GLFW_PRESS) && mods == GLFW_MOD_SHIFT)
			jonArm.setTranslateModel(glm::vec3((jonArm.getTranslateModel().x + .005f), jonArm.getTranslateModel().y, jonArm.getTranslateModel().z));
		else if ((state_A == GLFW_PRESS) && mods != GLFW_MOD_SHIFT)
			switch (selectJoint) {
			case(0): jonArm.setRotation(jonArm.getRotation() + 5); center.z -= .15;  break;
			case(1):if (jonArm.getERotation() + 5 > 90)jonArm.setERotation(90); else  jonArm.setERotation(jonArm.getERotation() + 5);  break;
			case(2):if (jonArm.getWRotation() + 5 > 65)jonArm.setWRotation(65); else  jonArm.setWRotation(jonArm.getWRotation() + 5); break;
			default: break;
			}
		else if ((state_D == GLFW_PRESS) && mods != GLFW_MOD_SHIFT)
			switch (selectJoint) {
			case(0): jonArm.setRotation(jonArm.getRotation() - 5);  center.z += .15; break;
			case(1):if (jonArm.getERotation() - 5 < 0)jonArm.setERotation(0); else  jonArm.setERotation(jonArm.getERotation() - 5);  break;
			case(2):if (jonArm.getWRotation() - 5 < -85)jonArm.setWRotation(-85); else  jonArm.setWRotation(jonArm.getWRotation() - 5); break;
			default: break;
			}
		else if (state_SPACE == GLFW_PRESS)
		{

			jonArm.setTranslateRandom(glm::vec3(number1, number2, number3));
		}
		else if (state_M == GLFW_PRESS) { 
			if (camereSelect ==0) {
				camereSelect = -1;
				glm::mat4 InitviewMatrix = glm::lookAt(RESETeye,  // eye
					RESETcenter,  // center
					up);// 

				glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewMatrix"), 1, GL_FALSE, &InitviewMatrix[0][0]);
			}
			else {
				camereSelect = 0;
				vec3 cameraPos = vec3(jonArm.getTranslation().x - .15f, jonArm.getTranslation().y + .08, jonArm.getTranslation().z + .01f);
				//eye = glm::vec3(jonArm.position.x - .25, jonArm.position.y + .5f, jonArm.position.z - .25);
				translateW = 0;
				translateY = 0;
				//center = (vec3(cameraPos.x + .01f, cameraPos.y, cameraPos.z);
				center = vec3(1, 0, 0);
				eye = cameraPos;
				glm::mat4 InitviewMatrix = glm::lookAt(eye,  // eye
					jonArm.position,  // center
					up);// 
				updateCamera(jonArm, center);
				//glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewMatrix"), 1, GL_FALSE, &InitviewMatrix[0][0]);

			}

		}
		if (camereSelect==0)updateCamera(jonArm, center);
		break;

	case(1):
		if (state_W == GLFW_PRESS) {
			micahArm.setTranslateModel(glm::vec3(micahArm.getTranslateModel().x, (micahArm.getTranslateModel().y + .005f), micahArm.getTranslateModel().z));
			//updateCamera(micahArm, micahArm.getTranslation());
		
		}
		else if (state_S == GLFW_PRESS)
			micahArm.setTranslateModel(glm::vec3(micahArm.getTranslateModel().x, (micahArm.getTranslateModel().y - .005f), micahArm.getTranslateModel().z));
		else if ((state_D == GLFW_PRESS) && mods == GLFW_MOD_SHIFT)
			micahArm.setTranslateModel(glm::vec3((micahArm.getTranslateModel().x - .005f), micahArm.getTranslateModel().y, micahArm.getTranslateModel().z));
		else if ((state_A == GLFW_PRESS) && mods == GLFW_MOD_SHIFT)
			micahArm.setTranslateModel(glm::vec3((micahArm.getTranslateModel().x + .005f), micahArm.getTranslateModel().y, micahArm.getTranslateModel().z));
		else if ((state_A == GLFW_PRESS) && mods != GLFW_MOD_SHIFT)
			switch (selectJoint) {
			case(0): micahArm.setRotation(micahArm.getRotation() + 5); center.z -= .15; break;
			case(1):if (micahArm.getERotation() + 5 > 90)micahArm.setERotation(90); else  micahArm.setERotation(micahArm.getERotation() + 5);  break;
			case(2):if (micahArm.getWRotation() + 5 > 65)micahArm.setWRotation(65); else  micahArm.setWRotation(micahArm.getWRotation() + 5); break;
			default: break;
			}
		else if ((state_D == GLFW_PRESS) && mods != GLFW_MOD_SHIFT)
			switch (selectJoint) {
			case(0): micahArm.setRotation(micahArm.getRotation() - 5); center.z += .15;  break;
			case(1):if (micahArm.getERotation() - 5 < 0)micahArm.setERotation(0); else  micahArm.setERotation(micahArm.getERotation() - 5);  break;
			case(2):if (micahArm.getWRotation() - 5 < -85)micahArm.setWRotation(-85); else  micahArm.setWRotation(micahArm.getWRotation() - 5); break;
			default: break;
			}
		else if (state_SPACE == GLFW_PRESS)
		{

			micahArm.setTranslateRandom(glm::vec3(number1, number2, number3));
		}
		else if (state_M == GLFW_PRESS) {
			if (camereSelect == 1) {
				camereSelect = -1;
				glm::mat4 InitviewMatrix = glm::lookAt(RESETeye,  // eye
					RESETcenter,  // center
					up);// 

				glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "viewMatrix"), 1, GL_FALSE, &InitviewMatrix[0][0]);
			}
			else {
				camereSelect = 1;
				translateW = 0;
				translateY = 0;
				vec3 cameraPos = vec3(micahArm.getTranslation().x- .15f, micahArm.getTranslation().y + .08, micahArm.getTranslation().z + .01f);
				center = vec3(cameraPos.x - .015f, cameraPos.y, cameraPos.z);
				center = vec3(1, 0, 0);
				eye = cameraPos;
				updateCamera(micahArm, center);
			}

		}

		if (camereSelect == 1)updateCamera(micahArm, center);
		break;
	}
	if (state_TAB == GLFW_PRESS && mods != GLFW_MOD_SHIFT) {
		if (selectModel == 0) selectModel += 1;
		else if (selectModel == 1) selectModel = 0;
		else selectModel += 1;
		switch (selectModel) {
		case(0):printf("selectModel is: %d , selected jonArm\n", selectModel); break;
		case(1):printf("selectModel is: %d , selected micahArm\n", selectModel); break;
		default: break;
		}
	}
	else if (state_TAB == GLFW_PRESS && mods == GLFW_MOD_SHIFT) {

		if (selectJoint == 0) selectJoint += 1;
		else if (selectJoint == 2) selectJoint = 0;
		else selectJoint += 1;
		switch (selectJoint) {
		case(0):printf("selectJoint is: %d , selected shoulder\n", selectJoint); break;
		case(1):printf("selectJoint is: %d , selected elbow\n", selectJoint); break;
		case(2):printf("selectJoint is: %d , selected wrist\n", selectJoint); break;
		default: break;
		}
	}


	// If u or j is pressed, scale up or down accordingly
	else if (state_U == GLFW_PRESS)
		GroupMatrixScale += .05f;

	else if (state_J == GLFW_PRESS)
		GroupMatrixScale -= .05f;
	else if (state_F == GLFW_PRESS)
	{
		follow = !follow;
		printf("follow: %d\n", follow);
	}

	// If the arrow keys are pressed, rotate accordingly
	else if (state_LEFT == GLFW_PRESS)
		rotationMatrixW *= glm::rotate(glm::mat4(1.0f), glm::radians(2.55f), glm::vec3(1.0f, 0.0f, 0.0f));
	else if (state_RIGHT == GLFW_PRESS)
		rotationMatrixW *= glm::rotate(glm::mat4(1.0f), glm::radians(2.55f), glm::vec3(-1.0f, 0.0f, 0.0f));
	else if (state_UP == GLFW_PRESS)
		rotationMatrixW *= glm::rotate(glm::mat4(1.0f), glm::radians(2.55f), glm::vec3(.0f, 1.0f, 0.0f));
	else if (state_DOWN == GLFW_PRESS)
		rotationMatrixW *= glm::rotate(glm::mat4(1.0f), glm::radians(2.55f), glm::vec3(.0f, -1.0f, 0.0f));
	else if (state_DEL == GLFW_PRESS )
		lightRotation -= .05f;
	else if (state_PAGEDOWN == GLFW_PRESS)
		lightRotation += .05f;
	else if (state_HOME == GLFW_PRESS )
		lightDepth += .02;
	else if (state_END == GLFW_PRESS )
		lightDepth -= .02;
	else if (state_P == GLFW_PRESS)
		renderAs = GL_POINTS;
	else if (state_L == GLFW_PRESS)
		renderAs = GL_LINES;
	else if (state_T == GLFW_PRESS)
		renderAs = GL_TRIANGLES;
	else if ( state_R == GLFW_PRESS) {		
		jonArm.resetArm();		
		GroupMatrixScale = glm::vec3(1.0f);
		rotationMatrixW = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 InitviewMatrix = glm::lookAt(RESETeye, RESETcenter, up);
		glm::mat4 projectionMatrix = glm::perspective(FOV, AR, near, far);
		setProjectionMatrix(shaderProgram, projectionMatrix);
		setViewMatrix(shaderProgram, InitviewMatrix);
	}
	else if (state_B == GLFW_PRESS)
		shouldApplyShadows = !shouldApplyShadows;
	else if (state_X == GLFW_PRESS)
		shouldApplyTextures = !shouldApplyTextures;
	else 	if (state_ESC == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	else 	if (state_1 == GLFW_PRESS) {		
		shouldApplySpLight = !shouldApplySpLight;
		glUniform1i(glGetUniformLocation(shaderProgram, "shouldApplySpLight"), shouldApplySpLight);
		printf("Apply Spotlight: %d:\n", shouldApplySpLight);
	}
	else 	if (state_2 == GLFW_PRESS) {
		shouldApplyMLight = !shouldApplyMLight;
		glUniform1i(glGetUniformLocation(shaderProgram, "shouldApplyMLight"), shouldApplyMLight);
		printf("Apply Mainlight: %d:\n", shouldApplyMLight);
}
	else 	if (state_3 == GLFW_PRESS) {
		shouldApplySpLight = false;
		shouldApplyMLight = false;
		glUniform1i(glGetUniformLocation(shaderProgram, "shouldApplySpLight"), false);
		glUniform1i(glGetUniformLocation(shaderProgram, "shouldApplyMLight"), false);
		printf("Apply no light\n");
}
}

/**
GLFW callback function for handling mouse button and position
*/
void mouseCursorPostionCallback(GLFWwindow* window, double xPos, double yPos)
{
	// Get state of each mouse button
	int state_LEFT = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int state_MIDDLE = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
	int state_RIGHT = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	if (state_LEFT == GLFW_PRESS)
	{
		double dy = yPos - lastMousePosY;
		//translateY = 0;
		if (dy < 0)
			translateY += .05;
		else if (dy > 0)
			translateY -= .05;

		glm::mat4 InitviewMatrix = glm::lookAt(eye, glm::vec3(translateW, translateY, 0.0f), up);
		setViewMatrix(shaderProgram, InitviewMatrix);
		lastMousePosY = yPos;

		if (camereSelect == 0) {
			vec3 jawn = glm::vec3(0.0f,  0.0f, 0.0f) + jonArm.getTranslation();
			lastMousePosX = xPos;
			glm::mat4 InitviewMatrix = glm::lookAt(jawn, glm::vec3(1, translateY + center.y, translateW + center.z), up);
	
			jawn= glm::vec3(1, translateY, translateW);
			updateCamera(jonArm, jawn);
		
		}
		else if (camereSelect == 1) {
			vec3 jawn = glm::vec3(0.0f, 0.0f, 0.0f) + micahArm.getTranslation();
			glm::mat4 InitviewMatrix = glm::lookAt(jawn, glm::vec3(1, translateY+center.y, translateW + center.z), up);

			updateCamera(micahArm, vec3(1, translateY, translateW));
			lastMousePosY = yPos;
		
		
		}
		else {
			glm::mat4 InitviewMatrix = glm::lookAt(RESETeye, glm::vec3(translateW + RESETcenter.x, translateY + RESETcenter.y, 0.0f), up);
			setViewMatrix(shaderProgram, InitviewMatrix);
			lastMousePosY = yPos;
		
		}




	}

	else if (state_RIGHT == GLFW_PRESS)
	{
		double dx = xPos - lastMousePosX;

		if (dx < 0)
			translateW -= .005;
		else if (dx > 0)
			translateW += .005;

		glm::mat4 InitviewMatrix = glm::lookAt(eye, glm::vec3(translateW, translateY, 0.0f), up);
		setViewMatrix(shaderProgram, InitviewMatrix);
		lastMousePosY = yPos;
		if (camereSelect == 0) {
			vec3 jawn = glm::vec3(0 , 0.0f ,0) + jonArm.getTranslation();
			glm::mat4 InitviewMatrix = glm::lookAt(jawn, glm::vec3(1, translateY, translateW), up);
	
			jawn = glm::vec3(1, translateY, translateW);
			updateCamera(jonArm, jawn);

			lastMousePosX = xPos;
		
		}
		else if (camereSelect == 1) {
			vec3 jawn = glm::vec3( 0.0f, 0.0f,0 ) + micahArm.getTranslation();
			glm::mat4 InitviewMatrix = glm::lookAt(jawn, glm::vec3(1, translateY, translateW), up);
		
			updateCamera(micahArm, vec3(1, translateY, translateW));

			lastMousePosX = xPos;
		
		
		}
		else {
			glm::mat4 InitviewMatrix = glm::lookAt(RESETeye, glm::vec3(translateW + RESETcenter.x, translateY + RESETcenter.y, 0.0f), up);
			setViewMatrix(shaderProgram, InitviewMatrix);
			lastMousePosX = xPos;
		}
	}

	else if (state_MIDDLE == GLFW_PRESS)
	{
		double zPos = yPos;
		double dy = zPos - lastMousePosZ;

		if (dy < 0)
			translateZ += .005;
		else if (dy > 0)
			translateZ -= .005;

		glm::mat4 projectionMatrix = glm::perspective(translateZ + 70.0f, (float)WIDTH / (float)HEIGHT, .01f, 50.0f);
		setProjectionMatrix(shaderProgram, projectionMatrix);
		lastMousePosZ = zPos;
	}
}
bool loadOBJ2(const char* path, std::vector<int>& vertexIndices, std::vector<glm::vec3>& temp_vertices,
	std::vector<glm::vec3>& out_normals, std::vector<glm::vec2>& out_uvs)
{

	std::vector<int> uvIndices, normalIndices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec3> my_normals;

	FILE* file;
	file = fopen(path, "r");
	if (!file) {
		printf("Impossible to open the file ! Are you in the right path ?\n");
		getchar();
		return false;
	}
	int count = 0;
	int indexCount = 0;
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			res = fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			temp_vertices.push_back(vertex);
			count += 1;
			indexCount += 1;
			int IndC = indexCount - 3;
			int IndB = indexCount - 2;
			int IndA = indexCount - 1;
			int pause = 1000;
			if (count == 3 && indexCount == 3) {
				//my_normals.push_back(temp_vertices[0]);
				//my_normals.push_back(temp_vertices[1]);
				//my_normals.push_back(temp_vertices[2]);
				my_normals.push_back(normalize(cross(temp_vertices[1] - temp_vertices[0], temp_vertices[2] - temp_vertices[0])));
				my_normals.push_back(normalize(cross(temp_vertices[1] - temp_vertices[0], temp_vertices[2] - temp_vertices[0])));
				my_normals.push_back(normalize(cross(temp_vertices[1] - temp_vertices[0], temp_vertices[2] - temp_vertices[0])));
				count = 0;
			}
			else if (count == 3) {
				int IndC = indexCount - 3;
				int IndB = indexCount - 2;
				int IndA = indexCount - 1;
				my_normals.push_back(normalize(cross(temp_vertices[indexCount - 2] - temp_vertices[indexCount - 3], temp_vertices[indexCount - 1] - temp_vertices[indexCount - 3])));
				my_normals.push_back(normalize(cross(temp_vertices[indexCount - 2] - temp_vertices[indexCount - 3], temp_vertices[indexCount - 1] - temp_vertices[indexCount - 3])));
				my_normals.push_back(normalize(cross(temp_vertices[indexCount - 2] - temp_vertices[indexCount - 3], temp_vertices[indexCount - 1] - temp_vertices[indexCount - 3])));
				count = 0;
			}


		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			res = fscanf(file, "%f %f\n", &uv.x, &uv.y);
			if (res != 2) {
				printf("Missing uv information!\n");
			}
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			res = fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			if (res != 3) {
				printf("Missing normal information!\n");
			}
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			char* getRes;
			int vertexIndex[3], uvIndex[3], normalIndex[3];
			bool uv = true;
			bool norm = true;
			char line[128];
			getRes = fgets(line, 128, file);
			if (getRes == 0) {
				printf("incomplete face\n");
			}

			//vertex, uv, norm
			int matches = sscanf(line, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				//vertex, norm
				matches = sscanf(line, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
				if (matches != 6) {
					//vertex, uv
					matches = sscanf(line, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2]);
					if (matches != 6) {
						//vertex
						matches = sscanf(line, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);
						if (matches != 3) {
							printf("File can't be read by our simple parser. 'f' format expected: d/d/d d/d/d d/d/d || d/d d/d d/d || d//d d//d d//d\n");
							printf("Character at %ld", ftell(file));
							return false;
						}
						uv, norm = false;
					}
					else {
						norm = false;
					}
				}
				else {
					uv = false;
				}
			}
			vertexIndices.push_back(abs(vertexIndex[0]) - 1);
			vertexIndices.push_back(abs(vertexIndex[1]) - 1);
			vertexIndices.push_back(abs(vertexIndex[2]) - 1);
			if (norm) {
				normalIndices.push_back(abs(normalIndex[0]) - 1);
				normalIndices.push_back(abs(normalIndex[1]) - 1);
				normalIndices.push_back(abs(normalIndex[2]) - 1);
			}
			if (uv) {
				uvIndices.push_back(abs(uvIndex[0]) - 1);
				uvIndices.push_back(abs(uvIndex[1]) - 1);
				uvIndices.push_back(abs(uvIndex[2]) - 1);
			}
		}
		else {
			char clear[1000];
			char* getsRes = fgets(clear, 1000, file);
		}
	}
	if (normalIndices.size() != 0)
		out_normals.resize(temp_normals.size());
	if (uvIndices.size() != 0)
		out_uvs.resize(temp_uvs.size());
	out_normals = my_normals;
	out_uvs = temp_uvs;
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		int vi = vertexIndices[i];
		if (normalIndices.size() != 0) {
			int ni = normalIndices[i];
			out_normals[vi] = temp_normals[ni];
		}
		if (uvIndices.size() != 0 && i < uvIndices.size()) {
			int ui = uvIndices[i];
			out_uvs[vi] = temp_uvs[ui];
		}
	}

	return true;
}


void doRandomColour(Letters& letterJ, Letters& letterM, Letters& letterI, Letters& letterL) {
	glm::vec4 colours[12];


	for (int trand = 0; trand < 12; trand++) {
		colours[trand] = glm::vec4((float)rand() / (float)RAND_MAX,
			(float)rand() / (float)RAND_MAX,
			(float)rand() / (float)RAND_MAX, 1.0f);
	}
	int colourIndex = 0;
	for (int i = 0; i < 3; i++) {
		colourIndex = i;
		if (i == 1) {
			glm::vec4 test = colours[colourIndex];
			vec4 testy = vec4(vec3(test),0.250f);
			letterJ.colour[i] = testy;

		}
		else letterJ.colour[i] = colours[colourIndex];
		
	}
	for (int i = 0; i < 3; i++) {
		colourIndex++;
		if (i == 1) {
			glm::vec4 test = colours[colourIndex];
			vec4 testy = vec4(vec3(test), 0.250f);
			letterM.colour[i] = testy;

		}
		else letterM.colour[i] = colours[colourIndex];
	}
	for (int i = 0; i < 3; i++) {
		colourIndex++;
		if (i == 1) {
			glm::vec4 test = colours[colourIndex];
			vec4 testy = vec4(vec3(test), 0.250f);
			letterI.colour[i] = testy;

		}
		else letterI.colour[i] = colours[colourIndex];
	}
	for (int i = 0; i < 3; i++) {
		colourIndex++;
		if (i == 1) {
			glm::vec4 test = colours[colourIndex];
			vec4 testy = vec4(vec3(test), 0.250f);
			letterL.colour[i] = testy;

		}
		else letterL.colour[i] = colours[colourIndex];
	}

}