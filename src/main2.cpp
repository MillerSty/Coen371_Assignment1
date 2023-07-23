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
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
using namespace std;

GLuint loadTexture(const char* filename);
int compileAndLinkShaders(bool texture);
//void scrollCallback(GLFWwindow* window, double xoffset, double yoffset,GLuint shaderProgram );
const char* getVertexShaderSource()
{
	// set position and mvp matrix, using only 1 location
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"out vec3 vertexColor;"
		"uniform mat4 worldMatrix;" //world matrix
		"uniform mat4 viewMatrix=mat4(1.0f);"
		"uniform mat4 projectionMatrix = mat4(1.0);"
		"void main()"
		"{"

		"mat4 modelViewProjection= projectionMatrix *  viewMatrix * worldMatrix;"
		"   gl_Position = modelViewProjection  * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
		"}";
}
//General reminder:
// translate * scale * rotate
//world matrix is position, scale, orienting
//view matrix is.. virtual camera position, orientation of camera in the world

const char* getFragmentShaderSource()
{
	return
		"#version 330 core\n"
		"in vec4 vertexColor;"
		"out vec4 FragColor;"
		"uniform vec3 objectColor;"
		"void main()"
		"{"
		"   FragColor = vec4(objectColor.r, objectColor.g, objectColor.b, 1.0f);"
		"}";
}
const char* getTextVertexShaderSource()
{
	// For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"
		"layout (location = 1) in vec3 aNorm;"
		"layout (location = 2) in vec2 aTexture;"

		"uniform mat4 worldMatrix;" //world matrix
		"uniform mat4 viewMatrix=mat4(1.0f);"
		"uniform mat4 projectionMatrix = mat4(1.0);"
		"out vec2 vertexUV;"
		"void main()"
		"{"
		
		"mat4 modelViewProjection= projectionMatrix *  viewMatrix * worldMatrix;"
		"gl_Position = modelViewProjection  * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
		"vertexUV=aTexture;"
		"}";
}
// translate * scale * rotate
//world matrix is position, scale, orienting
//view matrix is.. virtual camera position, orientatio of camera in the world

