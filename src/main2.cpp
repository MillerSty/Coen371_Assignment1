// Comp 371 - Assignment 1 
// Jon Micah Miller 40135070
// All done from scratch minus 
// shaders from tutorial's
// 
// 
// 

//#include "inputHandler.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
#include "Arm.h"
#include "Racket.h"

using namespace std;

int compileAndLinkShaders();
//void scrollCallback(GLFWwindow* window, double xoffset, double yoffset,GLuint shaderProgram );

const char* getVertexShaderSource()
{
	// For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"uniform mat4 worldMatrix;" //world matrix
		"uniform mat4 viewMatrix=mat4(1.0f);"
		"uniform mat4 projectionMatrix = mat4(1.0);"
		"void main()"
		"{"

		"mat4 modelViewProjection= projectionMatrix *  viewMatrix * worldMatrix;"
		"   gl_Position = modelViewProjection  * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
		"}";
}
// translate * scale * rotate
//world matrix is position, scale, orienting
//view matrix is.. virtual camera position, orientatio of camera in the world

const char* getFragmentShaderSource()
{
	return
		"#version 330 core\n"
		"out vec4 FragColor;"
		"uniform vec3 objectColor;"
		"void main()"
		"{"
		"   FragColor = vec4(objectColor.r, objectColor.g, objectColor.b, 1.0f);"
		"}";
}

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
		(void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
	);

	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return vertexArrayObject;
}

unsigned int indices[] = {
5 - 1,3 - 1,1 - 1,
3 - 1,8 - 1,4 - 1,
7 - 1,6 - 1,8 - 1,
2 - 1,8 - 1,6 - 1,
1 - 1,4 - 1,2 - 1,
5 - 1,2 - 1,6 - 1,
5 - 1,7 - 1,3 - 1,
3 - 1,7 - 1,8 - 1,
7 - 1,5 - 1,6 - 1,
2 - 1,4 - 1,8 - 1,
1 - 1,3 - 1,4 - 1,
5 - 1,1 - 1,2 - 1
};

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

GLuint IBO;
int createVertexArrayElementObject(const glm::vec3* vertexArray, int arraySize)
{
	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);


	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	/* unsigned int* index = indices;
	 int check0 = sizeof(&index);
	 int check = sizeof(indices);
	 int check2 = sizeof(unsigned int*/
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	GLuint vertexBufferObject;
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	int check = arraySize;
	glBufferData(GL_ARRAY_BUFFER, arraySize, vertexArray, GL_STATIC_DRAW);


	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		1 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return vertexArrayObject;
}

