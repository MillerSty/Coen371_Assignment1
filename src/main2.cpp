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

const char* vertex = "../src/shaders/vertexShader.glsl";
const char* fragment = "../src/shaders/fragmentShader.glsl";

int compileAndLinkShaders(const char* vertex, const char* fragment);

void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	                            const GLchar* message, const void* userParam);

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
	glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		2 * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)      // color is offset by a vec3 (comes after position)
	);

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
	glm::vec3(-0.0000, 1.0000 , -0.0000),
	glm::vec3(-0.0000, -0.0000, 1.0000),
	glm::vec3(-1.0000, -0.0000, -0.0000),
	glm::vec3(-0.0000, -1.0000, -0.0000),
	glm::vec3(1.0000 ,-0.0000 , -0.0000),
	glm::vec3(-0.0000, -0.0000, -1.0000)
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36*sizeof(unsigned int), indices, GL_STATIC_DRAW);

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

	// Enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(messageCallback, 0);

	// Print OpenGL version
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	// Black background	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);

	// Set frame rate to refresh rate of monitor
	glfwSwapInterval(1);

	// Compile and link shaders here
	int shaderProgram = compileAndLinkShaders(vertex, fragment);

	// Create base grid
	glm::vec3 lineArray[808]{}; // 168 because why... 21 + 21 + 42?	//792+8
	int gridCount = 0;
	//int o = j;
	for (int i = -100; i <= 100; i += 2) { //needs to be 100 x 100
		// if (i != 0) {
		float f = i / 100.0f;
		//this is for x z grid
		lineArray[gridCount] = glm::vec3(f, .0f, -1.0f);
		lineArray[gridCount + 1] = glm::vec3(0.61f, .61f, .61f);//colour
		lineArray[(gridCount + 2)] = glm::vec3(f, .0f, 1.0f);
		lineArray[gridCount + 3] = glm::vec3(0.61f, .61f, .61f);//colour

		lineArray[gridCount + 4] = glm::vec3(-1.0f, 0.0f, f);
		lineArray[gridCount + 5] = glm::vec3(0.61f, .61f, .61f);//colour
		lineArray[(gridCount + 6)] = glm::vec3(1.0f, 0.0f, f);
		lineArray[gridCount + 7] = glm::vec3(0.61f, .61f, .61f);
		gridCount += 8; //400 for first 50	
		//o = j;
	}

	for (int i = 0; i < 8; i++) {
		unitcube[i] = unitcube[i] * .05f; // This is to pre-scale the unit cube
	}
	
	// Initialize uniform locations
	glUseProgram(shaderProgram);
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	// vec3 variables
	glm::vec3 eye(.7650f, .250f, .7650f);
	glm::vec3 center(.00f, .0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 translate(0.0f, 0.0f, 0.0f);
	glm::vec3 Translate(.0f, .0f, .0f);
	glm::vec3 GroupMatrixScale(1.0f, 1.0f, 1.0f);
	glm::vec3 constArmScaler;

	// mat4 variables
	glm::mat4 worldMatrix;
	glm::mat4 groupMatrix;

	glm::mat4 partTranslate;
	glm::mat4 partRo;
	//glm::mat4 RacketGroupMatrix;
	glm::mat4 partMatrix;
	glm::mat4 partScale;
	glm::mat4 rotationMatrixW = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	// General variables
	//float armRotate = 0;
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
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	
	glm::mat4 InitviewMatrix = glm::lookAt(eye, center, up);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &InitviewMatrix[0][0]);


	int gridAO = createVertexArrayObject(lineArray, sizeof(lineArray));
	int unitCubeAO = createVertexArrayElementObject(unitcube, sizeof(unitcube),cubeNormal,sizeof(cubeNormal),cubeTexture,sizeof(cubeTexture),indices);
	int reverseCubeAO = createVertexArrayElementObject(unitcube, sizeof(unitcube), cubeNormal, sizeof(cubeNormal), cubeTexture, sizeof(cubeTexture), indices);
	Arm arm(unitCubeAO, "arm");
	Racket racket(unitCubeAO, "racket");
	racket.jawnAngle = 0;

	int select = -1;
	int* newWidth = new int;
	int* newHeight = new int;

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
		glBindVertexArray(unitCubeAO);		

		// Set a default group matrix
		groupMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f)) *
			          glm::scale(glm::mat4(1.0f), GroupMatrixScale) *
			          rotationMatrixW;

	
		arm.SetAttr(groupMatrix, renderAs, shaderProgram);
		arm.setTranslation(Translate, translate);
		arm.DrawArm();
		racket.SetAttr(groupMatrix, renderAs, shaderProgram, arm.partParent);
		racket.Draw();

		// Draw Coordinate Cubes
		glBindVertexArray(unitCubeAO);
		glm::mat4 cubeParent;
		glm::mat4 cubeRotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 cubeTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
		glm::mat4 cubeScale = glm::scale(glm::mat4(1.0f), glm::vec3(.99f, .15f, .15f));
		cubeParent = cubeTranslate * cubeRotate * rotationMatrixW;
		
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(1.0f, 1.0f, 1.0f));
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.043f, .0f,.0f));
		partMatrix = partTranslate * cubeScale * partRo;
		
		worldMatrix = cubeParent * partMatrix;
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.0, 0.0, 1.0)));//blue (x)
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//child z 
		glm::mat4 cubeChild;
		glm::mat4 cubeChildRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)90), glm::vec3(.0f, 1.0f, .0f));
		glm::mat4 cubeChildTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .043f));
		glm::mat4 cubeChildScale = glm::scale(glm::mat4(1.0f), glm::vec3( .99f,  .15f,  .15f));	

		cubeChild = cubeParent * cubeChildTranslate * cubeChildRotate * cubeChildScale;
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
		partMatrix = partTranslate* partScale * partRo;
		worldMatrix =  cubeChild * partMatrix;		
		
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 0.0, 0.0))); //red (z)
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);		
		//child y
		cubeChildRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)90), glm::vec3(.0f, 0.0f, 1.0f));
		cubeChildTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.0f,.043f, .001f));
		cubeChildScale = glm::scale(glm::mat4(1.0f), glm::vec3(.99f,  .15f, .15f));
		cubeChild = cubeParent * cubeChildTranslate * cubeChildRotate * cubeChildScale;
		
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
		partMatrix = partTranslate * partScale * partRo;
		worldMatrix = cubeChild * partMatrix;		
		
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.0, 1.0, 0.0))); //green (y)
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0); // Unbind unit cube VAO

		// Render grid
		glm::mat4 grid = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 gridRo = glm::rotate(glm::mat4(1.0f), glm::radians(Translate.z), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(Translate.z), glm::vec3(1.0f, .0f, 0.0f));
		glm::mat4 gridWorld = grid * rotationMatrixW;
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.61, 0.610, 0.610)));
		glBindVertexArray(gridAO);
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &gridWorld[0][0]);
		glDrawArrays(GL_LINES, 0, gridCount);
		glBindVertexArray(0);

		glBindVertexArray(unitCubeAO);
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 0.0, .0)));//red (x)
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.2f, .2f, .2f));
		int absX = 0;
		int absZ = 0;
		for (int pitchx = -78; pitchx < 78; pitchx += 2) {
			for (int pitchz = -36; pitchz < 36; pitchz += 2) {
				//now i need a way to change the colour 				
				partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(pitchx / (float)100, -.01, pitchz / (float)100 + .01));
				partMatrix = partTranslate * partScale;
				worldMatrix = groupMatrix * partMatrix;
				if (abs(pitchx) == 74 && (abs(pitchz) < 31)) {	 //back horizontal				 
					glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));//white
				}
				else if ((abs(pitchz) == 32) && abs(pitchx) < 76) {	//length lines			
					glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));//white
				}
				else if (abs(pitchx) == 54 && (abs(pitchz) <= 30)) { //inside horizontal	
					glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));//white
				}
				else if (abs(pitchx) < 54 && (abs(pitchz) == 0)) { //inside vertical			
					glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));//white
				}
				else { //pitch is green
					glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.0, 0.522, .40)));// 
				}
				glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
		}
		glBindVertexArray(0);

		// Bind the unit cube VAO
		glBindVertexArray(unitCubeAO); 	

		// Set colour of net poles
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.00f, .0f, .0f)));

		// Rotate net poles
		partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, 1.0f));
		
		// Translate one net pole
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0375f, -.34f));
		
		// Scale net poles
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.25f, .75f, .25f));
		
		// Set part matrix based on translation, scaling, and rotation. Set world matrix based on part and group
		partMatrix = partTranslate * partScale * partRo;
		worldMatrix = groupMatrix * partMatrix;

		// Draw world matrix with indices
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]); 
		glDrawElements(renderAs, gridCount, GL_UNSIGNED_INT, 0);

		//+ve Z net
		// Translate only the new pole. Can reuse scale and translation matrices from first pole
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0375f, .34f));
		partMatrix = partTranslate * partScale * partRo;
		worldMatrix = groupMatrix * partMatrix;

		// Draw again
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawElements(renderAs, gridCount, GL_UNSIGNED_INT, 0);

		// Strings of net
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.60f, .60f, .60f)));  // Set net colour
		
		// Loop to draw net. i is our translation offset
		for (float i = .0; i <= .07f; i += .01) {
			partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.03f, .03f, 7.0f));  // Scale to beautiful net dimensions
			partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.005, i, -.0f));  // Translate with that i offset
			partMatrix = partTranslate * partScale; // Set part matrix
			worldMatrix = groupMatrix * partMatrix; // Set world matrix

			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]); //comment
			glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);//draw with index
		}
		
		// Same as above, but net offset is along z this time
		for (float i = -.34; i <= .34f; i += .01) {
			// The scale is also different to account for the new direction
			partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.03f, .7f, .03f));
			partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, .04, i));
			partMatrix = partTranslate * partScale;
			worldMatrix = groupMatrix * partMatrix;

			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
		}

		// Top bar of the net
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.00f, 1.0f, 1.0f)));
		partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, 1.0f));
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.003f, .071f, .0f));
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.1f, .1f, 6.6f));
		partMatrix = partTranslate * partScale * partRo;
		worldMatrix = groupMatrix * partMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawElements(renderAs, gridCount, GL_UNSIGNED_INT, 0);
		
		// Unbinding the vertex array object 
		glBindVertexArray(0);

		//This does sky box blue only around grid
		// //can make into .h if we want 
		//glBindVertexArray(unitCubeUnscaledAO);	//bind our reverse unscaled cube	 
		float ysize = 25.0f; // Change this to make sky box closer to court

		// Sets sky box position
		glm::mat4 partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.0f, .001111f, -.0f));

		// Unused but usable
		partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, 1.0f));
		
		// Sets sky box scale 
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(20.780f, ysize, 20.36f));
		
		partMatrix = partTranslate * partScale * partRo;  // Part matrix for sky box
		worldMatrix = groupMatrix * partMatrix;  // World matrix for sky box

		// Bind the vertex array object to be the cube VAO with reverse winding order
		glBindVertexArray(reverseCubeAO);
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.008f, .8f, .9999999f))); // Sky box colour
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]); // Send to shader
		glDrawElements(renderAs, gridCount, GL_UNSIGNED_INT, 0); // Draw sky box
		glBindVertexArray(0); // Unbind vertex array object


		glfwSwapBuffers(window);
		glfwPollEvents();

		//trying to setup callbacks
		//glfwSetKeyCallback(window, KeyInput);
		//glfwSetScrollCallback(window, scrollCallback);


		// Handle inputs
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			printf("1\n");
			float army = arm.getRotation() + 5;
			arm.setRotation(arm.getRotation() + 5);
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
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &InitviewMatrix[0][0]);
			glm::mat4 projectionMatrix = glm::perspective(FOV,  // field of view in degrees
				AR,      // aspect ratio
				near, far);       // near and far (near > 0)
			glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
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
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &InitviewMatrix[0][0]);
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
				glm::vec3(translateW+center.x, translateY+center.y, 0.0f),  // center
				up);// up
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &InitviewMatrix[0][0]);
			lastMousePosX = mousePosX;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		{
			double  mousePosZ;
			glfwGetCursorPos(window, NULL, &mousePosZ);			
			double dy = mousePosZ - lastMousePosZ;

			if (dy < 0) translateZ += .005;
			else if (dy > 0)translateZ -= .005; 
			glm::mat4 projectionMatrix = glm::perspective(translateZ+70.0f,  // field of view in degrees
				(float)WIDTH / (float)HEIGHT,      // aspect ratio
				.01f, 50.0f);       // near and far (near > 0)
			glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
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
	//const char* vertexShaderSource = getVertexShaderSource();
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
	//const char* fragmentShaderSource = getFragmentShaderSource();
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
void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, 
	                            const GLchar* message, const void* userParam)
{
	std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
		<< " type = 0x" << type << ", severity = 0x" << severity << ", message = " << message << std::endl;
}
