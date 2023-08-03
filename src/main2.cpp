// Comp 371 - Quiz 2

// System includes
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
#include "SceneObjects.h"
#include "matt-models/MattModel.h"

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

//void updateLight(glm::vec3 newPosition,glm::vec3 newFocus,SceneObjects SceneObj,GLuint shaderProgram,float i,bool applyShadow) {
//	// light parameters
//	glm::vec3 lightPosition(-.30f, .30f, .0f); // the location of the light in 3D space
//	glm::vec3 lightFocus(0.0, 0.0, -1.0);      // the point in 3D space the light "looks" at
//	glm::vec3 lightDirection = glm::normalize(newFocus - newPosition);
//
//	GLint lightPositionLoc = glGetUniformLocation(shaderProgram, "lightPosition");
//	GLint lightDirectionLoc = glGetUniformLocation(shaderProgram, "lightDirection");
//	GLint lightViewProjMatrixLoc = glGetUniformLocation(shaderProgram, "lightViewProjMatrix");
//
//	// Set light position on scene shader
//	glUniform3fv(lightPositionLoc, 1, &newPosition[0]);
//
//	// Set light direction on scene shader
//	glUniform3fv(lightDirectionLoc, 1, &lightDirection[0]);
//	float lightNearPlane = 0.1f;
//	float lightFarPlane = 180.0f;
//
//	glm::mat4 lightProjectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, lightNearPlane, lightFarPlane);
//	glm::mat4 lightViewMatrix = glm::lookAt(newPosition, newFocus, glm::vec3(0.0f, 1.0f, 0.0f));
//	glm::mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;
//	glUniformMatrix4fv(lightViewProjMatrixLoc, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
//	if (!applyShadow) {
//		SceneObj.DrawLight(newPosition, glm::vec3(0.0f, 1.0f, 0.0f), i);
//	}
//}

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

// Set aspect ratio of the window
const int WIDTH = 1024, HEIGHT = 768;

