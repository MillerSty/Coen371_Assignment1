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

const char* vertex = "../src/shaders/vertexShader.glsl";
const char* fragment = "../src/shaders/fragmentShader.glsl";

const char* textureV = "../src/shaders/textureVShader.glsl";
const char* textureF = "../src/shaders/textureFShader.glsl";
int compileAndLinkShaders(const char* vertex, const char* fragment);
GLuint loadTexture(const char* filename);
void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam);
void setProjectionMatrix(int shaderProgram, glm::mat4 projectionMatrix);
void setViewMatrix(int shaderProgram, glm::mat4 viewMatrix);

bool loadOBJ2(
	const char* path,
	std::vector<int>& vertexIndices,
	std::vector<glm::vec3>& temp_vertices,
	std::vector<glm::vec3>& out_normals,
	std::vector<glm::vec2>& out_uvs);
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

// Reverse winding order indices -> still needed for skybox
std::vector<int>  reverseIndices = { //3*12 =36
2 - 1,  1 - 1,3 - 1,
4 - 1,  3 - 1,7 - 1,
8 - 1,  7 - 1,5 - 1,
6 - 1,  5 - 1,1 - 1,
7 - 1,  3 - 1,1 - 1,
4 - 1,  8 - 1,6 - 1,
2 - 1,  3 - 1,4 - 1,
4 - 1,  7 - 1,8 - 1,
8 - 1,  5 - 1,6 - 1,
6 - 1,  1 - 1,2 - 1,
7 - 1,  1 - 1,5 - 1,
4 - 1,  6 - 1 ,2 - 1
};


