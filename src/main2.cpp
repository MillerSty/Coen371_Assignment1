// Comp 371 - Project

// System includes
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>



// Dependency includes
#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
#include <irrKlang.h>
#pragma comment(lib, "../thirdparty/irrklang/lib/irrKlang.lib")  // Necessary to get irrKlang working

// Source code includes
#include "Arm.h"
#include "Racket.h"
#include "SceneObjects.h"
#include "Material.h"
#include "KeyFrame.h"
#include "Letters.h"
#include "Ball.h"
#include "Models/Model.h"
#include "CrowdObjects.h"

using namespace glm;

// Set the shader paths
const char* vertex = "../src/shaders/unifiedVertex.glsl";
const char* fragment = "../src/shaders/unifiedFragment.glsl";

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

int createVertexArrayObject3(std::vector<glm::vec3> arr)
{
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, arr.size(), &arr.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedNormaledVertex), (void*)sizeof(glm::vec3));
	//glEnableVertexAttribArray(1);
	//
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedNormaledVertex), (void*)(2 * sizeof(glm::vec3)));
	//glEnableVertexAttribArray(2);

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

void updateLight(glm::vec3 newPosition,glm::vec3 newFocus,SceneObjects SceneObj,GLuint shaderProgram,float i,bool applyShadow) {
	// light parameters
	glm::vec3 lightPosition(-.30f, .30f, .0f); // the location of the light in 3D space
	glm::vec3 lightFocus(0.0, 0.0, 0.0);      // the point in 3D space the light "looks" at
	glm::vec3 lightDirection = glm::normalize(newFocus - newPosition);

	GLint lightPositionLoc = glGetUniformLocation(shaderProgram, "lightPosition");
	GLint lightDirectionLoc = glGetUniformLocation(shaderProgram, "lightDirection");
	GLint lightViewProjMatrixLoc = glGetUniformLocation(shaderProgram, "lightViewProjMatrix");

	// Set light position on scene shader
	glUniform3fv(lightPositionLoc, 1, &newPosition[0]);

	// Set light direction on scene shader
	glUniform3fv(lightDirectionLoc, 1, &lightDirection[0]);
	float lightNearPlane = 0.1f;
	float lightFarPlane = 180.0f;

	glm::mat4 lightProjectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, lightNearPlane, lightFarPlane);
	glm::mat4 lightViewMatrix = glm::lookAt(newPosition, newFocus, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;
	glUniformMatrix4fv(lightViewProjMatrixLoc, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
	if (!applyShadow) {
		SceneObj.DrawLight(newPosition, glm::vec3(0.0f, 1.0f, 0.0f), i);
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

GLuint IBO;

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
glm::vec3 eye(.0f, .350f*1.5, .7650f*1.5);
glm::vec3 center(.00f, .0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 translateWSAD(0.0f, 0.0f, 0.0f);
glm::vec3 Translate(.0f, .0f, .0f);
glm::vec3 GroupMatrixScale(1.0f, 1.0f, 1.0f);
glm::mat4 groupMatrix;
glm::mat4 rotationMatrixW = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

Letters numberDraw;
//numberDraw.position = glm::vec3(.2f, 0, 0);
Letters numberDraw2;
//numberDraw.position = vec3(0.0f, 0.0f, 0.0f);
//numberDraw2.position = vec3(.4f, 0, 0);
int renderAs = GL_TRIANGLES;
int shaderProgram;
double lastMousePosX, lastMousePosY, lastMousePosZ;
float FOV = 70, AR = (float) WIDTH / (float) HEIGHT, near = .01, far = 50;
float translateW = 0, translateY = 0, translateZ = 0;

// Toggles for shadows and textures
bool shouldApplyShadows = true;
bool shouldApplyTextures = true;
Arm playerArm1;
Arm playerArm2;
int selectModel = 0; //we can se to 0 but then user has to toggle to before any thing
int selectJoint = 0;
CrowdObjects crowd;
// Create ball
Ball ball;
bool soundPlayed = false;

// Create irrKlang engine
irrklang::ISoundEngine* audioEngine;

// Create Scene object
SceneObjects SceneObj("scene");

int main(int argc, char* argv[])
{
    // Seed a random number generator for later use. Taken from https://stackoverflow.com/a/5891824
    srand(time(nullptr));

	// Initialize GLFW and OpenGL version
	if (!glfwInit())
		return -1;

    // Initialize irrKlang
    audioEngine = irrklang::createIrrKlangDevice();

    if (!audioEngine)
    {
        std::cout << "Failed to create irrKlang engine" << std::endl;
        return -1;
    }

	// Scale down the unit cube vertices
	for (auto& vertexArrayObject : texturedCubeVertexArray) {
        vertexArrayObject.position *= 0.1f;
	}

	// Set some GLFW window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

	// Create Window and rendering context using GLFW, resolution is 800x600
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Comp371 - Assignment 2", nullptr, nullptr);
	if (window == nullptr)
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
	
	// Black background	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Set frame rate to refresh rate of monitor
	glfwSwapInterval(1);

	// Compile and link shaders here
	shaderProgram = compileAndLinkShaders(vertex, fragment);

	// Initialize uniform locations
	glUseProgram(shaderProgram);
	GLint worldMatrixLocation      = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLint viewMatrixLocation       = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLint colorLocation            = glGetUniformLocation(shaderProgram, "objectColor");
    GLint viewPositionLocation     = glGetUniformLocation(shaderProgram, "viewPosition");
    GLint applyTexturesLocation    = glGetUniformLocation(shaderProgram, "shouldApplyTexture");
    GLint applyShadowsLocation     = glGetUniformLocation(shaderProgram, "shouldApplyShadows");
	
	// Maybe update this for shadows
    glUniform3fv(viewPositionLocation, 1, &eye[0]);

	SceneObj.InitGrid();

    // Set the sound engine for the Scene
	SceneObj.audioEngine = audioEngine;

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
	for (auto & vert : verticessphere) {
		vert *= 0.05f;
	}
	
	// Create VAOs
	int gridAO = createVertexArrayObject(SceneObj.lineArray, sizeof(SceneObj.lineArray));
	int unitSphereAO = createVertexArrayElementObject2(vertexIndicessphere, verticessphere, normalssphere, UVssphere);
	int unitCubeAO = createVertexArrayObject2();

	//TEXTURE DEFINITION
	// 	// Load the textures
	GLuint courtTextureID = loadTexture("../src/Assets/clay2.jpg");
	GLuint ropeTextureID = loadTexture("../src/Assets/rope.jpg");
	GLuint clothTextureID = loadTexture("../src/Assets/cloth.jpg");
	GLuint metalTextureID = loadTexture("../src/Assets/metal.jpg");
	GLuint grassTextureID = loadTexture("../src/Assets/grass4.jpg");
	GLuint plasticTextureID = loadTexture("../src/Assets/plastic.jpg");
	GLuint woodTextureID = loadTexture("../src/Assets/wood1.jpg");
	GLuint tattooTextureID = loadTexture("../src/Assets/tattoo.jpg");
						//diff spec ambient shiny
	Material courtMaterial(.2f, .002f, .50f, .001f, courtTextureID, shaderProgram); //court shouldnt reflect
	Material ropeMaterial(.5f, .60f, .5f, .09f, ropeTextureID, shaderProgram); // ropes are just ropes
	Material clothMaterial(.5f, .30f, .5f, .02f, clothTextureID, shaderProgram); //cloth should have a little reflection?
	Material metalMaterial(.6f, .90f, .6f, .12f, metalTextureID, shaderProgram); //metal should shine
	Material grassMaterial(.60f, .001f, .6f, .0001f, grassTextureID, shaderProgram); //just bright, thats all it needs
	Material plasticMaterial(.5f, .30f, .4f, .1f, plasticTextureID, shaderProgram); //needs to be glossy! This is our racket1
	Material woodMaterial(.5f, .60f, .5f, .002f, woodTextureID, shaderProgram); //this is you matt
	Material tattooMaterial(0.5f, 0.2f, 0.5f, 0.002f, tattooTextureID, shaderProgram);
	Material skinMaterial(.1f, .0f, .66f, .001f, plasticTextureID, shaderProgram); //this is skin
	Material skyMaterial(.3f, .001f, .9f, .0001f, plasticTextureID, shaderProgram); //Flat blue sky

	Model Bleachers;

	Bleachers = Model();
	Bleachers.LoadModel("../src/Models/bleachers.obj");

	SceneObj.setMaterials(courtMaterial, clothMaterial, ropeMaterial, metalMaterial, grassMaterial, plasticMaterial);
	SceneObj.skyTexture = skyMaterial;
	
	//Crowd
	crowd.vaos[0] = unitCubeAO;
	crowd.vaos[1] = unitSphereAO;
	crowd.sphereIndexCount = vertexIndicessphere.size();
	crowd.skinMaterial = skinMaterial;
	crowd.clothMaterial = clothMaterial;
	crowd.shaderProgram = shaderProgram;
	crowd.renderAs = GL_TRIANGLES;
	//Racket and Arm ****	
	playerArm1.InitArm(glm::vec3(-.5f, 0.15f, .2f), unitCubeAO, skinMaterial, clothMaterial);
    playerArm2.InitArm(glm::vec3(.5f, 0.15f, -.2f), unitCubeAO, skinMaterial, clothMaterial);

	Racket racket1(unitCubeAO, "racket1");
    racket1.plasticMaterial = plasticMaterial;
    Racket racket2(unitCubeAO, "racket2");
    racket2.plasticMaterial = plasticMaterial;
	//**** End jons jawn
	numberDraw.cubeVao = unitCubeAO;
	numberDraw.shaderProgram = shaderProgram;

	numberDraw.plastic = skinMaterial;
	numberDraw2.plastic = skinMaterial;

	//can be defined outside of while?
	numberDraw.position = vec3(-.20f, .21f, -0.40f); //this is scoreboard option
	numberDraw2.position = vec3(.20f, .21f, -0.40f);

	numberDraw2.cubeVao = unitCubeAO;
	numberDraw2.shaderProgram = shaderProgram;



	// Set mouse and keyboard callbacks
	glfwSetKeyCallback(window, keyPressCallback);
	glfwSetCursorPosCallback(window, mouseCursorPostionCallback);
	//glfwSetWindowSizeCallback(window, windowSizeCallback);

    // Lighting
    float lightAngleOuter = 10.0;
    float lightAngleInner = 0.01;
    // Set light cutoff angles on scene shader
    GLint lightCutoffInnerLoc = glGetUniformLocation( shaderProgram, "lightCutoffInner");
    GLint lightCutoffOuterLoc = glGetUniformLocation( shaderProgram, "lightCutoffOuter");
    glUniform1f(lightCutoffInnerLoc, cos(glm::radians(lightAngleInner)));
    glUniform1f(lightCutoffOuterLoc, cos(glm::radians(lightAngleOuter)));

    GLint lightColorLoc = glGetUniformLocation( shaderProgram, "lightColor");

    glUniform3fv(lightColorLoc, 1, glm::value_ptr(vec3(1.0f, 1.0f, 1.0f)));

    // light parameters
    glm::vec3 lightPosition(-0.0f, 30.0f, .0f); // the location of the light in 3D space
	glm::vec3 lightFocus(0.0, -0.01, .0f);      // the point in 3D space the light "looks" at
	glm::vec3 lightDirection = glm::normalize(lightFocus - lightPosition);

    float lightNearPlane = 0.01f;
    float lightFarPlane = 180.0f;

	glm::mat4 lightProjectionMatrix = glm::ortho(-1.5f, 1.50f, -1.50f, 1.50f, lightNearPlane, lightFarPlane);
	glm::mat4 lightViewMatrix = glm::lookAt(lightPosition, lightFocus, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;

	// Get lighting-related uniform locations
    GLint lightViewProjMatrixLoc = glGetUniformLocation( shaderProgram, "lightViewProjMatrix");
    GLint lightNearPlaneLoc      = glGetUniformLocation( shaderProgram, "lightNearPlane");
    GLint lightFarPlaneLoc       = glGetUniformLocation( shaderProgram, "lightFarPlane");
    GLint lightPositionLoc       = glGetUniformLocation( shaderProgram, "lightPosition");
    GLint lightDirectionLoc      = glGetUniformLocation( shaderProgram, "lightDirection");

	// Set light view projection matrix
    glUniformMatrix4fv(lightViewProjMatrixLoc, 1, GL_FALSE, &lightSpaceMatrix[0][0]);

    // Set light far and near planes on scene shader
    glUniform1f(lightNearPlaneLoc, lightNearPlane);
    glUniform1f(lightFarPlaneLoc, lightFarPlane);

    // Set light position on scene shader
    glUniform3fv(lightPositionLoc, 1, &lightPosition[0]);

    // Set light direction on scene shader
    glUniform3fv(lightDirectionLoc, 1, &lightDirection[0]);

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
	GLint kdepthMap = glGetUniformLocation(shaderProgram, "shadowMap");
	glUniform1i(kdepthMap, 2);

    glfwSetTime(0.0f);
	//double lastWorldTime = 0.0;
    double dt = 0;

	// Keyframes for Blue player
    KeyFrame keyframesBlue[] = {
        KeyFrame(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0), 0.0), // Initial key frame
        KeyFrame(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0), 5.0), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, -0.1), glm::vec3(0.0), 6.0), // Hit ball
		KeyFrame(glm::vec3(0.0, 0.0, -0.1), glm::vec3(0.0), 20.0), // Start moving away from ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.005), glm::vec3(0.0), 21.2), // Be away from ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.005), glm::vec3(0.0), 23.0), // Start moving back
		KeyFrame(glm::vec3(0.0, 0.0, -0.1), glm::vec3(0.0), 24.0), // Be in position
    };

    // Keyframes for Red player
	KeyFrame keyframesRed[] = {
		KeyFrame(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0), 0.0), // Initial key frame
		KeyFrame(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0), 2.0), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(0.0), 3.0), // Hit ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(0.0), 8.0), // Start moving away from ball
		KeyFrame(glm::vec3(0.0, 0.0, -0.005), glm::vec3(0.0), 9.0), // Be away from ball
		KeyFrame(glm::vec3(0.0, 0.0, -0.005), glm::vec3(0.0), 11.0), // Start moving back
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(0.0), 12.0), // Be in position
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(0.0), 39.0), // Start moving away from ball
		KeyFrame(glm::vec3(0.0, 0.0, -0.005), glm::vec3(0.0), 40.0), // Be away from ball
    };

    const float BALL_Y_OFFSET = 0.25f;  // Make sure ball is constant height to keep things simple

    // Keyframes for Ball
	KeyFrame keyframesBall[] = {
        KeyFrame(glm::vec3(0.0, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 0.0), // Initial key frame
        KeyFrame(glm::vec3(0.47, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 3.0), // Initial movement right
        KeyFrame(glm::vec3(-0.47, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 6.0), // Ball hit, move back
        KeyFrame(glm::vec3(0.75, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 9.0), // SCORE
        KeyFrame(glm::vec3(0.75, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 10.0), // Stay off-screen for a moment
        KeyFrame(glm::vec3(0.0, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 11.0), // Reset to center
        KeyFrame(glm::vec3(0.0, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 13.0), // Stay in center for a moment
        KeyFrame(glm::vec3(-0.47, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 16.0), // Move to left
        KeyFrame(glm::vec3(0.47, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 19.0), // Ball hit, move back
        KeyFrame(glm::vec3(-0.75, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 22.0), // SCORE
        KeyFrame(glm::vec3(-0.75, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 23.0), // Stay off-screen a moment
        KeyFrame(glm::vec3(0.0, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 24.0), // Reset to center
        KeyFrame(glm::vec3(0.0, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 25.0), // Stay in center for a moment
        KeyFrame(glm::vec3(0.47, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 28.0), // Move to the right
        KeyFrame(glm::vec3(-0.47, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 31.0), // Ball hit, move back
        KeyFrame(glm::vec3(0.47, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 34.0), // Move to the right
        KeyFrame(glm::vec3(-0.47, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 37.0), // Ball hit, move back
        KeyFrame(glm::vec3(0.75, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 40.0), // SCORE
	};

    int keyframeNumBlue = 0;
    int keyframeNumRed = 0;
	int keyframeNumBall = 0;

	ball.setShaderProgram(shaderProgram);
	ball.setVAO(unitSphereAO);
	ball.setSphereVertCount(vertexIndicessphere.size());
	ball.setMaterial(grassMaterial);
    ball.setSoundEngine(audioEngine);

	int number = 0;
	float i = -1;
	float spin = 0;
	bool reverse = false;

	bool scoreIncremented = false;
	int redScore = 0, blueScore = 0;

	irrklang::ISoundEngine* bigCrowdSound = irrklang::createIrrKlangDevice();
	irrklang::ISound* sound = bigCrowdSound->play2D("../src/Assets/sounds/BigCrowd.wav", true, false, true);

	bigCrowdSound->setSoundVolume(0.15f);

	glfwSetTime(0.0f);

    // MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
        // Clear the depth buffer and color buffer each frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set initial group matrix
		groupMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f)) *
			          glm::scale(glm::mat4(1.0f), GroupMatrixScale) *
			          rotationMatrixW;
		crowd.groupMatrix = groupMatrix;
		float lightDepth = 1.0f; //we can do 30, but it works better lower because the scale?
		bool noshowLightBox = false;
		float x = sin(i);
		float z = cos(i);
		i += .002;

        // KEYFRAME ANIMATION
        // Get current time
        double currentWorldTime = glfwGetTime();

        // Blue player keyframes
        // Since we need the current and next keyframes, make sure we stop advancing through the array when we
        // only have 2 keyframes left
        if (keyframeNumBlue <= (sizeof(keyframesBlue) / sizeof(KeyFrame)) - 2)
        {
            // Handle time calculations
            double currentFrameTime = keyframesBlue[keyframeNumBlue].time;
            double nextFrameTime = keyframesBlue[keyframeNumBlue + 1].time;
            double frameDuration = nextFrameTime - currentFrameTime ;
            double timeProportion = (currentWorldTime - currentFrameTime) / frameDuration;

            // Handle setting how much X should translate based on time proportion
            double currentCoordX = keyframesBlue[keyframeNumBlue].translation.x;
            double nextCoordX = keyframesBlue[keyframeNumBlue + 1].translation.x;
            currentCoordX += (nextCoordX - currentCoordX) * timeProportion;

            // Handle setting how much Y should translate based on time proportion
            double currentCoordY = keyframesBlue[keyframeNumBlue].translation.y;
            double nextCoordY = keyframesBlue[keyframeNumBlue + 1].translation.y;
            currentCoordY += (nextCoordY - currentCoordY) * timeProportion;

            // Handle setting how much Z should translate based on time proportion
            double currentCoordZ = keyframesBlue[keyframeNumBlue].translation.z;
            double nextCoordZ = keyframesBlue[keyframeNumBlue + 1].translation.z;
            currentCoordZ += (nextCoordZ - currentCoordZ) * timeProportion;

            // Set the model translation in world space
            playerArm1.setTranslateModel(glm::vec3(currentCoordX, currentCoordY, currentCoordZ));

            // If the realtime clock is beyond the next keyframes time parameter, move to the next keyframe
            if (currentWorldTime >= nextFrameTime)
                keyframeNumBlue++;
        }

        // Red player keyframes
        if (keyframeNumRed <= (sizeof(keyframesRed) / sizeof(KeyFrame)) - 2)
        {
            // Handle time calculations
            double currentFrameTime = keyframesRed[keyframeNumRed].time;
            double nextFrameTime = keyframesRed[keyframeNumRed + 1].time;
            double frameDuration = nextFrameTime - currentFrameTime ;
            double timeProportion = (currentWorldTime - currentFrameTime) / frameDuration;

            // Handle setting how much X should translate based on time proportion
            double currentCoordX = keyframesRed[keyframeNumRed].translation.x;
            double nextCoordX = keyframesRed[keyframeNumRed + 1].translation.x;
            currentCoordX += (nextCoordX - currentCoordX) * timeProportion;

            // Handle setting how much Y should translate based on time proportion
            double currentCoordY = keyframesRed[keyframeNumRed].translation.y;
            double nextCoordY = keyframesRed[keyframeNumRed + 1].translation.y;
            currentCoordY += (nextCoordY - currentCoordY) * timeProportion;

            // Handle setting how much Z should translate based on time proportion
            double currentCoordZ = keyframesRed[keyframeNumRed].translation.z;
            double nextCoordZ = keyframesRed[keyframeNumRed + 1].translation.z;
            currentCoordZ += (nextCoordZ - currentCoordZ) * timeProportion;

            // Set the model translation in world space
            playerArm2.setTranslateModel(glm::vec3(currentCoordX, currentCoordY, currentCoordZ));

            // If the realtime clock is beyond the next keyframes time parameter, move to the next keyframe
            if (currentWorldTime >= nextFrameTime)
                keyframeNumRed++;
        }

        // Ball keyframes
        if (keyframeNumBall <= (sizeof(keyframesBall) / sizeof(KeyFrame)) - 2)
        {
            // Handle time calculations
            double currentFrameTime = keyframesBall[keyframeNumBall].time;
            double nextFrameTime = keyframesBall[keyframeNumBall + 1].time;
            double frameDuration = nextFrameTime - currentFrameTime ;
            double timeProportion = (currentWorldTime - currentFrameTime) / frameDuration;

            // Handle setting how much X should translate based on time proportion
            double currentCoordX = keyframesBall[keyframeNumBall].translation.x;
            double nextCoordX = keyframesBall[keyframeNumBall + 1].translation.x;
            currentCoordX += (nextCoordX - currentCoordX) * timeProportion;

            // Handle setting how much Y should translate based on time proportion
            double currentCoordY = keyframesBall[keyframeNumBall].translation.y;
            double nextCoordY = keyframesBall[keyframeNumBall + 1].translation.y;
            currentCoordY += (nextCoordY - currentCoordY) * timeProportion;

            // Handle setting how much Z should translate based on time proportion
            double currentCoordZ = keyframesBall[keyframeNumBall].translation.z;
            double nextCoordZ = keyframesBall[keyframeNumBall + 1].translation.z;
            currentCoordZ += (nextCoordZ - currentCoordZ) * timeProportion;

            // Set the model translation in world space
            ball.setTranslationModel(glm::vec3(currentCoordX, currentCoordY, currentCoordZ));

            // If the realtime clock is beyond the next keyframes time parameter, move to the next keyframe
            if (currentWorldTime >= nextFrameTime)
                keyframeNumBall++;
        }

		numberDraw.groupMatrix = groupMatrix;
		numberDraw2.groupMatrix = groupMatrix;
		numberDraw.renderAs = renderAs;
		numberDraw2.renderAs = renderAs;
		number = floor(glfwGetTime());
		if (number > 98)glfwSetTime(0);

		/*so to control 1 arm .
		setTranslate model to translate -> have to figure out specific vec positions
		setRotation  for should rotation
		setERotation for elbow rotation
		setWRotation for wrist rotation
		*/
		
		vec3 position1 = ball.getPosition();
	
		float checky = playerArm1.position.x;
	
		//https://stackoverflow.com/questions/13915479/c-get-every-number-separately
		//this for seperating more

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
			updateLight(glm::vec3(x, lightDepth, z), glm::vec3(0, 0, 0), SceneObj, shaderProgram, i, true);
			if (i == 1.0f) i = -1.0f;

			numberDraw.Scoreboard(number,false,true);
			numberDraw2.Scoreboard(number,false,false);

			playerArm1.SetAttr(groupMatrix, renderAs, shaderProgram);
			playerArm1.DrawArm();
			racket1.SetAttr(groupMatrix, renderAs, shaderProgram, playerArm1.partParent);
			racket1.Draw();

            playerArm2.SetAttr(groupMatrix, renderAs, shaderProgram);
            playerArm2.DrawArm();
            racket2.SetAttr(groupMatrix, renderAs, shaderProgram, playerArm2.partParent);
            racket2.Draw();
			ball.setGroupMatrix(groupMatrix);
			ball.setRenderAs(renderAs);
			ball.drawBall();

            SceneObj.sphereVao = unitSphereAO;
            SceneObj.sphereVertCount = vertexIndicessphere.size();
            SceneObj.SetAttr(rotationMatrixW, renderAs, shaderProgram);
            SceneObj.SetVAO(unitCubeAO, gridAO);
            SceneObj.DrawScene(false);  // Draw scene without the skybox, so it can't be used to make shadows on the scene

			glm::mat4 letterTranslate;
			glm::mat4 letterRotate;
			glm::mat4 letterScale;
			glm::mat4 LetterGroupMatrix;
			letterTranslate = glm::translate(glm::mat4(1.0f), vec3(0.25,0,-0.5));
			letterRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)00), glm::vec3(.0f, .0f, 1.0f));
			letterRotate *= glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(1.0f, .0f, .0f));
			letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.00015f, .00015f, .00015f)*3.0f );
			glm::mat4 letterParent = letterTranslate * letterScale * letterRotate;
			LetterGroupMatrix = groupMatrix * letterParent;

			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &LetterGroupMatrix[0][0]);
			Bleachers.RenderModel();

		}

		{ // 2nd pass
			//reset 
			glUniform1i(applyTexturesLocation, shouldApplyTextures);
			glUniform1i(applyShadowsLocation, false);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			glViewport(0, 0, width, height);

			// Clear color and depth data on frame buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glActiveTexture(GL_TEXTURE0 + 2);
			glBindTexture(GL_TEXTURE_2D, depth_map_texture);

			playerArm1.SetAttr(groupMatrix, renderAs, shaderProgram);
			playerArm1.DrawArm();
			racket1.SetAttr(groupMatrix, renderAs, shaderProgram, playerArm1.partParent);
			racket1.Draw();

            playerArm2.SetAttr(groupMatrix, renderAs, shaderProgram);
            playerArm2.DrawArm();
            racket2.SetAttr(groupMatrix, renderAs, shaderProgram, playerArm2.partParent);
            racket2.Draw();

			ball.setGroupMatrix(groupMatrix);
			ball.setRenderAs(renderAs);
			ball.drawBall();


			numberDraw.Scoreboard(number, false, true);
			numberDraw2.Scoreboard(number, false, false);

			SceneObj.sphereVao = unitSphereAO;
			SceneObj.sphereVertCount = vertexIndicessphere.size();
			SceneObj.SetAttr(rotationMatrixW, renderAs, shaderProgram);
			SceneObj.SetVAO(unitCubeAO, gridAO);
			SceneObj.DrawScene(true);  // Draw scene with the skybox

			//rename all this
			//****************
			glm::mat4 letterTranslate;
			glm::mat4 letterRotate;
			glm::mat4 letterScale;
			glm::mat4 LetterGroupMatrix;
			letterTranslate = glm::translate(glm::mat4(1.0f), vec3(0.35, -0.02, -0.75));
			letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.00015f, .00015f, .00015f)*6.0f);
			glm::mat4 letterParent = letterTranslate * letterScale;
			LetterGroupMatrix = groupMatrix * letterParent;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &LetterGroupMatrix[0][0]);

			//these texture and colour dont affect bleacher.rendermodel
			skinMaterial.loadToShader();
			skinMaterial.bindTexture();
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.94f, .76f, .5f))); //al have the same colour

			Bleachers.RenderModel();
			letterTranslate = glm::translate(glm::mat4(1.0f), vec3(-0.35, -0.02, -0.75));
			letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.00015f, .00015f, .00015f) * 6.0f);
			letterParent = letterTranslate * letterScale;
			LetterGroupMatrix = groupMatrix * letterParent;
			skinMaterial.loadToShader();
			skinMaterial.bindTexture();
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &LetterGroupMatrix[0][0]);
			Bleachers.RenderModel();
			//******************
			updateLight(glm::vec3(x, lightDepth, z), glm::vec3(0, 0, 0), SceneObj, shaderProgram, i, noshowLightBox);
		}

		//crowd.drawCrowd();
		crowd.drawSingle(vec3(-.750,.25,0),vec3(0));
		crowd.drawSingle(vec3(-0.65, .25, 0), vec3(0));
		crowd.test(4);
		//blue side is Player1
		playerArm1.flexFingers();
		//red is player2
        playerArm2.flexFingers();

		if (glfwGetTime() >= 3 && glfwGetTime() < 3.1 && !soundPlayed)
		{
			ball.playSound();
			soundPlayed = true;
		}
		if (glfwGetTime() >= 3.2 && glfwGetTime() < 3.3)
		{
			soundPlayed = false;
		}
		if (glfwGetTime() >= 6 && glfwGetTime() < 6.1 && !soundPlayed)
		{
			ball.playSound();
			soundPlayed = true;
		}
		if (glfwGetTime() >= 6.2 && glfwGetTime() < 6.3)
		{
			soundPlayed = false;
		}
		if (glfwGetTime() >= 9 && glfwGetTime() < 9.1 && !soundPlayed)
		{
			SceneObj.playCrowdSound(true);
			soundPlayed = true;
		}
		if (glfwGetTime() >= 9.2 && glfwGetTime() < 9.3)
		{
			soundPlayed = false;
		}
		if (glfwGetTime() >= 16 && glfwGetTime() < 16.1 && !soundPlayed)
		{
			ball.playSound();
			soundPlayed = true;
		}
		if (glfwGetTime() >= 16.2 && glfwGetTime() < 16.3)
		{
			soundPlayed = false;
		}
		if (glfwGetTime() >= 19 && glfwGetTime() < 19.1 && !soundPlayed)
		{
			ball.playSound();
			soundPlayed = true;
		}
		if (glfwGetTime() >= 19.2 && glfwGetTime() < 19.3)
		{
			soundPlayed = false;
		}
		if (glfwGetTime() >= 22 && glfwGetTime() < 22.1 && !soundPlayed)
		{
			SceneObj.playCrowdSound(false);
			soundPlayed = true;
		}
		if (glfwGetTime() >= 22.2 && glfwGetTime() < 22.3)
		{
			soundPlayed = false;
		}
		if (glfwGetTime() >= 28 && glfwGetTime() < 28.1 && !soundPlayed)
		{
			ball.playSound();
			soundPlayed = true;
		}
		if (glfwGetTime() >= 28.2 && glfwGetTime() < 28.3)
		{
			soundPlayed = false;
		}
		if (glfwGetTime() >= 31 && glfwGetTime() < 31.1 && !soundPlayed)
		{
			ball.playSound();
			soundPlayed = true;
		}
		if (glfwGetTime() >= 31.2 && glfwGetTime() < 31.3)
		{
			soundPlayed = false;
		}
		if (glfwGetTime() >= 34 && glfwGetTime() < 34.1 && !soundPlayed)
		{
			ball.playSound();
			soundPlayed = true;
		}
		if (glfwGetTime() >= 34.2 && glfwGetTime() < 34.3)
		{
			soundPlayed = false;
		}
		if (glfwGetTime() >= 37 && glfwGetTime() < 37.1 && !soundPlayed)
		{
			ball.playSound();
			soundPlayed = true;
		}
		if (glfwGetTime() >= 37.2 && glfwGetTime() < 37.3)
		{
			soundPlayed = false;
		}
		if (glfwGetTime() >= 40 && glfwGetTime() < 40.1 && !soundPlayed)
		{
			SceneObj.playCrowdSound(true);
			soundPlayed = true;
		}
		if (glfwGetTime() >= 40.2 && glfwGetTime() < 40.3)
		{
			soundPlayed = false;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
    // Shut down irrklang
    audioEngine->drop();

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
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// Check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cerr << "Error. Vertex shader compilation failed!\n" << infoLog << std::endl;
	}

	// Fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const std::string fss = getShaderSource(fragment);
	const char* fragmentShaderSource = fss.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// Check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
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
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
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
void setProjectionMatrix(int shaderProg, glm::mat4 projectionMatrix)
{
	glUseProgram(shaderProgram);
	GLint projectionMatrixLocation = glGetUniformLocation(shaderProg, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void setViewMatrix(int shaderProg, glm::mat4 viewMatrix)
{
	glUseProgram(shaderProgram);
	GLint viewMatrixLocation = glGetUniformLocation(shaderProg, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
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
	int state_W = glfwGetKey(window, GLFW_KEY_W);
	int state_A = glfwGetKey(window, GLFW_KEY_A);
	int state_S = glfwGetKey(window, GLFW_KEY_S);
	int state_D = glfwGetKey(window, GLFW_KEY_D);
    int state_I = glfwGetKey(window, GLFW_KEY_I);
    int state_J = glfwGetKey(window, GLFW_KEY_J);
    int state_K = glfwGetKey(window, GLFW_KEY_K);
    int state_L = glfwGetKey(window, GLFW_KEY_L);
	int state_UP = glfwGetKey(window, GLFW_KEY_UP);
	int state_DOWN = glfwGetKey(window, GLFW_KEY_DOWN);
	int state_LEFT = glfwGetKey(window, GLFW_KEY_LEFT);
	int state_RIGHT = glfwGetKey(window, GLFW_KEY_RIGHT);
	int state_HOME = glfwGetKey(window, GLFW_KEY_HOME);
	int state_P = glfwGetKey(window, GLFW_KEY_P);
	int state_T = glfwGetKey(window, GLFW_KEY_T);
	int state_B = glfwGetKey(window, GLFW_KEY_B);
	int state_X = glfwGetKey(window, GLFW_KEY_X);
	int state_1 = glfwGetKey(window, GLFW_KEY_1);
	int state_2 = glfwGetKey(window, GLFW_KEY_2);
    int state_COMMA = glfwGetKey(window, GLFW_KEY_COMMA);
    int state_PERIOD = glfwGetKey(window, GLFW_KEY_PERIOD);

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

    if ((state_D == GLFW_PRESS) && mods != GLFW_MOD_SHIFT)
        playerArm1.setTranslateModel(glm::vec3((playerArm1.getTranslateModel().x - .005f), playerArm1.getTranslateModel().y, playerArm1.getTranslateModel().z));
    else if ((state_A == GLFW_PRESS) && mods != GLFW_MOD_SHIFT)
        playerArm1.setTranslateModel(glm::vec3((playerArm1.getTranslateModel().x + .005f), playerArm1.getTranslateModel().y, playerArm1.getTranslateModel().z));
    else if ((state_A == GLFW_PRESS) && mods == GLFW_MOD_SHIFT)
        switch (selectJoint) {
            case(0): playerArm1.setRotation(playerArm1.getRotation() + 5);  break;
            case(1):if (playerArm1.getERotation() + 5 > 90)playerArm1.setERotation(90); else  playerArm1.setERotation(playerArm1.getERotation() + 5);  break;
            case(2):if (playerArm1.getWRotation() + 5 > 65)playerArm1.setWRotation(65); else  playerArm1.setWRotation(playerArm1.getWRotation() + 5); break;
            default: break;
        }
    else if ((state_D == GLFW_PRESS) && mods == GLFW_MOD_SHIFT)
        switch (selectJoint) {
            case(0): playerArm1.setRotation(playerArm1.getRotation() - 5);  break;
            case(1):if (playerArm1.getERotation() - 5 < 0)playerArm1.setERotation(0); else  playerArm1.setERotation(playerArm1.getERotation() - 5);  break;
            case(2):if (playerArm1.getWRotation() - 5 < -85)playerArm1.setWRotation(-85); else  playerArm1.setWRotation(playerArm1.getWRotation() - 5); break;
            default: break;
        }

    else if ((state_J == GLFW_PRESS) && mods != GLFW_MOD_SHIFT)
        playerArm2.setTranslateModel(glm::vec3((playerArm2.getTranslateModel().x - .005f), playerArm2.getTranslateModel().y, playerArm2.getTranslateModel().z));
    else if ((state_L == GLFW_PRESS) && mods != GLFW_MOD_SHIFT)
        playerArm2.setTranslateModel(glm::vec3((playerArm2.getTranslateModel().x + .005f), playerArm2.getTranslateModel().y, playerArm2.getTranslateModel().z));
    else if ((state_L == GLFW_PRESS) && mods == GLFW_MOD_SHIFT)
        switch (selectJoint) {
            case(0): playerArm2.setRotation(playerArm2.getRotation() + 5);  break;
            case(1):if (playerArm2.getERotation() + 5 > 90)playerArm2.setERotation(90); else  playerArm2.setERotation(playerArm2.getERotation() + 5);  break;
            case(2):if (playerArm2.getWRotation() + 5 > 65)playerArm2.setWRotation(65); else  playerArm2.setWRotation(playerArm2.getWRotation() + 5); break;
            default: break;
        }
    else if ((state_J == GLFW_PRESS) && mods == GLFW_MOD_SHIFT)
        switch (selectJoint) {
            case(0): playerArm2.setRotation(playerArm2.getRotation() - 5);  break;
            case(1):if (playerArm2.getERotation() - 5 < 0)playerArm2.setERotation(0); else  playerArm2.setERotation(playerArm2.getERotation() - 5);  break;
            case(2):if (playerArm2.getWRotation() - 5 < -85)playerArm2.setWRotation(-85); else  playerArm2.setWRotation(playerArm2.getWRotation() - 5); break;
            default: break;
        }

	// If ESC is pressed, window should close
	else if (state_ESC == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	else if (state_TAB == GLFW_PRESS && mods != GLFW_MOD_SHIFT) {
		/*
		select -1 is original model micah
		0 is second model matthew
		1 is third model evan
		2 is fouth jonah
		3 is five Noot
		*/
		if (selectModel == 0) selectModel += 1;
		else if (selectModel == 1) selectModel = 0;
		else selectModel += 1;
		//printf("selectModel is: %d\n", selectModel);
		switch (selectModel) {
		case(0):printf("selectModel is: %d , selected Player 1 \n", selectModel); break;
		case(1):printf("selectModel is: %d , selected Player 2 \n", selectModel); break;

		default: break;
		}
	}

	else if (state_TAB == GLFW_PRESS && mods == GLFW_MOD_SHIFT) {
		/*
		select -1 is bicep
		0 is elbow
		1 is wrist y
		2 is wrist x?
		*/
		if (selectJoint == 0) selectJoint += 1; 
		else if (selectJoint == 2) selectJoint = 0;
		else selectJoint += 1;
		switch (selectJoint) {
		case(0):printf("selectJoint is: %d , selected shoulder\n", selectJoint); break;
		case(1):printf("selectJoint is: %d , selected elbow\n", selectJoint); break;
		case(2):printf("selectJoint is: %d , selected wrist\n", selectJoint); break;
		default: break;
		}
		//printf("selectJoint is: %d\n", selectJoint);
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

	// If p, l, or t is pressed, changed render mode between points, lines, and triangles, respectively
	else if (state_P == GLFW_PRESS)
		renderAs = GL_POINTS;

	else if (state_T == GLFW_PRESS)
		renderAs = GL_TRIANGLES;

	// If HOME is pressed, remove translations, rotations, and scalings
	else if (state_HOME == GLFW_PRESS) {		
		playerArm1.resetArm();
		GroupMatrixScale = glm::vec3(1.0f);
		rotationMatrixW = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 InitviewMatrix = glm::lookAt(eye, center, up);
		glm::mat4 projectionMatrix = glm::perspective(FOV, AR, near, far);
		setProjectionMatrix(shaderProgram, projectionMatrix);
		setViewMatrix(shaderProgram, InitviewMatrix);
	}

	// If b is pressed, toggle shadows
	else if (state_B == GLFW_PRESS)
		shouldApplyShadows = !shouldApplyShadows;

	// If x is pressed, toggle textures
	else if (state_X == GLFW_PRESS)
		shouldApplyTextures = !shouldApplyTextures;

    // If comma is pressed, play a ball sound
    else if (state_COMMA == GLFW_PRESS)
        ball.playSound();

    // If period is pressed, play a crowd sound. Male normally, female with SHIFT
    else if (state_PERIOD == GLFW_PRESS)
    {
        if (mods != GLFW_MOD_SHIFT)
            SceneObj.playCrowdSound(true);
        else
            SceneObj.playCrowdSound(false);
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

		if (dy < 0)
			translateY += .005;
		else if (dy > 0)
			translateY -= .005;

		glm::mat4 InitviewMatrix = glm::lookAt(eye, glm::vec3(translateW, translateY, 0.0f), up);
		setViewMatrix(shaderProgram, InitviewMatrix);

		lastMousePosY = yPos;
	}

	else if (state_RIGHT == GLFW_PRESS)
	{
		double dx = xPos - lastMousePosX;

		if (dx < 0)
			translateW -= .005;
		else if (dx > 0)
			translateW += .005;

		glm::mat4 InitviewMatrix = glm::lookAt(eye, glm::vec3(translateW + center.x, translateY + center.y, 0.0f), up);
		setViewMatrix(shaderProgram, InitviewMatrix);
		lastMousePosX = xPos;
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

//void windowSizeCallback(GLFWwindow* window, int width, int height)
//{
//	glfwSetWindowSize(window, width, height);
//}

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
			if (count == 3&& indexCount==3) {
				//my_normals.push_back(temp_vertices[0]);
				//my_normals.push_back(temp_vertices[1]);
				//my_normals.push_back(temp_vertices[2]);
				my_normals.push_back(normalize(cross(temp_vertices[1] - temp_vertices[0], temp_vertices[2] - temp_vertices[0])));
				my_normals.push_back(normalize(cross(temp_vertices[1] - temp_vertices[0], temp_vertices[2] - temp_vertices[0])));
				my_normals.push_back(normalize(cross(temp_vertices[1] - temp_vertices[0], temp_vertices[2] - temp_vertices[0])));
				count = 0;
			}
			else if (count==3 ){
				int IndC = indexCount - 3;
					int IndB = indexCount - 2;
					int IndA = indexCount - 1;
				my_normals.push_back(normalize(cross(temp_vertices[indexCount-2] - temp_vertices[indexCount-3], temp_vertices[indexCount-1] - temp_vertices[indexCount - 3])));
				my_normals.push_back(normalize(cross(temp_vertices[indexCount - 2] - temp_vertices[indexCount - 3], temp_vertices[indexCount-1] - temp_vertices[indexCount - 3])));
				my_normals.push_back(normalize(cross(temp_vertices[indexCount - 2] - temp_vertices[indexCount - 3], temp_vertices[indexCount-1] - temp_vertices[indexCount - 3])));
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