// Set/ declare some variables
glm::vec3 eye(.7650f, .250f, .7650f);
glm::vec3 center(.00f, .0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 translateWSAD(0.0f, 0.0f, 0.0f);
glm::vec3 Translate(.0f, .0f, .0f);
glm::vec3 GroupMatrixScale(1.0f, 1.0f, 1.0f);
glm::mat4 groupMatrix;
glm::mat4 rotationMatrixW = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

// Create 2 models
MattModel mattModel1(1);
MattModel mattModel2(2);

//glm::vec3 mattTranslationRandom(.0f, .0f, .0f);
//glm::vec3 mattTranslationModel(.0f, .0f, .0f);
//float mattRotationAngle = 0.0f;

int renderAs = GL_TRIANGLES;
int shaderProgram;
double lastMousePosX, lastMousePosY, lastMousePosZ;
float FOV = 70, AR = (float)(WIDTH / HEIGHT), near = .01, far = 50;
float translateW = 0, translateY = 0, translateZ = 0;

// Toggles for shadows and textures
bool shouldApplyShadows = true;
bool shouldApplyTextures = true;

int selectModel = 0; // Which model is selected
int selectCamera = 0; // Which camera is selected
bool lightOn = true; // Whether the light is on or off

int main(int argc, char* argv[])
{
	// Initialize GLFW and OpenGL version
	if (!glfwInit())
		return -1;

	// Scale down the unit cube vertices
	for (size_t i = 0; i < 36; i++) {
		TexturedNormaledVertex thisOne = texturedCubeVertexArray[i];
		texturedCubeVertexArray[i].position *= 0.1f;
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

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Print OpenGL version
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	// Enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(messageCallback, 0);

	// Load the textures
	GLuint courtTextureID	= loadTexture("../src/Assets/clay2.jpg");
	GLuint ropeTextureID	= loadTexture("../src/Assets/rope.jpg");
	GLuint clothTextureID	= loadTexture("../src/Assets/cloth.jpg");
	GLuint metalTextureID	= loadTexture("../src/Assets/metal.jpg");
	GLuint grassTextureID   = loadTexture("../src/Assets/grass4.jpg");
	GLuint plasticTextureID = loadTexture("../src/Assets/plastic.jpg");
	GLuint woodTextureID    = loadTexture("../src/Assets/wood1.jpg");

	// Black background	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Set frame rate to refresh rate of monitor
	glfwSwapInterval(1);

	// Compile and link shaders here
	shaderProgram = compileAndLinkShaders(vertex, fragment);

	// Initialize uniform locations
	glUseProgram(shaderProgram);
	GLuint worldMatrixLocation      = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation       = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint colorLocation            = glGetUniformLocation(shaderProgram, "objectColor");
    GLint viewPositionLocation      = glGetUniformLocation(shaderProgram, "viewPosition");
    GLint applyTexturesLocation     = glGetUniformLocation(shaderProgram, "shouldApplyTexture");
    GLint applyShadowsLocation      = glGetUniformLocation(shaderProgram, "shouldApplyShadows");
    GLint lightOnLocation           = glGetUniformLocation(shaderProgram, "lightOn");

	//maybe update this for shadows
    glUniform3fv(viewPositionLocation, 1, &eye[0]);
	
	//Scene
	SceneObjects SceneObj("scene");
	SceneObj.InitGrid();
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
	lastMousePosZ = lastMousePosY;

	// Initialize projection and view matrices
	glm::mat4 projectionMatrix = glm::perspective(FOV, AR, near, far);

	glm::mat4 InitviewMatrix = glm::lookAt(eye, center, up);

	setProjectionMatrix(shaderProgram, projectionMatrix);
	setViewMatrix(shaderProgram, InitviewMatrix);

	std::vector<glm::vec2> UVscube;
	
	// Create VAOs
	int gridAO = createVertexArrayObject(SceneObj.lineArray, sizeof(SceneObj.lineArray));
	int unitCubeAO = createVertexArrayObject2();

	//TEXTURE DEFINITION
	Material courtMaterial(.2f, .20f, 1.0f, .1f, courtTextureID, shaderProgram);
	Material ropeMaterial(.5f, .60f, .5f, .9f, ropeTextureID, shaderProgram);
	Material clothMaterial(.5f, .60f, .5f, .9f, clothTextureID, shaderProgram);
	Material metalMaterial(.6f, .90f, .6f, .00012f, metalTextureID, shaderProgram);
	Material grassMaterial(.2f, .20f, .7f, 1.5f, grassTextureID, shaderProgram);
	Material plasticMaterial(.5f, .60f, .5f, .002f, plasticTextureID, shaderProgram);
	Material woodMaterial(.5f, .60f, .5f, .002f, woodTextureID, shaderProgram);

	SceneObj.setMaterials(courtMaterial, clothMaterial, ropeMaterial, metalMaterial, grassMaterial, plasticMaterial);

	// Set mouse and keyboard callbacks
	glfwSetKeyCallback(window, keyPressCallback);
	glfwSetCursorPosCallback(window, mouseCursorPostionCallback);

	glm::vec3 modelScale(0.25, 0.25, 0.25);

    // Lighting
    float lightAngleOuter = 30.0;
    float lightAngleInner = 20.0;
    // Set light cutoff angles on scene shader
    GLint lightCutoffInnerLoc = glGetUniformLocation( shaderProgram, "lightCutoffInner");
    GLint lightCutoffOuterLoc = glGetUniformLocation( shaderProgram, "lightCutoffOuter");
    glUniform1f(lightCutoffInnerLoc, cos(glm::radians(lightAngleInner)));
    glUniform1f(lightCutoffOuterLoc, cos(glm::radians(lightAngleOuter)));

    GLint lightColorLoc = glGetUniformLocation( shaderProgram, "lightColor");

    glUniform3fv(lightColorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

    // light parameters
    glm::vec3 lightPosition(0.0f, 2.5f, 0.3f); // the location of the light in 3D space
	glm::vec3 lightFocus(0.0, 0.0, -0.1);      // the point in 3D space the light "looks" at
	glm::vec3 lightDirection = glm::normalize(lightFocus - lightPosition);

    float lightNearPlane = 0.001f;
    float lightFarPlane = 50.0f;

	glm::mat4 lightProjectionMatrix = glm::ortho(-.7f, .70f, -.70f, .70f, lightNearPlane, lightFarPlane);
	glm::mat4 lightViewMatrix = glm::lookAt(lightPosition, lightFocus, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;

	// Get lighting-related uniform locations
    GLint lightViewProjMatrixLoc = glGetUniformLocation( shaderProgram, "lightViewProjMatrix");
    GLint lightNearPlaneLoc = glGetUniformLocation( shaderProgram, "lightNearPlane");
    GLint lightFarPlaneLoc = glGetUniformLocation( shaderProgram, "lightFarPlane");
    GLint lightPositionLoc = glGetUniformLocation( shaderProgram, "lightPosition");
    GLint lightDirectionLoc = glGetUniformLocation( shaderProgram, "lightDirection");

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
	GLuint kdepthMap = glGetUniformLocation(shaderProgram, "shadowMap");
	glUniform1i(kdepthMap, 2);

	//float lastFrameTime = glfwGetTime();

	// Set Matt VAO and shader program
	mattModel1.setShaderProgram(shaderProgram);
	mattModel1.setVAO(unitCubeAO);
	mattModel2.setShaderProgram(shaderProgram);
	mattModel2.setVAO(unitCubeAO);

	//float i = -1;
	float spin = 0;
	bool reverse = false;
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		groupMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), GroupMatrixScale) * rotationMatrixW;

		float lightDepth = 1.0f;
		bool noshowLightBox = false;
		//float x = sin(i);
		//float z = cos(i);
		//i += .02;
		//glm::scale(glm::mat4(1.0f), GroupMatrixScale) * rotationMatrixW;
		//glm::vec3 MattTranslation = mattTranslationModel + mattTranslationRandom;

		//lastFrameTime = glfwGetTime();

		// Must draw scene in 2 passes: once for shadows, and another normally
		// 1st pass
		{
			glUniform1i(applyTexturesLocation, false);
			glUniform1i(applyShadowsLocation, shouldApplyShadows);
			glUniform1i(lightOnLocation, lightOn);

			// Use proper image output size
			glViewport(0, 0, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE);
			// Bind depth map texture as output frame buffer
			glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
			// Clear depth data on the frame buffer
			glClear(GL_DEPTH_BUFFER_BIT);

			//Note .8 != 30 for the height so that we can visually see the affects of rotating
			//updateLight(glm::vec3(x, lightDepth, z), glm::vec3(0, 0, 0), SceneObj, shaderProgram, i, true);
			//if (i == 1.0f) i = -1.0f;

			// Draw geometry
			mattModel1.setGroupMatrix(groupMatrix);
			mattModel1.drawModel();
			mattModel2.setGroupMatrix(groupMatrix);
			mattModel2.drawModel();

            SceneObj.SetAttr(rotationMatrixW, GL_TRIANGLES, shaderProgram);
            SceneObj.SetVAO(unitCubeAO, gridAO);
            SceneObj.DrawScene(false);  // Draw scene without the skybox, so it can't be used to make shadows on the scene
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

			// Draw geometry
			mattModel1.setGroupMatrix(groupMatrix);
			mattModel1.drawModel();
			mattModel2.setGroupMatrix(groupMatrix);
			mattModel2.drawModel();

			SceneObj.SetAttr(rotationMatrixW, GL_TRIANGLES, shaderProgram);
			SceneObj.SetVAO(unitCubeAO, gridAO);
			SceneObj.DrawScene(true);  // Draw scene with the skybox

			//updateLight(glm::vec3(x, lightDepth, z), glm::vec3(0, 0, 0), SceneObj, shaderProgram, i, noshowLightBox);
		}
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

/**
GLFW callback function for handling keyboard inputs
*/
void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Get states of each relevant key
	int state_ESC = glfwGetKey(window, GLFW_KEY_ESCAPE);
	int state_SPACE = glfwGetKey(window, GLFW_KEY_SPACE);
	int state_U = glfwGetKey(window, GLFW_KEY_U);
	int state_J = glfwGetKey(window, GLFW_KEY_J);
	int state_W = glfwGetKey(window, GLFW_KEY_W);
	int state_A = glfwGetKey(window, GLFW_KEY_A);
	int state_S = glfwGetKey(window, GLFW_KEY_S);
	int state_D = glfwGetKey(window, GLFW_KEY_D);
	int state_UP = glfwGetKey(window, GLFW_KEY_UP);
	int state_DOWN = glfwGetKey(window, GLFW_KEY_DOWN);
	int state_LEFT = glfwGetKey(window, GLFW_KEY_LEFT);
	int state_RIGHT = glfwGetKey(window, GLFW_KEY_RIGHT);
	int state_HOME = glfwGetKey(window, GLFW_KEY_HOME);
	int state_L = glfwGetKey(window, GLFW_KEY_L);
	int state_M = glfwGetKey(window, GLFW_KEY_M);
	int state_R = glfwGetKey(window, GLFW_KEY_R);
	int state_B = glfwGetKey(window, GLFW_KEY_B);
	int state_X = glfwGetKey(window, GLFW_KEY_X);
	int state_1 = glfwGetKey(window, GLFW_KEY_1);
	int state_2 = glfwGetKey(window, GLFW_KEY_2);
	
	//global random
	float number1 = (rand()) / (float)(RAND_MAX);
	float number2 = (rand()) / (float)(RAND_MAX);
	float number3 = (rand()) / (float)(RAND_MAX);
	// Constrain to visible grid locations
	if (number1 >= .75f)
		number1 = number1 / (float)(RAND_MAX);
	else if (number2 >= .25f)
		number2 = number2 / (float)(RAND_MAX);
	else if (number3 >= .75f)
		number3 = number3 / (float)(RAND_MAX);

	if (state_1 == GLFW_PRESS)
		selectModel = 0;
	else if (state_2 == GLFW_PRESS)
		selectModel = 1;
	
	// Use m to toggle between selected cameras
	else if (state_M == GLFW_PRESS)
	{
		if (selectCamera == 3)
			selectCamera = 0;
		else
			selectCamera++;
	}

	// Use r to return to main camera
	else if (state_R == GLFW_PRESS)
	{
		selectCamera = 0;
	}

	// Translate the models
	else if (state_W == GLFW_PRESS) {
		if (selectModel == 0)
			mattModel1.setTranslationModel(mattModel1.getTranslationModel() + glm::vec3(0.0f, 0.005f, 0.0f));
		else if (selectModel == 1)
			mattModel2.setTranslationModel(mattModel2.getTranslationModel() + glm::vec3(0.0f, 0.005f, 0.0f));
		//mattTranslationModel.y += .005f;
	}
	else if (state_S == GLFW_PRESS) {
		if (selectModel == 0)
			mattModel1.setTranslationModel(mattModel1.getTranslationModel() - glm::vec3(0.0f, 0.005f, 0.0f));
		else if (selectModel == 1)
			mattModel2.setTranslationModel(mattModel2.getTranslationModel() - glm::vec3(0.0f, 0.005f, 0.0f));
		//mattTranslationModel.y -= .005f;
	}
	else if ((state_D == GLFW_PRESS) && mods == GLFW_MOD_SHIFT) {
		if (selectModel == 0)
			mattModel1.setTranslationModel(mattModel1.getTranslationModel() + glm::vec3(0.005f, 0.0f, 0.0f));
		else if (selectModel == 1)
			mattModel2.setTranslationModel(mattModel2.getTranslationModel() + glm::vec3(0.005f, 0.0f, 0.0f));
		//mattTranslationModel.x += .005f;
	}
	else if ((state_A == GLFW_PRESS) && mods == GLFW_MOD_SHIFT) {
		if (selectModel == 0)
			mattModel1.setTranslationModel(mattModel1.getTranslationModel() - glm::vec3(0.005f, 0.0f, 0.0f));
		else if (selectModel == 1)
			mattModel2.setTranslationModel(mattModel2.getTranslationModel() - glm::vec3(0.005f, 0.0f, 0.0f));
		//mattTranslationModel.x -= .005f;
	}
	else if ((state_A == GLFW_PRESS) && mods != GLFW_MOD_SHIFT) {
		if (selectModel == 0)
			mattModel1.setRotationAngle(mattModel1.getRotationAngle() + 5.0f);
		else if (selectModel == 1)
			mattModel2.setRotationAngle(mattModel2.getRotationAngle() + 5.0f);
		//mattRotationAngle += 5.0f;
	}
	else if ((state_D == GLFW_PRESS) && mods != GLFW_MOD_SHIFT) {
		if (selectModel == 0)
			mattModel1.setRotationAngle(mattModel1.getRotationAngle() - 5.0f);
		else if (selectModel == 1)
			mattModel2.setRotationAngle(mattModel2.getRotationAngle() + -5.0f);
		//mattRotationAngle -= 5.0f;
	}

	// If SPACE is pressed, translate randomly around the scene
	else if (state_SPACE == GLFW_PRESS) {
		if (selectModel == 0)
			mattModel1.setTranslationRandom(glm::vec3(number1, number2, number3));
		else if (selectModel == 1)
			mattModel2.setTranslationRandom(glm::vec3(number1, number2, number3));
		//mattTranslationRandom = glm::vec3(number1, number2, number3);
	}

	// If ESC is pressed, window should closed
	else if (state_ESC == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// If u or j is pressed, scale up or down accordingly
	else if (state_U == GLFW_PRESS)
		GroupMatrixScale += .05f;

	else if (state_J == GLFW_PRESS)
		GroupMatrixScale -= .05f;

	// If the arrow keys are pressed, rotate accordingly
	else if (state_LEFT == GLFW_PRESS)
		rotationMatrixW *= glm::rotate(glm::mat4(1.0f), glm::radians(2.55f), glm::vec3(1.0f, 0.0f, 0.0f));

	else if (state_RIGHT == GLFW_PRESS)
		rotationMatrixW *= glm::rotate(glm::mat4(1.0f), glm::radians(2.55f), glm::vec3(-1.0f, 0.0f, 0.0f));

	else if (state_UP == GLFW_PRESS)
		rotationMatrixW *= glm::rotate(glm::mat4(1.0f), glm::radians(2.55f), glm::vec3(.0f, 1.0f, 0.0f));

	else if (state_DOWN == GLFW_PRESS)
		rotationMatrixW *= glm::rotate(glm::mat4(1.0f), glm::radians(2.55f), glm::vec3(.0f, -1.0f, 0.0f));

	// l is pressed turn light on or off
	else if (state_L == GLFW_PRESS)
		lightOn = !lightOn;

	// If HOME is pressed, remove translations, rotations, and scalings
	else if (state_HOME == GLFW_PRESS) {
		mattModel1.setTranslationModel(mattModel1.getTranslationModel() + (-1.0f * mattModel1.getTranslationModel()));
		mattModel2.setTranslationModel(mattModel2.getTranslationModel() + (-1.0f * mattModel2.getTranslationModel()));
		mattModel1.setTranslationRandom(mattModel1.getTranslationRandom() + (-1.0f * mattModel1.getTranslationRandom()));
		mattModel2.setTranslationRandom(mattModel2.getTranslationRandom() + (-1.0f * mattModel2.getTranslationRandom()));
		//mattTranslationModel += (-1.0f * mattTranslationModel);
		//mattTranslationRandom += (-1.0f * mattTranslationRandom);
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