GLuint IBO;
int createVertexArrayElementObject(const glm::vec3* vertexArray, int arraySize, const glm::vec3* NormalArray, int NormalArraySize, const glm::vec2* TextureArray, int TextureArraySize, unsigned int indices[], int indiceSize)
{
	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceSize * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	//int check = arraySize;
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
int createVertexArrayElementObject2(std::vector<int> vertexIndices,
	std::vector<glm::vec3> vertices,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> UVs)
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
const int WIDTH = 1024, HEIGHT = 768;
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
	GLuint grassTextureID = loadTexture("../src/Assets/grass4.jpg");
	// Black background	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);
	// Set frame rate to refresh rate of monitor
	glfwSwapInterval(1);

	// Compile and link shaders here
	int shaderProgram = compileAndLinkShaders(vertex, fragment);
	int textureProgram = compileAndLinkShaders(textureV, textureF);

	// vec3 variables
	glm::vec3 eye(.7650f, .250f, .7650f);
	glm::vec3 center(.00f, .0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 translate(0.0f, 0.0f, 0.0f);
	glm::vec3 Translate(.0f, .0f, .0f);
	glm::vec3 GroupMatrixScale(1.0f, 1.0f, 1.0f);

	// mat4 variables
	glm::mat4 groupMatrix;
	glm::mat4 rotationMatrixW = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	//Scene Jawn
	SceneObjects SceneObj("scene");
	SceneObj.InitGrid();
	SceneObj.setTextures(courtTextureID, ropeTextureID, metalTextureID, clothTextureID,grassTextureID);
	SceneObj.textureProgram = textureProgram;
	
	// General variables
	int renderAs = GL_TRIANGLES;
	double lastMousePosX, lastMousePosY, lastMousePosZ;
	float translateW = 0, translateY = 0, translateZ = 0;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
	lastMousePosZ = lastMousePosY;
	float FOV = 70, AR = (float)WIDTH / (float)HEIGHT, near = .01, far = 50;
	// Initialize projection and view matrices
	glm::mat4 projectionMatrix = glm::perspective(FOV,  // field of view in degrees
		AR,      // aspect ratio
		near, far);       // near and far (near > 0)
	glm::mat4 InitviewMatrix = glm::lookAt(eye, center, up);
	glUseProgram(shaderProgram);
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	setProjectionMatrix(shaderProgram, projectionMatrix);
	setProjectionMatrix(textureProgram, projectionMatrix);
	setViewMatrix(shaderProgram, InitviewMatrix);
	setViewMatrix(textureProgram, InitviewMatrix);



	std::vector<int> vertexIndicescube, vertexIndicessphere;
	std::vector<glm::vec3> verticescube, verticessphere, normalscube, normalssphere;
	std::vector<glm::vec2> UVscube, UVssphere;
	std::string pathCube = "../src/Assets/mesh/unitCube.obj";	
	std::string pathSphere = "../src/Assets/mesh/unitSphere.obj";
	loadOBJ2(pathCube.c_str(), vertexIndicescube, verticescube, normalscube, UVscube);
	loadOBJ2(pathSphere.c_str(), vertexIndicessphere, verticessphere, normalssphere, UVssphere);

	for (int i = 0; i < verticescube.size(); i++) {
		verticescube[i] = verticescube[i] * .05f;
	}
	for (int i = 0; i < verticessphere.size(); i++) {
		verticessphere[i] = verticessphere[i] * .05f;
	}

	int gridAO = createVertexArrayObject(SceneObj.lineArray, sizeof(SceneObj.lineArray));
	int unitCubeAO = createVertexArrayElementObject2(vertexIndicescube, verticescube, normalscube, UVscube);
	int unitSphereAO = createVertexArrayElementObject2(vertexIndicessphere, verticessphere, normalssphere, UVssphere);
	int reverseCubeAO = createVertexArrayElementObject2(reverseIndices, verticescube, normalscube, UVscube);

	Arm arm(unitCubeAO, "arm");
	Racket racket(unitCubeAO, "racket");
	racket.jawnAngle = 0;

	int select = -1;
	int* newWidth = new int;
	int* newHeight = new int;

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
		glUseProgram(shaderProgram); //note: still dependent on this for scene objects?

		// Set a default group matrix
		groupMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f)) *
			glm::scale(glm::mat4(1.0f), GroupMatrixScale) *
			rotationMatrixW;


		arm.SetAttr(groupMatrix, renderAs, shaderProgram);
		arm.setTranslation(Translate, translate);
		arm.DrawArm();
		racket.SetAttr(groupMatrix, renderAs, shaderProgram, arm.partParent);
		racket.Draw();
		SceneObj.sphereVao = unitSphereAO;
		SceneObj.sphereVertCount = vertexIndicessphere.size();
		SceneObj.SetAttr(rotationMatrixW, renderAs, shaderProgram);
		SceneObj.SetVAO(unitCubeAO, reverseCubeAO, gridAO);
		SceneObj.DrawScene();


		glfwSwapBuffers(window);
		glfwPollEvents();


		// Handle inputs
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{

		}

		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
		}
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
		}

		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			float number1 = (rand()) / (float)(RAND_MAX), number2 = (rand()) / (float)(RAND_MAX), number3 = (rand()) / (float)(RAND_MAX);

			// Constrain to visible grid locations
			while (number1 >= .75f || number2 >= .25f || number3 >= .75f) {
				if (number1 >= .75f) {
					number1 = number1 / (float)(RAND_MAX);
				}
				if (number2 >= .25f) {
					number2 = number2 / (float)(RAND_MAX);
				}
				if (number3 >= .75f) {
					number3 = number3 / (float)(RAND_MAX);
				}
			}
			int numZ = rand(), numX = rand(), numY = rand();
			int flZ = -1, flX = 1;

			if (numZ % 2 == 1) flZ *= -1;
			if (numX % 2 == 1) flX *= -1;
			Translate.x = number1;
			Translate.y = number2; Translate.z = number3;
		}
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
			GroupMatrixScale += .05f;
		}
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
			GroupMatrixScale -= .05f;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			rotationMatrixW *= glm::rotate(glm::mat4(1.0f), glm::radians(2.55f), glm::vec3(1.0f, 0.0f, 0.0f));
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			rotationMatrixW *= glm::rotate(glm::mat4(1.0f), glm::radians(2.55f), glm::vec3(-1.0f, 0.0f, 0.0f));
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			rotationMatrixW *= glm::rotate(glm::mat4(1.0f), glm::radians(2.55f), glm::vec3(.0f, 1.0f, 0.0f));
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			rotationMatrixW *= glm::rotate(glm::mat4(1.0f), glm::radians(2.55f), glm::vec3(.0f, -1.0f, 0.0f));
		}
		// w and d for axis checking
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			translate.y += .005;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			translate.y -= .005;
		}
		if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)) {
			translate.x -= .005;
		}
		if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)) {
			translate.x += .005;
		}
		if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && !(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)) {
			arm.setRotation(arm.getRotation() + 5);
		}
		if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && !(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)) {
			arm.setRotation(arm.getRotation() - 5);
		}
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
			renderAs = GL_POINTS;
		}
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
			renderAs = GL_LINES;
		}
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
			renderAs = GL_TRIANGLES;
		}
		if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
			//or set each translate to 0
			Translate.x += -1 * Translate.x; Translate.y += -1 * Translate.y; Translate.z += -1 * Translate.z;
			translate.x += -1 * translate.x; translate.y += -1 * translate.y;
			arm.armRotate = 0;
			GroupMatrixScale = glm::vec3(1.0f);
			rotationMatrixW = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			glm::mat4 InitviewMatrix = glm::lookAt(eye,  // eye
				center,  // center
				up);// up
			glm::mat4 projectionMatrix = glm::perspective(FOV,  // field of view in degrees
				AR,      // aspect ratio
				near, far);       // near and far (near > 0)
			setProjectionMatrix(textureProgram, projectionMatrix);
			setProjectionMatrix(shaderProgram, projectionMatrix);
			setViewMatrix(textureProgram, InitviewMatrix);
			setViewMatrix(shaderProgram, InitviewMatrix);
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			double  mousePosY;
			glfwGetCursorPos(window, NULL, &mousePosY);
			double dy = mousePosY - lastMousePosY;
			if (dy < 0) translateY += .005;
			else if (dy > 0) translateY -= .005;
			glm::mat4 InitviewMatrix = glm::lookAt(eye,  // eye
				glm::vec3(translateW, translateY, 0.0f),  // center
				up);// up
			setViewMatrix(shaderProgram, InitviewMatrix);
			setViewMatrix(textureProgram, InitviewMatrix);
			lastMousePosY = mousePosY;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			double  mousePosX;
			glfwGetCursorPos(window, &mousePosX, NULL);
			double dx = mousePosX - lastMousePosX;
			printf("x: %f  \n", dx);
			if (dx < 0) translateW -= .005;
			else if (dx > 0) translateW += .005;
			glm::mat4 InitviewMatrix = glm::lookAt(eye,  // eye
				glm::vec3(translateW + center.x, translateY + center.y, 0.0f),  // center
				up);// up
			setViewMatrix(shaderProgram, InitviewMatrix);
			setViewMatrix(textureProgram, InitviewMatrix);
			lastMousePosX = mousePosX;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		{
			double  mousePosZ;
			glfwGetCursorPos(window, NULL, &mousePosZ);
			double dy = mousePosZ - lastMousePosZ;

			if (dy < 0) translateZ += .005;
			else if (dy > 0)translateZ -= .005;
			glm::mat4 projectionMatrix = glm::perspective(translateZ + 70.0f,  // field of view in degrees
				(float)WIDTH / (float)HEIGHT,      // aspect ratio
				.01f, 50.0f);       // near and far (near > 0)
			setProjectionMatrix(shaderProgram, projectionMatrix);
			setProjectionMatrix(textureProgram, projectionMatrix);
			lastMousePosZ = mousePosZ;
		}
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



bool loadOBJ2(
	const char* path,
	std::vector<int>& vertexIndices,
	std::vector<glm::vec3>& temp_vertices,
	std::vector<glm::vec3>& out_normals,
	std::vector<glm::vec2>& out_uvs) {

	std::vector<int> uvIndices, normalIndices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE* file;
	file = fopen(path, "r");
	if (!file) {
		printf("Impossible to open the file ! Are you in the right path ?\n");
		getchar();
		return false;
	}

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
	out_normals = temp_normals;
	out_uvs = temp_uvs;
	//for (unsigned int i = 0; i < vertexIndices.size(); i++) {
	//	int vi = vertexIndices[i];
	//	if (normalIndices.size() != 0) {
	//		int ni = normalIndices[i];
	//		out_normals[vi] = temp_normals[ni];
	//	}
	//	if (uvIndices.size() != 0 && i < uvIndices.size()) {
	//		int ui = uvIndices[i];
	//		out_uvs[vi] = temp_uvs[ui];
	//	}
	//}

	return true;
}
