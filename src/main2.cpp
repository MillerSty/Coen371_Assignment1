// Comp 371 - Assignment 2

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
// Source code includes
#include "Arm.h"
#include "Racket.h"
#include "SceneObjects.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Evan-models/EvanArm.h"
#include "Evan-models/EvanRacket.h"

const char* vertex = "../src/shaders/vertexShader.glsl";
const char* fragment = "../src/shaders/fragmentShader.glsl";

const char* textureV = "../src/shaders/textureVShader.glsl";
const char* textureF = "../src/shaders/textureFShader.glsl";

const char* evanVertex = "../src/shaders/vertex.glsl";
const char* evanFragment = "../src/shaders/fragment.glsl";

int compileAndLinkShaders(const char* vertex, const char* fragment);
GLuint loadTexture(const char* filename);
void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam);
void setProjectionMatrix(int shaderProgram, glm::mat4 projectionMatrix);
void setViewMatrix(int shaderProgram, glm::mat4 viewMatrix);

void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCursorPostionCallback(GLFWwindow* window, double xPos, double yPos);

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

	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return vertexArrayObject;
}

// We use indices to create element buffer objects to increase efficiency
unsigned int indices[] = {
	4,2,0,
	2,7,3,
	6,5,7,
	1,7,5,
	0,3,1,
	4, 1,5,
	4,6,2,
	2,6,7,
	6,4,5,
	1,3,7,
	0,2,3,
	4,0, 1
};

// Reverse winding order indices
unsigned int reverseIndices[] = {
	4,0,2,
	2,3,7,
	6,7,5,
	1,5,7,
	0,1,3,
	4,5,1,
	4,2,6,
	2,7,6,
	6,5,4,
	1,7,3,
	0,3,2,
	4,1,0
};

// Create vertex buffer object for a unit cube
glm::vec3 unitcube[] = {
	glm::vec3(1.000000 ,1.000000  ,-1.000000),
	glm::vec3(1.000000 ,-1.000000 ,-1.000000),
	glm::vec3(1.000000 ,1.000000  , 1.000000),
	glm::vec3(1.000000 ,-1.000000 , 1.000000),
	glm::vec3(-1.000000,1.000000  , -1.00000),
	glm::vec3(-1.000000,-1.000000 , -1.00000),
	glm::vec3(-1.000000,1.000000  , 1.000000),
	glm::vec3(-1.000000,-1.000000 , 1.000000)
};

glm::vec3 cubeNormal[] = {
	glm::vec3(0.0000, 1.0000 , 0.0000),
	glm::vec3(0.0000, 0.0000, 1.0000),
	glm::vec3(-1.0000, 0.0000, 0.0000),
	glm::vec3(0.0000, -1.0000, 0.0000),
	glm::vec3(1.0000 ,0.0000 , 0.0000),
	glm::vec3(0.0000, 0.0000, -1.0000)
};
glm::vec2 cubeTexture[] = {
	glm::vec2(0.625000, 0.500000),
	glm::vec2(0.875000, 0.500000),
	glm::vec2(0.875000, 0.750000),
	glm::vec2(0.625000, 0.750000),
	glm::vec2(0.375000, 0.750000),
	glm::vec2(0.625000, 1.000000),
	glm::vec2(0.375000, 1.000000),
	glm::vec2(0.375000, 0.000000),
	glm::vec2(0.625000, 0.000000),
	glm::vec2(0.625000, 0.250000),
	glm::vec2(0.375000, 0.250000),
	glm::vec2(0.125000, 0.500000),
	glm::vec2(0.375000, 0.500000),
	glm::vec2(0.125000, 0.750000)

};