const char* getTextFragmentShaderSource()
{
	return
		"#version 330 core\n"

		"in vec2 vertexUV;"
		"out vec4 FragColor;"

		"uniform vec3 objectColor;"
		"uniform sampler2D textureSampler;"
		"void main()"
		"{"
		"vec4 textureColor=texture(textureSampler,vertexUV);"
		"   FragColor = textureColor*vec4(objectColor.r, objectColor.g, objectColor.b, 1.0f);;"
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
	return vertexArrayObject;
}

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
//reverse indices for winding
unsigned int ReverseIndices[] = {
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

//we could use OBJ loaded?
glm::vec3 cubeVertex[] = {
glm::vec3(1.000000 ,1.000000  ,-1.000000),// Coord
glm::vec3(1.000000 ,-1.000000 ,-1.000000),// Coord
glm::vec3(1.000000 ,1.000000  , 1.000000),// Coord
glm::vec3(1.000000 ,-1.000000 , 1.000000),// Coord
glm::vec3(-1.000000,1.000000  , -1.00000),// Coord
glm::vec3(-1.000000,-1.000000 , -1.00000),// Coord
glm::vec3(-1.000000,1.000000  , 1.000000),// Coord
glm::vec3(-1.000000,-1.000000 , 1.000000), // Coord

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

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		1 * sizeof(glm::vec3), // stride - each vertex contain 1 vec3 (position)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);
	//"layout (location = 0) in vec3 aPos;"
	//	"layout (location = 1) in vec3 aNorm;"
	//	"layout (location = 2) in vec2 aTexture;"


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
//lab4
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

void setWorldMatrix(int shaderProgram, glm::mat4 worldMatrix)
{
	glUseProgram(shaderProgram);
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
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

	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
	
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

#if defined(PLATFORM_OSX)
	GLuint brickTextureID = loadTexture("Textures/brick.jpg");
	GLuint cementTextureID = loadTexture("Textures/cement.jpg");
#else
	GLuint asphaltTextureID = loadTexture("../Assets/asphalt/clay.jpg");
	GLuint cementTextureID = loadTexture("../Assets/grass/grass.jpg");
#endif



	// Black background	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);
	// Compile and link shaders here ...
	int shaderProgram = compileAndLinkShaders(false);
	int textshaderProgram = compileAndLinkShaders(true);
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
		cubeVertex[i] = cubeVertex[i] * .05f; // this is to prescale my unitcube
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
	
	glm::mat4 InitviewMatrix = glm::lookAt(eye,  // eye
		center,  // center
		up);// up
	setViewMatrix(shaderProgram, InitviewMatrix);
	setViewMatrix(textshaderProgram, InitviewMatrix);
	setProjectionMatrix(shaderProgram, projectionMatrix);

	setProjectionMatrix(textshaderProgram, projectionMatrix);

	int gridAO = createVertexArrayObject(lineArray, sizeof(lineArray));
	int unitCubeAO = createVertexArrayElementObject(cubeVertex, sizeof(cubeVertex),cubeNormal,sizeof(cubeNormal),cubeTexture,sizeof(cubeTexture),indices);
	int reverseCubeAO = createVertexArrayElementObject(cubeVertex, sizeof(cubeVertex), cubeNormal, sizeof(cubeNormal), cubeTexture, sizeof(cubeTexture), ReverseIndices);
	Arm arm(unitCubeAO, "arm");
	Racket racket(unitCubeAO, "racket");
	racket.jawnAngle = 0;

	int select = -1;
	int* newWidth = new int;
	int* newHeight = new int;
	while (!glfwWindowShouldClose(window))
	{
		//handle Window resize
		glfwGetWindowSize(window, newWidth, newHeight);
		glfwSetWindowSize(window, *newWidth, *newHeight);
		glViewport(0, 0, *newWidth, *newHeight);
		AR = (float)*newWidth / (float)*newHeight; //note unsure if this will cause issues
		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Draw geometry
		glUseProgram(shaderProgram);
		glBindVertexArray(unitCubeAO);		
		colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
		groupMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f)) 
			* glm::scale(glm::mat4(1.0f), GroupMatrixScale)
			* rotationMatrixW;

	
		arm.SetAttr(groupMatrix, renderAs, shaderProgram);
		arm.setTranslation(Translate, translate);
		arm.DrawArm();
		racket.SetAttr(groupMatrix, renderAs, shaderProgram, arm.partParent);
		racket.Draw();
				

		//Coordinate cubes
		//can do as .h
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

		//this is net, net is good
		//can be made into .h
		glBindVertexArray(unitCubeAO); //bind our unit cube 		
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.00f, .0f, .0f))); //set colour of net poles
		partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, 1.0f)); //rotate net poles
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0375f, -.34f)); //translate one net pole
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.25f, .75f, .25f)); //scale net poles
		partMatrix = partTranslate * partScale * partRo; //set matrix for one pole
		worldMatrix = groupMatrix * partMatrix; //set world for one pole 
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]); //make it my world
		glDrawElements(renderAs, gridCount, GL_UNSIGNED_INT, 0); //draw with index
		//+ve Z net
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0375f, .34f)); //note: now we only need the new pole translation
		partMatrix = partTranslate * partScale * partRo;// reusing scale and rotation from 1st pole
		worldMatrix = groupMatrix * partMatrix; //"A WHOLE NEW WOOOOOOOOORLD" - the little mermaid
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]); // comment
		glDrawElements(renderAs, gridCount, GL_UNSIGNED_INT, 0);//draw using index

		//Strings of net 		
		//another student said my net sucks so i made the scale better,
		//i hope you like it
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.60f, .60f, .60f)));// set net colour
		for (float i = .0; i <= .07f; i += .01) {//loop to draw new, i is our translation offset
			partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.03f, .03f, 7.0f));//scale to beatuiful net dimensions
			partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.005, i, -.0f)); //translate with that i offset
			partMatrix = partTranslate * partScale; // part matrix
			worldMatrix = groupMatrix * partMatrix; // another little mermaid reference

			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]); //comment
			glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);//draw with index
		}
		for (float i = -.34; i <= .34f; i += .01) { //again as above but net offset is along z this time,
			// the scale is also different to account for the new direction
			partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.03f, .7f, .03f));
			partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, .04, i));
			partMatrix = partTranslate * partScale;
			worldMatrix = groupMatrix * partMatrix;

			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
		}

		//here we do the top bar of the net
		//I will stop commenting every line for you
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.00f, 1.0f, 1.0f)));
		partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, 1.0f));
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.003f, .071f, .0f));
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(.1f, .1f, 6.6f));
		partMatrix = partTranslate * partScale * partRo;
		worldMatrix = groupMatrix * partMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawElements(renderAs, gridCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0); //unbinding 

		//This does skybox blue only around grid
		// //can make into .h if we want 
		//glBindVertexArray(unitCubeUnscaledAO);	//bind our reverse unscaled cube	 
		float ysize = 25.0f; //change this to make skybox closer to court


		glm::mat4 partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.0f, .001111f, -.0f)); //sets skybox position
		partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, 1.0f)); //unused but usable
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(20.780f, ysize, 20.36f));	 //sets sky box scale 
		partMatrix = partTranslate * partScale * partRo; //part matrix for skybox
		worldMatrix = groupMatrix * partMatrix; //worldMatrix for skybox
		glBindVertexArray(reverseCubeAO);
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.008f, .8f, .9999999f))); //skybox colour
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]); //send to shader
		glDrawElements(renderAs, gridCount, GL_UNSIGNED_INT, 0); //draw 
		glBindVertexArray(0); //unbind



		//note i want to draw tennis court after drawing grid.
		//can .h if we want
		glUseProgram(textshaderProgram);

		glBindVertexArray(unitCubeAO);
		glBindTexture(GL_TEXTURE_2D, asphaltTextureID);
		colorLocation = glGetUniformLocation(textshaderProgram, "objectColor");
		//glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0, 0.0, .0)));//red (x)
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
				setWorldMatrix(textshaderProgram, worldMatrix);
				//glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
		}
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

int compileAndLinkShaders(bool texture)
{
	// compile and link shader program
	// return shader program id
	// ------------------------------------

	// vertex shader
	int vertexShader;
	int fragmentShader;
	int success;
	char infoLog[512];
	if (!texture) {
		 vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const char* vertexShaderSource = getVertexShaderSource();
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		// check for shader compile errors
		//int success;
		//char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// fragment shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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
	}
	else {
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const char* vertexShaderSource = getTextVertexShaderSource();
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		// check for shader compile errors
		//int success;
		//char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// fragment shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fragmentShaderSource = getTextFragmentShaderSource();
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		// check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
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


//from lab4
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