const int WIDTH = 1024, HEIGHT = 768;
int main(int argc, char* argv[])
{
	// Initialize GLFW and OpenGL version
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create Window and rendering context using GLFW, resolution is 800x600
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Comp371 - Assignment 1 -JMM 40135070", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Initialize GLEW

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	
	//glEnable(GL_CULL_FACE);
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Black background	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);
	// Compile and link shaders here ...
	int shaderProgram = compileAndLinkShaders();

	//create base grid
	glm::vec3 lineArray[808]; // 168 because why... 21 + 21 + 42?	//792+8
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

		lineArray[gridCount + 4] = glm::vec3(-1.0f, 0.0f, f); //
		lineArray[gridCount + 5] = glm::vec3(0.61f, .61f, .61f);//colour
		lineArray[(gridCount + 6)] = glm::vec3(1.0f, 0.0f, f);
		lineArray[gridCount + 7] = glm::vec3(0.61f, .61f, .61f);
		gridCount += 8; //400 for first 50	
		//o = j;
	}

	for (int i = 0; i < 8; i++) {
		unitcube[i] = unitcube[i] * .05f; // this is to prescale my unitcube
	}
	
	//initialize GLUint locations
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

	//mat4 variables
	glm::mat4 worldMatrix;
	glm::mat4 groupMatrix;

	glm::mat4 partTranslate;
	glm::mat4 partRo;
	//glm::mat4 RacketGroupMatrix;
	glm::mat4 partMatrix;
	glm::mat4 partScale;
	glm::mat4 rotationMatrixW = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));


	//general variables
	//float armRotate = 0;
	int renderAs = GL_TRIANGLES;
	double lastMousePosX, lastMousePosY, lastMousePosZ;
	float translateW = 0, translateY = 0, translateZ = 0;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
	lastMousePosZ = lastMousePosY;
	float FOV = 70, AR = (float)WIDTH / (float)HEIGHT, near = .01, far = 50;
	
	//Init projection and view
	glm::mat4 projectionMatrix = glm::perspective(FOV,  // field of view in degrees
			AR,      // aspect ratio
			near, far);       // near and far (near > 0)
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	
	glm::mat4 InitviewMatrix = glm::lookAt(eye,  // eye
		center,  // center
		up);// up
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &InitviewMatrix[0][0]);



	int gridAO = createVertexArrayObject(lineArray, sizeof(lineArray));
	int unitCubeAO = createVertexArrayElementObject(unitcube, sizeof(unitcube));
	Arm arm(unitCubeAO, "arm");
	Racket racket(unitCubeAO, "racket");
	racket.jawnAngle = 0;

	while (!glfwWindowShouldClose(window))
	{
		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Draw geometry
		glUseProgram(shaderProgram);
		glBindVertexArray(unitCubeAO);		

		groupMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f)) 
			* glm::scale(glm::mat4(1.0f), GroupMatrixScale)
			* rotationMatrixW;

	
		arm.SetAttr(groupMatrix, renderAs, shaderProgram);
		arm.setTranslation(Translate, translate);
		arm.DrawArm();
		racket.SetAttr(groupMatrix, renderAs, shaderProgram, arm.partParent);
		racket.Draw();
				

		//Note we are trying doing cubes as heirarchy
		//parent x
		glBindVertexArray(unitCubeAO);
		glm::mat4 cubeParent;
		glm::mat4 cubeRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 cubeTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
		glm::mat4 cubeScale= glm::scale(glm::mat4(1.0f), glm::vec3(.99f, .15f, .15f));
		cubeParent= cubeTranslate* cubeRotate*rotationMatrixW;
		
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(1.0f, 1.0f, 1.0f));
		partTranslate= glm::translate(glm::mat4(1.0f), glm::vec3(.043f, .0f,.0f));
		partMatrix = partTranslate * cubeScale *  partRo;
		
		worldMatrix =  cubeParent * partMatrix;
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
		//partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		//partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, 0.0f, 1.0f));
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
		//partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		//partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, 0.0f, 1.0f));
		partMatrix = partTranslate * partScale * partRo;
		worldMatrix = cubeChild * partMatrix;		
		
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.0, 1.0, 0.0))); //green (y)
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		glBindVertexArray(0); //UnBind  unitCube VAO

		//render grid
		glm::mat4 grid = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 gridRo = glm::rotate(glm::mat4(1.0f), glm::radians(Translate.z), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(Translate.z), glm::vec3(1.0f, .0f, 0.0f));
		glm::mat4 gridWorld = grid * rotationMatrixW;
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.61, 0.610, 0.610)));
		glBindVertexArray(gridAO);
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &gridWorld[0][0]);
		glDrawArrays(GL_LINES, 0, gridCount);
		glBindVertexArray(0);



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
			while (number1 >= .75f || number2 >= .25f || number3 >= .75f) { //constain to visible grid locations
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
			printf("x: %f  \n", dy);
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
	// } ENDS WHILE LOOPS -> Shutdown GLFW
	glfwTerminate();
	return 0;
}

int compileAndLinkShaders()
{
	// compile and link shader program
	// return shader program id
	// ------------------------------------

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = getVertexShaderSource();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = getFragmentShaderSource();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}