GLuint IBO;
int createVertexArrayElementObject(const glm::vec3* vertexArray, int arraySize, const glm::vec3* NormalArray, int NormalArraySize, const glm::vec2* TextureArray, int TextureArraySize, unsigned int indices[])
{
	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	int check = arraySize;
	glBufferData(GL_ARRAY_BUFFER, arraySize, vertexArray, GL_STATIC_DRAW);

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
	glBufferData(GL_ARRAY_BUFFER, NormalArraySize, NormalArray, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	//UVs VBO setup
	GLuint uvs_VBO;
	glGenBuffers(1, &uvs_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
	glBufferData(GL_ARRAY_BUFFER, TextureArraySize, TextureArray, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), (GLvoid*)0);
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return vertexArrayObject;
}

const int WIDTH = 1024, HEIGHT = 768;

// vec3 variables
glm::vec3 eye(.7650f, .250f, .7650f);
glm::vec3 center(.00f, .0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 translateWSAD(0.0f, 0.0f, 0.0f);
glm::vec3 Translate(.0f, .0f, .0f);
glm::vec3 GroupMatrixScale(1.0f, 1.0f, 1.0f);

// mat4 variables
glm::mat4 groupMatrix;
glm::mat4 rotationMatrixW = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

Arm arm;
int renderAs = GL_TRIANGLES;

int shaderProgram;
int textureProgram;

double lastMousePosX, lastMousePosY, lastMousePosZ;
float FOV = 70, AR = (float)(WIDTH / HEIGHT), near = .01, far = 50;
float translateW = 0, translateY = 0, translateZ = 0;

int main(int argc, char* argv[])
{
	// Initialize GLFW and OpenGL version
	if (!glfwInit())
		return -1;

	// Set some GLFW window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
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
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Print OpenGL version
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	// Load Textures
#if defined(__APPLE__) // NOTE Rez: Youll need to path the textures
#else
	// Enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(messageCallback, 0);
#endif

	GLuint courtTextureID = loadTexture("../src/Assets/clay2.jpg");
	GLuint ropeTextureID = loadTexture("../src/Assets/rope.jpg");
	GLuint clothTextureID = loadTexture("../src/Assets/cloth.jpg");
	GLuint metalTextureID = loadTexture("../src/Assets/metal.jpg");
	// Black background	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);

	// Set frame rate to refresh rate of monitor
	glfwSwapInterval(1);

	// Compile and link shaders here
	shaderProgram = compileAndLinkShaders(vertex, fragment);
	textureProgram = compileAndLinkShaders(textureV, textureF);

	for (int i = 0; i < 8; i++) {
		unitcube[i] = unitcube[i] * .05f; // This is to pre-scale the unit cube
	}

	// Initialize uniform locations
	glUseProgram(shaderProgram);
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	//Scene Jawn
	SceneObjects SceneObj("scene");
	SceneObj.InitGrid();
	//SO.courtTexture = courtTextureID;
	SceneObj.setTextures(courtTextureID, ropeTextureID, metalTextureID, clothTextureID);
	SceneObj.textureProgram = textureProgram;
	// General variables

	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
	lastMousePosZ = lastMousePosY;

	// Initialize projection and view matrices
	glm::mat4 projectionMatrix = glm::perspective(FOV,  // field of view in degrees
		AR,      // aspect ratio
		near, far);       // near and far (near > 0)

	glm::mat4 InitviewMatrix = glm::lookAt(eye, center, up);
	setProjectionMatrix(shaderProgram, projectionMatrix);
	setProjectionMatrix(textureProgram, projectionMatrix);
	setViewMatrix(shaderProgram, InitviewMatrix);
	setViewMatrix(textureProgram, InitviewMatrix);

	int gridAO = createVertexArrayObject(SceneObj.lineArray, sizeof(SceneObj.lineArray));
	int unitCubeAO = createVertexArrayElementObject(unitcube, sizeof(unitcube), cubeNormal, sizeof(cubeNormal), cubeTexture, sizeof(cubeTexture), indices);
	int reverseCubeAO = createVertexArrayElementObject(unitcube, sizeof(unitcube), cubeNormal, sizeof(cubeNormal), cubeTexture, sizeof(cubeTexture), reverseIndices);
	//arm(unitCubeAO, "arm");
	arm.setVAO(unitCubeAO);
	Racket racket(unitCubeAO, "racket");

	racket.jawnAngle = 0;

	int select = -1;
	int* newWidth = new int;
	int* newHeight = new int;

	// Set mouse and keyboard callbacks
	glfwSetKeyCallback(window, keyPressCallback);
	glfwSetCursorPosCallback(window, mouseCursorPostionCallback);

	int evanShaderProgram = compileAndLinkShaders(evanVertex, evanFragment);
	GLint evanWorldMatrixLocation = glGetUniformLocation(evanShaderProgram, "modelMatrix");
	GLint evanViewMatrixLocation = glGetUniformLocation(evanShaderProgram, "viewMatrix");
	GLint evanProjectionMatrixLocation = glGetUniformLocation(evanShaderProgram, "projectMatrix");
	glUseProgram(evanShaderProgram);
	glUniformMatrix4fv(evanViewMatrixLocation, 1, GL_FALSE, &InitviewMatrix[0][0]);
	glUniformMatrix4fv(evanProjectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	vec3 modelScale = vec3(0.03, 0.03, 0.03);
	EvanArm evanArm(vec3(0.2f, 0.0f, 0.0f), modelScale);
	EvanRacket evanRacket(vec3(0.2f, 0.0f, 0.0f), modelScale);

	//NOTE we have issues when doing mouse jawn with current set up
	while (!glfwWindowShouldClose(window))
	{
		// Handle resizing
		glfwGetWindowSize(window, newWidth, newHeight);
		glfwSetWindowSize(window, *newWidth, *newHeight);
		glViewport(0, 0, *newWidth, *newHeight);

		// Calculate aspect ratio
		AR = (float)*newWidth / (float)*newHeight; //note unsure if this will cause issues

		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw geometry
		glUseProgram(shaderProgram);



		// Set a default group matrix
		groupMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f)) *
			glm::scale(glm::mat4(1.0f), GroupMatrixScale) *
			rotationMatrixW;

		arm.SetAttr(groupMatrix, renderAs, shaderProgram);
		arm.setTranslation(Translate, translateWSAD);
		arm.DrawArm();
		racket.SetAttr(groupMatrix, renderAs, shaderProgram, arm.partParent);
		racket.Draw();

		evanArm.draw(evanWorldMatrixLocation, evanShaderProgram);
		evanRacket.draw(evanWorldMatrixLocation, evanShaderProgram);

		SceneObj.SetAttr(rotationMatrixW, renderAs, shaderProgram);
		SceneObj.SetVAO(unitCubeAO, reverseCubeAO, gridAO);
		SceneObj.DrawScene();

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

// loadTexture from lab04
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
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
		0, format, GL_UNSIGNED_BYTE, data);

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
	int state_P = glfwGetKey(window, GLFW_KEY_P);
	int state_L = glfwGetKey(window, GLFW_KEY_L);
	int state_T = glfwGetKey(window, GLFW_KEY_T);

	// If ESC is pressed, window should closed
	if (state_ESC == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// If SPACE is pressed, should reposition at random place on grid
	else if (state_SPACE == GLFW_PRESS)
	{
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

		int numZ = rand(), numX = rand(), numY = rand();
		int flZ = -1, flX = 1;

		if (numZ % 2 == 1) flZ *= -1;
		if (numX % 2 == 1) flX *= -1;
		Translate.x = number1;
		Translate.y = number2; Translate.z = number3;
	}

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

	else if (state_W == GLFW_PRESS)
		translateWSAD.y += .005;

	else if (state_S == GLFW_PRESS)
		translateWSAD.y -= .005;

	else if ((state_D == GLFW_PRESS) && mods == GLFW_MOD_SHIFT)
		translateWSAD.x -= .005;

	else if ((state_A == GLFW_PRESS) && mods == GLFW_MOD_SHIFT)
		translateWSAD.x += .005;

	else if ((state_A == GLFW_PRESS) && mods != GLFW_MOD_SHIFT)
		arm.setRotation(arm.getRotation() + 5);

	else if ((state_D == GLFW_PRESS) && mods != GLFW_MOD_SHIFT)
		arm.setRotation(arm.getRotation() - 5);

	// If p, l, or t is pressed, changed render mode between points, lines, and triangles, respectively
	else if (state_P == GLFW_PRESS)
		renderAs = GL_POINTS;

	else if (state_L == GLFW_PRESS)
		renderAs = GL_LINES;

	else if (state_T == GLFW_PRESS)
		renderAs = GL_TRIANGLES;

	// If HOME is pressed, remove translations, rotations, and scalings
	else if (state_HOME == GLFW_PRESS) {
		//or set each translate to 0
		Translate.x += -1 * Translate.x;
		Translate.y += -1 * Translate.y;
		Translate.z += -1 * Translate.z;
		translateWSAD.x += -1 * translateWSAD.x;
		translateWSAD.y += -1 * translateWSAD.y;
		arm.armRotate = 0;
		GroupMatrixScale = glm::vec3(1.0f);
		rotationMatrixW = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 InitviewMatrix = glm::lookAt(eye, center, up);
		glm::mat4 projectionMatrix = glm::perspective(FOV, AR, near, far);
		setProjectionMatrix(textureProgram, projectionMatrix);
		setProjectionMatrix(shaderProgram, projectionMatrix);
		setViewMatrix(textureProgram, InitviewMatrix);
		setViewMatrix(shaderProgram, InitviewMatrix);
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
		setViewMatrix(textureProgram, InitviewMatrix);
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
		setViewMatrix(textureProgram, InitviewMatrix);
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
		setProjectionMatrix(textureProgram, projectionMatrix);
		lastMousePosZ = zPos;
	}
}