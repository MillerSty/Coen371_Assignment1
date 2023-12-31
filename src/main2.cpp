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
#include <glm/gtx/color_space.hpp>
#define STB_IMAGE_IMPLEMENTATION
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
#include <random>

// Declare some functions for later use
int compileAndLinkShaders(const char* vertex, const char* fragment);
GLuint loadTexture(const char* filename);
void FireWorks(glm::vec3 position, GLuint unitCube, Material courtMaterial, GLint worldMatrixLocation, GLint colorLocation,bool explode);
void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                const GLchar* message, const void* userParam);
void setProjectionMatrix(int shaderProgram, glm::mat4 projectionMatrix);
void setViewMatrix(int shaderProgram, glm::mat4 viewMatrix);
void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
bool loadOBJ2(const char* path, std::vector<int>& vertexIndices, std::vector<glm::vec3>& temp_vertices,
	          std::vector<glm::vec3>& out_normals, std::vector<glm::vec2>& out_uvs);
void handleSounds(double currentTime);
void handleScoring(double currentTime, int& red, int& blue);
void setUpLighting();
void setUpShadowMap(const unsigned int &DEPTH_MAP_TEXTURE_SIZE, GLuint &depth_map_texture, GLuint &depth_map_fbo);
GLuint createVertexArrayObject(const glm::vec3* vertexArray, int arraySize);
GLuint createVertexArrayObject2();
GLuint createVertexArrayElementObject2(std::vector<int> vertexIndices, std::vector<glm::vec3> vertices,
                                       std::vector<glm::vec3> normals, std::vector<glm::vec2> UVs);
void updateLight(glm::vec3 newPosition, glm::vec3 newFocus, SceneObjects SceneObj, GLuint shaderProgram, bool applyShadow);
glm::vec3 getRandomLocationSides();
glm::vec3 getRandomLocationFront();

// Set the shader paths
const char* vertex = "../src/shaders/unifiedVertex.glsl";
const char* fragment = "../src/shaders/unifiedFragment.glsl";

/**
A struct to contain the position, normal, and UV coordinates for a vertex
*/
struct TexturedNormalledVertex
{
    TexturedNormalledVertex(glm::vec3 _position, glm::vec3 _normals, glm::vec2 _uv) : position(_position), normals(_normals), uv(_uv) {}

    glm::vec3 position;
    glm::vec3 normals;
    glm::vec2 uv;
};

// Textured and normalled cube model
TexturedNormalledVertex texturedCubeVertexArray[] = {
	// LEFT
	TexturedNormalledVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1, 0, 0), glm::vec2(0.0f, 0.0f)),
    TexturedNormalledVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-1, 0, 0), glm::vec2(0.0f, 1.0f)),
    TexturedNormalledVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1, 0, 0), glm::vec2(1.0f, 1.0f)),

    TexturedNormalledVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1, 0, 0), glm::vec2(0.0f, 0.0f)),
    TexturedNormalledVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1, 0, 0), glm::vec2(1.0f, 1.0f)),
    TexturedNormalledVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-1, 0, 0), glm::vec2(1.0f, 0.0f)),

	// FAR
	TexturedNormalledVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 0, -1), glm::vec2(1.0f, 1.0f)),
    TexturedNormalledVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, 0, -1), glm::vec2(0.0f, 0.0f)),
    TexturedNormalledVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 0, -1), glm::vec2(0.0f, 1.0f)),

    TexturedNormalledVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 0, -1), glm::vec2(1.0f, 1.0f)),
    TexturedNormalledVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0, 0, -1), glm::vec2(1.0f, 0.0f)),
    TexturedNormalledVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, 0, -1), glm::vec2(0.0f, 0.0f)),

	// BOTTOM
	TexturedNormalledVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, -1, 0), glm::vec2(1.0f, 1.0f)),
    TexturedNormalledVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, -1, 0), glm::vec2(0.0f, 0.0f)),
    TexturedNormalledVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0, -1, 0), glm::vec2(1.0f, 0.0f)),

    TexturedNormalledVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, -1, 0), glm::vec2(1.0f, 1.0f)),
    TexturedNormalledVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, -1, 0), glm::vec2(0.0f, 1.0f)),
    TexturedNormalledVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, -1, 0), glm::vec2(0.0f, 0.0f)),

	// NEAR
	TexturedNormalledVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(0.0f, 1.0f)),
    TexturedNormalledVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(0.0f, 0.0f)),
    TexturedNormalledVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(1.0f, 0.0f)),

    TexturedNormalledVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(1.0f, 1.0f)),
    TexturedNormalledVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(0.0f, 1.0f)),
    TexturedNormalledVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(1.0f, 0.0f)),

	// RIGHT
	TexturedNormalledVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec2(1.0f, 1.0f)),
    TexturedNormalledVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1, 0, 0), glm::vec2(0.0f, 0.0f)),
    TexturedNormalledVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1, 0, 0), glm::vec2(1.0f, 0.0f)),

    TexturedNormalledVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1, 0, 0), glm::vec2(0.0f, 0.0f)),
    TexturedNormalledVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec2(1.0f, 1.0f)),
    TexturedNormalledVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec2(0.0f, 1.0f)),

	// TOP
	TexturedNormalledVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec2(1.0f, 1.0f)),
    TexturedNormalledVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 1, 0), glm::vec2(1.0f, 0.0f)),
    TexturedNormalledVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 1, 0), glm::vec2(0.0f, 0.0f)),

    TexturedNormalledVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec2(1.0f, 1.0f)),
    TexturedNormalledVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 1, 0), glm::vec2(0.0f, 0.0f)),
    TexturedNormalledVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec2(0.0f, 1.0f))
};

// Create an IBO to be saved to later
GLuint IBO;

// Set aspect ratio of the window
const int WIDTH = 1024, HEIGHT = 768;

// State-related globals
bool soundPlayed = false;
bool scoreIncremented = false;
int renderAs = GL_TRIANGLES;
int shaderProgram;
double lastMousePosX, lastMousePosY, lastMousePosZ;
float FOV = 70.3f;
float AR = (float) WIDTH / (float) HEIGHT;
float near = .01;
float far = 50;
float translateW = 0;
float translateY = 0;
float translateZ = 0;
float lightDepth = 1.0f;
bool noShowLightBox = false;
bool shouldApplyShadows = true;
bool shouldApplyTextures = true;
float camCounter = 0.0f;
bool shouldRotateCamera = false;
glm::vec3 eye(.0f, .350f*1.5, .7650f*1.5);
glm::vec3 center(.00f, .0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 GroupMatrixScale(1.0f, 1.0f, 1.0f);
glm::mat4 groupMatrix;
glm::mat4 rotationMatrixW = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

const int size = 250;
const int smallSize = 80;
glm::vec3 locations[size];
glm::vec3 smallLocations[smallSize];

// Create Letter objects for the scoreboard
Letters numberDraw;
Letters numberDraw2;

// Create crowd
CrowdObjects crowd;

// Create ball
Ball ball;

// Create arms
Arm playerArm1;
Arm playerArm2;

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

    // If irrKlang failed to load, halt
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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Comp371 - Team 9 Project", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

    // Set viewport size
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

#if defined(__APPLE__)
#else
	// Enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(messageCallback, 0);
#endif

    // Set mouse and keyboard callbacks
    glfwSetKeyCallback(window, keyPressCallback);
    glfwSetCursorPosCallback(window, mouseCursorPositionCallback);
	
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

    // Set the sound engine for the Scene
	SceneObj.audioEngine = audioEngine;

	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
	lastMousePosZ = lastMousePosY;

	// Initialize projection and view matrices
	glm::mat4 projectionMatrix = glm::perspective(FOV, AR, near, far);
	glm::mat4 InitViewMatrix = glm::lookAt(eye, center, up);
	setProjectionMatrix(shaderProgram, projectionMatrix);
	setViewMatrix(shaderProgram, InitViewMatrix);

    // Load sphere object for the ball
    std::vector<int> vertexIndicesSphere;
	std::vector<glm::vec3> verticesSphere, normalsSphere;
	std::vector<glm::vec2> UVsSphere;
	std::string pathSphere = "../src/Assets/mesh/unitSphere.obj";
	loadOBJ2(pathSphere.c_str(), vertexIndicesSphere, verticesSphere, normalsSphere, UVsSphere);
  
	// Scale the vertex positions of the sphere
	for (auto & vert : verticesSphere) {
		vert *= 0.05f;
	}
	
	// Create VAOs
	GLuint gridAO = createVertexArrayObject(SceneObj.lineArray, sizeof(SceneObj.lineArray));
	GLuint unitSphereAO = createVertexArrayElementObject2(vertexIndicesSphere, verticesSphere, normalsSphere, UVsSphere);
	GLuint unitCubeAO = createVertexArrayObject2();

    // TEXTURES
	// Load textures
	GLuint courtTextureID    = loadTexture("../src/Assets/clay2.jpg");
	GLuint ropeTextureID     = loadTexture("../src/Assets/rope.jpg");
	GLuint clothTextureID    = loadTexture("../src/Assets/cloth.jpg");
	GLuint metalTextureID    = loadTexture("../src/Assets/metal.jpg");
	GLuint grassTextureID    = loadTexture("../src/Assets/grass4.jpg");
	GLuint plasticTextureID  = loadTexture("../src/Assets/plastic.jpg");

	// Define materials based on textures
	Material courtMaterial(.2f, .002f, .50f, .001f, courtTextureID, shaderProgram); //court shouldn't reflect
	Material ropeMaterial(.5f, .60f, .5f, .09f, ropeTextureID, shaderProgram); // ropes are just ropes
	Material clothMaterial(.5f, .30f, .5f, .02f, clothTextureID, shaderProgram); //cloth should have a little reflection?
	Material metalMaterial(.6f, .90f, .6f, .12f, metalTextureID, shaderProgram); //metal should shine
	Material grassMaterial(.60f, .001f, .6f, .0001f, grassTextureID, shaderProgram); //just bright, that's all it needs
	Material plasticMaterial(.5f, .30f, .4f, .1f, plasticTextureID, shaderProgram); //needs to be glossy! This is our racket1
	Material skinMaterial(.1f, .0f, .66f, .001f, plasticTextureID, shaderProgram); // This is skin
	Material skyMaterial(.3f, .001f, .9f, .0001f, plasticTextureID, shaderProgram); // Flat blue sky

	//Model.cpp, Texture.cpp, Mesh.cpp taken from LearnOpenGL Udemy course
	//NOTE: Only used within Model's not for how we use textures in general
	//TODO jawn the link
	Model Bleachers;
	Bleachers = Model();
	Bleachers.LoadModel("../src/Models/bleachers.obj");

	//Made by alicefox, personal use license
	Model Trees;
	Trees = Model();
	Trees.LoadModel("../src/Models/tree.obj");

	Model Flower;
	Flower = Model();
	Flower.LoadModel("../src/Models/Flowers/flower.obj");

	Model Grass;
	Grass = Model();
	Grass.LoadModel("../src/Models/Grass/grass.obj");

	Model ShortShrub;
	ShortShrub = Model();
	ShortShrub.LoadModel("../src/Models/ShortShrub/shortshrub.obj");

	Model TallShrub;
	TallShrub = Model();
	TallShrub.LoadModel("../src/Models/TallShrub/tallshrub.obj");



    // Set the materials of the various objects in the scene
	SceneObj.setMaterials(courtMaterial, clothMaterial, ropeMaterial, metalMaterial, grassMaterial, plasticMaterial);
	SceneObj.skyTexture = skyMaterial;
	
	//Crowd
	crowd.vaos[0] = unitCubeAO;
	crowd.vaos[1] = unitSphereAO;
	crowd.sphereIndexCount = (int) vertexIndicesSphere.size();
	crowd.skinMaterial = skinMaterial;
	crowd.shaderProgram = shaderProgram;
	crowd.renderAs = GL_TRIANGLES;

	// Set parameters for rackets and arms
	playerArm1.InitArm(glm::vec3(-.5f, 0.15f, .2f), unitCubeAO, skinMaterial, clothMaterial);
    playerArm1.gloveColor = glm::vec3(0.0f, 0.0f, 1.0f);
    playerArm2.InitArm(glm::vec3(.5f, 0.15f, -.2f), unitCubeAO, skinMaterial, clothMaterial);
    playerArm2.gloveColor = glm::vec3(1.0f, 0.0f, 0.0f);
	Racket racket1(unitCubeAO, "racket1");
    racket1.plasticMaterial = plasticMaterial;
    Racket racket2(unitCubeAO, "racket2");
    racket2.plasticMaterial = plasticMaterial;

    // Set parameters for the scoreboard
	numberDraw.cubeVao = unitCubeAO;
    numberDraw2.cubeVao = unitCubeAO;
	numberDraw.shaderProgram = shaderProgram;
	numberDraw2.shaderProgram = shaderProgram;
	numberDraw.plastic = skinMaterial;
	numberDraw2.plastic = skinMaterial;
	numberDraw.position = glm::vec3(-.20f, .21f, -0.40f);
	numberDraw2.position = glm::vec3(.20f, .21f, -0.40f);

    // Set ball parameters
    ball.setShaderProgram(shaderProgram);
    ball.setVAO(unitSphereAO);
    ball.setSphereVertCount((int) vertexIndicesSphere.size());
    ball.setMaterial(grassMaterial);
    ball.setSoundEngine(audioEngine);

    // SET UP LIGHTING
    setUpLighting();

    // SET UP SHADOWS
    const unsigned int DEPTH_MAP_TEXTURE_SIZE = 1024;
    GLuint depth_map_texture;
    GLuint depth_map_fbo;
    setUpShadowMap(DEPTH_MAP_TEXTURE_SIZE, depth_map_texture, depth_map_fbo);

    // DEFINE KEYFRAMES FOR ANIMATION
	// Keyframes for Blue player
	KeyFrame keyframesBlue[] = {
		KeyFrame(glm::vec3(0.0, 0.0, 0.0), glm::vec3(90.0,0,0), 0.0), // Initial key frame
		KeyFrame(glm::vec3(0.0, 0.0, 0.0), glm::vec3(90.0,0,0), 3.0), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.0), glm::vec3(120.0,25,60), 5.5), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.035), glm::vec3(90.0,0,-35), 6.0), // Hit ball
		KeyFrame(glm::vec3(0.0, 0.0, -0.1), glm::vec3(90.0,0,0), 13.0), // Start moving back
		KeyFrame(glm::vec3(0.0, 0.0, -0.1), glm::vec3(120.0,25,60), 15.0), // Start moving away from ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.035), glm::vec3(90.0,0,-35), 16.0), // Hit ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.035), glm::vec3(90.0,0,0), 19.0), // Start moving back
		KeyFrame(glm::vec3(0.0, 0.0, -0.1), glm::vec3(120.0,25,60), 21.0), // Start moving away from ball (MISS)
		KeyFrame(glm::vec3(0.0, 0.0, 0.005), glm::vec3(70.0,0,0), 21.5), // Be away from ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.005), glm::vec3(90.0,0,0), 23.0), // Start moving back
		KeyFrame(glm::vec3(0.0, 0.0, -0.1), glm::vec3(90.0,0,0), 24.0), // Be in position
		KeyFrame(glm::vec3(0.0, 0.0, -0.1), glm::vec3(90.0,0,0), 28.0), // Start moving back 
		KeyFrame(glm::vec3(0.0, 0.0, -0.1), glm::vec3(120.0,25,60), 30.3), // Start moving away from ball HIT
		KeyFrame(glm::vec3(0.0, 0.0, 0.035), glm::vec3(90.0,0,-35), 31.0), // Start moving back
		KeyFrame(glm::vec3(0.0, 0.0, -0.1), glm::vec3(90.0,0,0), 34.0), // Start moving back
		KeyFrame(glm::vec3(0.0, 0.0, -0.1), glm::vec3(120.0,25,60), 36.3), // Start moving away from ball
		KeyFrame(glm::vec3(0.0, 0.0, -0.255), glm::vec3(90.0,0,-35), 37.0), // Start moving back
	};

	// Keyframes for Red player
	KeyFrame keyframesRed[] = {
		KeyFrame(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-90.0,25,60), 0.0), // Initial key frame
		KeyFrame(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-60.0,0.0,0.0), 2.5), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, -0.035), glm::vec3(-90.0,0.0,-35.0), 3.0), // Hit ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-60.0,0,0), 8.0), // Start moving away from ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.18), glm::vec3(-110.0,25,60), 8.5), // Be away from ball (MISS)
		KeyFrame(glm::vec3(0.0, 0.0, -0.005), glm::vec3(-90.0,0,0), 11.0), // Start moving back
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-90.0,0,0), 12.0), // Be in position
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-90.0,0,0), 14.0),
		KeyFrame(glm::vec3(0.0, 0.0, 0.07), glm::vec3(-90.0,25,60), 16.0), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-60.0,0,0), 18.0), // Hit ball
		KeyFrame(glm::vec3(0.0, 0.0, -0.035), glm::vec3(-90.0,0.0,-35.0), 19.0), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-90.0,25,60), 25.0), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-60.0,0,0), 27.3), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, -0.035), glm::vec3(-90.0,0.0,-35.0), 28.0), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-90.0,25,60), 31.0), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-60.0,0.0,-35.0), 33.3), // 
		KeyFrame(glm::vec3(0.0, 0.0, 0.255), glm::vec3(-90.0,25,0), 34.0), // 
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-60.0,0,0), 39.0), // Start moving away from ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.18), glm::vec3(-110.0,0,35.0), 39.5), // Be away from ball
	};

    const float BALL_Y_OFFSET = 0.25f;  // Make sure ball is constant height to keep things simple

    // Keyframes for Ball
	KeyFrame keyframesBall[] = {
        KeyFrame(glm::vec3(0.0, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 0.0),				// Initial key frame
		KeyFrame(glm::vec3(0.235, BALL_Y_OFFSET-0.23, -0.075), glm::vec3(0.0), 1.5),	// Ball Bounce
        KeyFrame(glm::vec3(0.47, BALL_Y_OFFSET, -0.15), glm::vec3(0.0), 3.0),			// Ball hit, move back
		KeyFrame(glm::vec3(-0.235, BALL_Y_OFFSET -0.23, 0.05), glm::vec3(0.0), 5.0),	// Ball Bounce
		KeyFrame(glm::vec3(-0.47, BALL_Y_OFFSET, 0.15), glm::vec3(0.0), 6.0),			// Ball hit, move back
		KeyFrame(glm::vec3(0.375, BALL_Y_OFFSET -0.23, 0.25), glm::vec3(0.0), 8.0),		// Ball Bounce
        KeyFrame(glm::vec3(0.75, BALL_Y_OFFSET, 0.3), glm::vec3(0.0), 9.0),				// SCORE
        KeyFrame(glm::vec3(0.75, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 10.0),			// Stay off-screen for a moment
        KeyFrame(glm::vec3(0.0, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 11.0),				// Reset to center
        KeyFrame(glm::vec3(0.0, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 13.0),				// Stay in center for a moment
		KeyFrame(glm::vec3(-0.235, BALL_Y_OFFSET-0.23, 0.075), glm::vec3(0.0), 14.5),	// Ball Bounce
        KeyFrame(glm::vec3(-0.47, BALL_Y_OFFSET, 0.15), glm::vec3(0.0), 16.0),			// Ball hit, move back
		KeyFrame(glm::vec3(0.235, BALL_Y_OFFSET-0.23, -0.05), glm::vec3(0.0), 18.0),	// Ball Bounce
        KeyFrame(glm::vec3(0.47, BALL_Y_OFFSET, -0.15), glm::vec3(0.0), 19.0),			// Ball hit, move back
		KeyFrame(glm::vec3(-0.375, BALL_Y_OFFSET-0.23, 0.0), glm::vec3(0.0), 21.0),		// Ball Bounce					
        KeyFrame(glm::vec3(-0.75, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 22.0),			// SCORE
        KeyFrame(glm::vec3(-0.75, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 23.0),			// Stay off-screen a moment
        KeyFrame(glm::vec3(0.0, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 24.0),				// Reset to center
        KeyFrame(glm::vec3(0.0, BALL_Y_OFFSET, 0.0), glm::vec3(0.0), 25.0),				// Stay in center for a moment
		KeyFrame(glm::vec3(0.235, BALL_Y_OFFSET-0.23, -0.075), glm::vec3(0.0), 26.5),	// Ball Bounce
        KeyFrame(glm::vec3(0.47, BALL_Y_OFFSET, -0.15), glm::vec3(0.0), 28.0),			// Ball hit, move back
		KeyFrame(glm::vec3(-0.235, BALL_Y_OFFSET-0.23, 0.05), glm::vec3(0.0), 30.0),	// Ball Bounce
        KeyFrame(glm::vec3(-0.47, BALL_Y_OFFSET, 0.15), glm::vec3(0.0), 31.0),			// Ball hit, move back
		KeyFrame(glm::vec3(0.235, BALL_Y_OFFSET-0.23, 0.15), glm::vec3(0.0), 33.0),		// Ball Bounce
        KeyFrame(glm::vec3(0.47, BALL_Y_OFFSET, 0.15), glm::vec3(0.0), 34.0),			// Move to the right
		KeyFrame(glm::vec3(-0.235, BALL_Y_OFFSET-0.23, -0.05), glm::vec3(0.0), 36.0),	// Ball Bounce
        KeyFrame(glm::vec3(-0.47, BALL_Y_OFFSET, -0.15), glm::vec3(0.0), 37.0),			// Ball hit, move back
		KeyFrame(glm::vec3(0.375, BALL_Y_OFFSET-0.23, -0.25), glm::vec3(0.0), 39.0),	// Ball Bounce
        KeyFrame(glm::vec3(0.75, BALL_Y_OFFSET, -0.3), glm::vec3(0.0), 40.0),			// SCORE
	};


	KeyFrame keyframesFireWorks[] = {
		KeyFrame(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), 40.0), // Initial key frame
		KeyFrame(glm::vec3(0.0f, .40f, 0.0f), glm::vec3(0.0f), 45.5), // Start moving for ball
		KeyFrame(glm::vec3(0.0f, .50f, 0.0f), glm::vec3(0.0f), 48.5),
		KeyFrame(glm::vec3(0.0f, -.50f, 0.0f), glm::vec3(0.0f),53.5),
	};
	glm::vec3 fireworkPosition(0, 0, 0);



	// Keyframes for Red player
	KeyFrame keyframesCamera[] = {
		KeyFrame(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-90.0), 0.0), // Initial key frame
		KeyFrame(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-60.0), 2.5), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, -0.035), glm::vec3(-90.0), 3.0), // Hit ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-60.0), 8.0), // Start moving away from ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.18), glm::vec3(-110.0), 8.5), // Be away from ball
		KeyFrame(glm::vec3(0.0, 0.0, -0.005), glm::vec3(-90.0), 11.0), // Start moving back
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-90.0), 12.0), // Be in position
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-90.0), 14.0),
		KeyFrame(glm::vec3(0.0, 0.0, 0.07), glm::vec3(-90.0), 16.0), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-60.0), 18.0), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, -0.035), glm::vec3(-90.0), 19.0), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-90.0), 25.0), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-60.0), 27.3), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, -0.035), glm::vec3(-90.0), 28.0), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-90.0), 31.0), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-60.0), 33.3), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.255), glm::vec3(-90.0), 34.0), // Start moving for ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.1), glm::vec3(-60.0), 39.0), // Start moving away from ball
		KeyFrame(glm::vec3(0.0, 0.0, 0.18), glm::vec3(-110.0), 39.5), // Be away from ball
	};




    // Start at the 0th keyframe for each set of keyframes
    int keyframeNumBlue = 0;
    int keyframeNumRed = 0;
	int keyframeNumBall = 0;
	int keyframeNumCamera = 0;
	int keyframeNumFireWorks = 0;
	int KF_FireWorkSize = (sizeof(keyframesFireWorks) / sizeof(KeyFrame));

	float i = -1;

    // Initialize player scores
	int redScore = 0, blueScore = 0;

    // Decide whether to play sounds or not. Mostly for debug
    bool playSound = true;

    // If we want to play sounds, play a constant crowd chatter
	if (playSound) {
		irrklang::ISoundEngine* bigCrowdSound = irrklang::createIrrKlangDevice();
		irrklang::ISound* sound = bigCrowdSound->play2D("../src/Assets/sounds/BigCrowd.wav", true, false, true);
		bigCrowdSound->setSoundVolume(0.15f);
	}

	for (int i = 0; i < size; ++i) {
		locations[i] = getRandomLocationSides();
	}

	for (int i = 0; i < smallSize; ++i) {
		smallLocations[i] = getRandomLocationFront();
	}

    // Set the current time to be 0. Animation relies on specific times
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
        numberDraw.groupMatrix = groupMatrix;
        numberDraw2.groupMatrix = groupMatrix;
        numberDraw.renderAs = renderAs;
        numberDraw2.renderAs = renderAs;

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
			double frameDuration = nextFrameTime - currentFrameTime;
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

			float currentRotationAngle = keyframesBlue[keyframeNumBlue].rotation.x; // Assuming rotation is a float
			float nextRotationAngle = keyframesBlue[keyframeNumBlue + 1].rotation.x;
			float interpolatedRotationAngle = currentRotationAngle + (nextRotationAngle - currentRotationAngle) * timeProportion;


			playerArm1.setRotation(interpolatedRotationAngle);

			currentRotationAngle = keyframesBlue[keyframeNumBlue].rotation.y; // Assuming rotation is a float
			nextRotationAngle = keyframesBlue[keyframeNumBlue + 1].rotation.y;
			interpolatedRotationAngle = currentRotationAngle + (nextRotationAngle - currentRotationAngle) * timeProportion;
			playerArm1.elbowRotate = interpolatedRotationAngle;

			currentRotationAngle = keyframesBlue[keyframeNumBlue].rotation.z; // Assuming rotation is a float
			nextRotationAngle = keyframesBlue[keyframeNumBlue + 1].rotation.z;
			interpolatedRotationAngle = currentRotationAngle + (nextRotationAngle - currentRotationAngle) * timeProportion;
			playerArm1.wristRotate = interpolatedRotationAngle;
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
			double frameDuration = nextFrameTime - currentFrameTime;
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

			float currentRotationAngle = keyframesRed[keyframeNumRed].rotation.x; // Assuming rotation is a float
			float nextRotationAngle = keyframesRed[keyframeNumRed + 1].rotation.x;
			float interpolatedRotationAngle = currentRotationAngle + (nextRotationAngle - currentRotationAngle) * timeProportion;


			playerArm2.setRotation(interpolatedRotationAngle);

			currentRotationAngle = keyframesRed[keyframeNumRed].rotation.y; // Assuming rotation is a float
			nextRotationAngle = keyframesRed[keyframeNumRed + 1].rotation.y;
			interpolatedRotationAngle = currentRotationAngle + (nextRotationAngle - currentRotationAngle) * timeProportion;
			playerArm2.elbowRotate = interpolatedRotationAngle;

			currentRotationAngle = keyframesRed[keyframeNumRed].rotation.z; // Assuming rotation is a float
			nextRotationAngle = keyframesRed[keyframeNumRed + 1].rotation.z;
			interpolatedRotationAngle = currentRotationAngle + (nextRotationAngle - currentRotationAngle) * timeProportion;
			playerArm2.wristRotate = interpolatedRotationAngle;
			// If the realtime clock is beyond the next keyframes time parameter, move to the next keyframe
			if (currentWorldTime >= nextFrameTime)
				keyframeNumRed++;
		}


		if (keyframeNumBall <= (sizeof(keyframesBall) / sizeof(KeyFrame)) - 2)
		{
			// Handle time calculations
			double currentFrameTime = keyframesBall[keyframeNumBall].time;
			double nextFrameTime = keyframesBall[keyframeNumBall + 1].time;
			double frameDuration = nextFrameTime - currentFrameTime;
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

		if (keyframeNumFireWorks <= (sizeof(keyframesFireWorks) / sizeof(KeyFrame)) - 2)
		{
			// Handle time calculations
			double currentFrameTime = keyframesFireWorks[keyframeNumFireWorks].time;
			double nextFrameTime = keyframesFireWorks[keyframeNumFireWorks + 1].time;
			double frameDuration = nextFrameTime - currentFrameTime;
			double timeProportion = (currentWorldTime - currentFrameTime) / frameDuration;

			// Handle setting how much X should translate based on time proportion
			double currentCoordX = keyframesFireWorks[keyframeNumFireWorks].translation.x;
			double nextCoordX = keyframesFireWorks[keyframeNumFireWorks + 1].translation.x;
			currentCoordX += (nextCoordX - currentCoordX) * timeProportion;

			// Handle setting how much Y should translate based on time proportion
			double currentCoordY = keyframesFireWorks[keyframeNumFireWorks].translation.y;
			double nextCoordY = keyframesFireWorks[keyframeNumFireWorks + 1].translation.y;
			currentCoordY += (nextCoordY - currentCoordY) * timeProportion;

			// Handle setting how much Z should translate based on time proportion
			double currentCoordZ = keyframesFireWorks[keyframeNumFireWorks].translation.z;
			double nextCoordZ = keyframesFireWorks[keyframeNumFireWorks + 1].translation.z;
			currentCoordZ += (nextCoordZ - currentCoordZ) * timeProportion;

			// Set the model translation in world space
			//ball.setTranslationModel(glm::vec3(currentCoordX, currentCoordY, currentCoordZ));
			fireworkPosition =glm::vec3((float)currentCoordX, (float)currentCoordY, (float)currentCoordZ);



			//FireWorks(positiony, unitCubeAO, courtMaterial, worldMatrixLocation, colorLocation);
			// If the realtime clock is beyond the next keyframes time parameter, move to the next keyframe
			if (currentWorldTime >= nextFrameTime)
				keyframeNumFireWorks++;
		}







	
        // Handle changing the score, if necessary
        handleScoring(currentWorldTime, redScore, blueScore);

        // Handle playing sounds, if necessary (and desired)
        if (playSound)
            handleSounds(currentWorldTime);

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

            // Update position of light
			updateLight(glm::vec3(x, lightDepth, z), glm::vec3(0, 0, 0), SceneObj, shaderProgram, true);
			if (i == 1.0f) i = -1.0f;

            // DRAW GEOMETRY
            // Draw scoreboard
			numberDraw.Scoreboard(blueScore,false,true);
			numberDraw2.Scoreboard(redScore,false,false);

            // Draw blue player and racket
			playerArm1.SetAttr(groupMatrix, renderAs, shaderProgram);
			playerArm1.DrawArm();
			racket1.SetAttr(groupMatrix, renderAs, shaderProgram, playerArm1.partParent);
			racket1.Draw();

            // Draw red player and racket
            playerArm2.SetAttr(groupMatrix, renderAs, shaderProgram);
            playerArm2.DrawArm();
            racket2.SetAttr(groupMatrix, renderAs, shaderProgram, playerArm2.partParent);
            racket2.Draw();

            // Draw ball
			ball.setGroupMatrix(groupMatrix);
			ball.setRenderAs(renderAs);
			ball.drawBall();

            // Draw court and skybox
            SceneObj.SetAttr(rotationMatrixW, renderAs, shaderProgram);
            SceneObj.SetVAO(unitCubeAO, gridAO);
            SceneObj.DrawScene(false);  // Draw scene without the skybox, so it can't be used to make shadows on the scene

			// This is all for the bleachers
			//****************
			glm::mat4 bleacherTranslate;
			glm::mat4 bleacherScale;
			glm::mat4 bleacherGroupMatrix;

			bleacherTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.35, .08, -0.75));
			bleacherScale = glm::scale(glm::mat4(1.0f), glm::vec3(.00015f, .00015f, .00015f) * 6.0f);
			glm::mat4 letterParent = bleacherTranslate * bleacherScale;
			bleacherGroupMatrix = groupMatrix * letterParent;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &bleacherGroupMatrix[0][0]);
			metalMaterial.loadToShader();
			metalMaterial.bindTexture();
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.66f, .6f, .66f))); //al have the same colour
			Bleachers.RenderModelBleacher();

			bleacherTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.35, .08, -0.75));
			bleacherScale = glm::scale(glm::mat4(1.0f), glm::vec3(.00015f, .00015f, .00015f) * 6.0f);
			letterParent = bleacherTranslate * bleacherScale;
			bleacherGroupMatrix = groupMatrix * letterParent;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &bleacherGroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.66f, .6f, .66f)));
			Bleachers.RenderModelBleacher();
			//****************

			// This is for the tree
			glm::mat4 tree1Translate;
			glm::mat4 tree1Scale;
			glm::mat4 tree1GroupMatrix;

			tree1Translate = glm::translate(glm::mat4(1.0f), glm::vec3(1.05, 0.0, -0.9));
			tree1Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.0095f, 0.0095f, 0.0095f) * 6.0f);
			glm::mat4 letterParentTree1 = tree1Translate * tree1Scale;
			tree1GroupMatrix = groupMatrix * letterParentTree1;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tree1GroupMatrix[0][0]);
			grassMaterial.loadToShader();
			grassMaterial.bindTexture();
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(51.0f / 255.0f, 153.0f / 255.0f, 51.0f / 255.0f)));
			Trees.RenderModeltree1();

			tree1Translate = glm::translate(glm::mat4(1.0f), glm::vec3(-1.05, 0.0, -0.9));
			tree1Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.0095f, 0.0095f, 0.0095f) * 6.0f);
			letterParentTree1 = tree1Translate * tree1Scale;
			tree1GroupMatrix = groupMatrix * letterParentTree1;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tree1GroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(51.0f / 255.0f, 153.0f / 255.0f, 51.0f / 255.0f)));
			Trees.RenderModeltree1();

			tree1Translate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.95, 0.0, -1.6));
			tree1Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.0095f, 0.0095f, 0.0095f) * 7.0f);
			letterParentTree1 = tree1Translate * tree1Scale;
			tree1GroupMatrix = groupMatrix * letterParentTree1;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tree1GroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(51.0f / 255.0f, 153.0f / 255.0f, 59.0f / 255.0f)));
			Trees.RenderModeltree1();

			tree1Translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.95, 0.0, -1.6));
			tree1Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.0095f, 0.0095f, 0.0095f) * 6.0f);
			letterParentTree1 = tree1Translate * tree1Scale;
			tree1GroupMatrix = groupMatrix * letterParentTree1;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tree1GroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(51.0f / 255.0f, 153.0f / 255.0f, 51.0f / 255.0f)));
			Trees.RenderModeltree1();

			tree1Translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.55, 0.0, -1.6));
			tree1Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.0095f, 0.0095f, 0.0095f) * 7.5f);
			letterParentTree1 = tree1Translate * tree1Scale;
			tree1GroupMatrix = groupMatrix * letterParentTree1;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tree1GroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(51.0f / 255.0f, 150.0f / 255.0f, 51.0f / 255.0f)));
			Trees.RenderModeltree1();

			tree1Translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.1, 0.0, -1.6));
			tree1Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.0095f, 0.0095f, 0.0095f) * 6.0f);
			letterParentTree1 = tree1Translate * tree1Scale;
			tree1GroupMatrix = groupMatrix * letterParentTree1;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tree1GroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(51.0f / 255.0f, 153.0f / 255.0f, 51.0f / 255.0f)));
			Trees.RenderModeltree1();

			tree1Translate = glm::translate(glm::mat4(1.0f), glm::vec3(-.45, 0.0, -1.6));
			tree1Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.0095f, 0.0095f, 0.0095f) * 5.5f);
			letterParentTree1 = tree1Translate * tree1Scale;
			tree1GroupMatrix = groupMatrix * letterParentTree1;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tree1GroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(55.0f / 255.0f, 153.0f / 255.0f, 51.0f / 255.0f)));
			Trees.RenderModeltree1();


			// This is for the grass
			glm::mat4 grassTranslate;
			glm::mat4 grassScale;
			glm::mat4 grassGroupMatrix;
			glm::mat4 letterParentgrass;

			for (int i = 0; i < size; i++)
			{
				grassTranslate = glm::translate(glm::mat4(1.0f),locations[i]);
				grassScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.10f));
				letterParentgrass = grassTranslate * grassScale;
				grassGroupMatrix = groupMatrix * letterParentgrass;
				glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &grassGroupMatrix[0][0]);
				glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(153.0f / 255.0f, 255.0f / 255.0f, 51.0f / 255.0f)));
				Grass.RenderModelGrass();
			}

			for (int i = 0; i < smallSize; i++)
			{
				grassTranslate = glm::translate(glm::mat4(1.0f), smallLocations[i]);
				grassScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.10f));
				letterParentgrass = grassTranslate * grassScale;
				grassGroupMatrix = groupMatrix * letterParentgrass;
				glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &grassGroupMatrix[0][0]);
				glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(153.0f / 255.0f, 255.0f / 255.0f, 51.0f / 255.0f)));
				Grass.RenderModelGrass();
			}

			// This is for the shortshrub
			glm::mat4 shortshrubTranslate;
			glm::mat4 shortshrubScale;
			glm::mat4 shortshrubGroupMatrix;

			shortshrubTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.35, 0.0, 0.5));
			shortshrubScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.12f));
			glm::mat4 letterParentshortshrub = shortshrubTranslate * shortshrubScale;
			shortshrubGroupMatrix = groupMatrix * letterParentshortshrub; 

			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &shortshrubGroupMatrix[0][0]);
			skinMaterial.loadToShader();
			skinMaterial.bindTexture();
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(255.0f / 255.0f, 153.0f / 255.0f, 204.0f / 255.0f)));
			ShortShrub.RenderModelBleacher();


			shortshrubTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.15, 0.0, 0.5));
			shortshrubScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			letterParentshortshrub = shortshrubTranslate * shortshrubScale;
			shortshrubGroupMatrix = groupMatrix * letterParentshortshrub;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &shortshrubGroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(204.0f / 255.0f, 153.0f / 255.0f, 102.0f / 255.0f)));
			ShortShrub.RenderModelBleacher();

			shortshrubTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.55, 0.0, 0.5));
			shortshrubScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.11f));
			letterParentshortshrub = shortshrubTranslate * shortshrubScale;
			shortshrubGroupMatrix = groupMatrix * letterParentshortshrub;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &shortshrubGroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(255.0f / 255.0f, 153.0f / 255.0f, 204.0f / 255.0f)));
			ShortShrub.RenderModelBleacher();

			shortshrubTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.75, 0.0, 0.5));
			shortshrubScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			letterParentshortshrub = shortshrubTranslate * shortshrubScale;
			shortshrubGroupMatrix = groupMatrix * letterParentshortshrub;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &shortshrubGroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(204.0f / 255.0f, 153.0f / 255.0f, 102.0f / 255.0f)));
			ShortShrub.RenderModelBleacher();


			// This is for the tallshrub
			glm::mat4 tallshrubTranslate;
			glm::mat4 tallshrubScale;
			glm::mat4 tallshrubGroupMatrix;

			tallshrubTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.15, 0.0, 0.5));
			tallshrubScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			glm::mat4 letterParenttallshrub = tallshrubTranslate * tallshrubScale;
			tallshrubGroupMatrix = groupMatrix * letterParenttallshrub;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tallshrubGroupMatrix[0][0]);
			skinMaterial.loadToShader();
			skinMaterial.bindTexture();
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(255.0f / 255.0f, 153.0f / 255.0f, 204.0f / 255.0f)));
			TallShrub.RenderModelBleacher();


			tallshrubTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.35, 0.0, 0.5));
			tallshrubScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.13f));
			letterParenttallshrub = tallshrubTranslate * tallshrubScale;
			tallshrubGroupMatrix = groupMatrix * letterParenttallshrub;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tallshrubGroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(204.0f / 255.0f, 153.0f / 255.0f, 102.0f / 255.0f)));
			TallShrub.RenderModelBleacher();

			tallshrubTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.55, 0.0, 0.5));
			tallshrubScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			letterParenttallshrub = tallshrubTranslate * tallshrubScale;
			tallshrubGroupMatrix = groupMatrix * letterParenttallshrub;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tallshrubGroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(255.0f / 255.0f, 153.0f / 255.0f, 204.0f / 255.0f)));
			TallShrub.RenderModelBleacher();

			tallshrubTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.75, 0.0, 0.5));
			tallshrubScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.12f));
			letterParenttallshrub = tallshrubTranslate * tallshrubScale;
			tallshrubGroupMatrix = groupMatrix * letterParenttallshrub;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tallshrubGroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(204.0f / 255.0f, 153.0f / 255.0f, 102.0f / 255.0f)));
			TallShrub.RenderModelBleacher();

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

            // Update position of the light
            updateLight(glm::vec3(x, lightDepth, z), glm::vec3(0, 0, 0), SceneObj, shaderProgram, noShowLightBox);

            // DRAW GEOMETRY
            // Draw scoreboard
            numberDraw.Scoreboard(blueScore, false, true);
            numberDraw2.Scoreboard(redScore, false, false);

            // Draw blue player and racket
			playerArm1.SetAttr(groupMatrix, renderAs, shaderProgram);
			playerArm1.DrawArm();
			racket1.SetAttr(groupMatrix, renderAs, shaderProgram, playerArm1.partParent);
			racket1.Draw();

            // Draw red player and racket
            playerArm2.SetAttr(groupMatrix, renderAs, shaderProgram);
            playerArm2.DrawArm();
            racket2.SetAttr(groupMatrix, renderAs, shaderProgram, playerArm2.partParent);
            racket2.Draw();

            // Draw ball
			ball.setGroupMatrix(groupMatrix);
			ball.setRenderAs(renderAs);
			ball.drawBall();

            // Draw court and sky box
			SceneObj.SetAttr(rotationMatrixW, renderAs, shaderProgram);
			SceneObj.SetVAO(unitCubeAO, gridAO);
			SceneObj.DrawScene(true);  // Draw scene with the skybox

			// This is all for the bleachers
			//****************
			glm::mat4 bleacherTranslate;
			glm::mat4 bleacherScale;
			glm::mat4 bleacherGroupMatrix;
			bleacherTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.35, .08, -0.75));

			bleacherScale = glm::scale(glm::mat4(1.0f), glm::vec3(.00015f, .00015f, .00015f)*6.0f);
			glm::mat4 bleacherParent = bleacherTranslate * bleacherScale;
			bleacherGroupMatrix = groupMatrix * bleacherParent;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &bleacherGroupMatrix[0][0]);
			metalMaterial.bindTexture();
			metalMaterial.loadToShader();
			
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.66f, .6f, .66f)));
			Bleachers.RenderModelBleacher();

			bleacherTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.35, .08, -0.75));
			
			bleacherScale = glm::scale(glm::mat4(1.0f), glm::vec3(.00015f, .00015f, .00015f) * 6.0f);
			bleacherParent = bleacherTranslate * bleacherScale;
			bleacherGroupMatrix = groupMatrix * bleacherParent;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &bleacherGroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(.66f,.6f,.66f)));
			Bleachers.RenderModelBleacher();
            //****************

			// This is all for the tree1
			//****************
			glm::mat4 tree1Translate;
			glm::mat4 tree1Scale;
			glm::mat4 tree1GroupMatrix;

			tree1Translate = glm::translate(glm::mat4(1.0f), glm::vec3(1.05, 0.0, -0.9));
			tree1Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.0095f, 0.0095f, 0.0095f) * 6.0f);
			glm::mat4 letterParentTree1 = tree1Translate * tree1Scale;
			tree1GroupMatrix = groupMatrix * letterParentTree1;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tree1GroupMatrix[0][0]);
			grassMaterial.loadToShader();
			grassMaterial.bindTexture();
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(51.0f / 255.0f, 153.0f / 255.0f, 51.0f / 255.0f)));
			Trees.RenderModeltree1();

			tree1Translate = glm::translate(glm::mat4(1.0f), glm::vec3(-1.05, 0.0, -0.9));
			tree1Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.0095f, 0.0095f, 0.0095f) * 6.0f);
			letterParentTree1 = tree1Translate * tree1Scale;
			tree1GroupMatrix = groupMatrix * letterParentTree1;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tree1GroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(51.0f / 255.0f, 153.0f / 255.0f, 51.0f / 255.0f)));
			Trees.RenderModeltree1();

			tree1Translate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.95, 0.0, -1.6));
			tree1Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.0095f, 0.0095f, 0.0095f) * 7.0f);
			letterParentTree1 = tree1Translate * tree1Scale;
			tree1GroupMatrix = groupMatrix * letterParentTree1;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tree1GroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(51.0f / 255.0f, 153.0f / 255.0f, 59.0f / 255.0f)));
			Trees.RenderModeltree1();

			tree1Translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.95, 0.0, -1.6));
			tree1Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.0095f, 0.0095f, 0.0095f) * 6.0f);
			letterParentTree1 = tree1Translate * tree1Scale;
			tree1GroupMatrix = groupMatrix * letterParentTree1;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tree1GroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(51.0f / 255.0f, 153.0f / 255.0f, 51.0f / 255.0f)));
			Trees.RenderModeltree1();

			tree1Translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.55, 0.0, -1.6));
			tree1Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.0095f, 0.0095f, 0.0095f) * 7.5f);
			letterParentTree1 = tree1Translate * tree1Scale;
			tree1GroupMatrix = groupMatrix * letterParentTree1;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tree1GroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(51.0f / 255.0f, 150.0f / 255.0f, 51.0f / 255.0f)));
			Trees.RenderModeltree1();

			tree1Translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.1, 0.0, -1.6));
			tree1Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.0095f, 0.0095f, 0.0095f) * 6.0f);
			letterParentTree1 = tree1Translate * tree1Scale;
			tree1GroupMatrix = groupMatrix * letterParentTree1;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tree1GroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(51.0f / 255.0f, 153.0f / 255.0f, 51.0f / 255.0f)));
			Trees.RenderModeltree1();

			tree1Translate = glm::translate(glm::mat4(1.0f), glm::vec3(-.45, 0.0, -1.6));
			tree1Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.0095f, 0.0095f, 0.0095f) * 5.5f);
			letterParentTree1 = tree1Translate * tree1Scale;
			tree1GroupMatrix = groupMatrix * letterParentTree1;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tree1GroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(55.0f / 255.0f, 153.0f / 255.0f, 51.0f / 255.0f)));
			Trees.RenderModeltree1();


			// This is for the grass
			glm::mat4 grassTranslate;
			glm::mat4 grassScale;
			glm::mat4 grassGroupMatrix;
			glm::mat4 letterParentgrass;


			for (int i = 0; i < size; i++)
			{
				grassTranslate = glm::translate(glm::mat4(1.0f), locations[i]);
				grassScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.10f));
				letterParentgrass = grassTranslate * grassScale;
				grassGroupMatrix = groupMatrix * letterParentgrass;
				glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &grassGroupMatrix[0][0]);
				glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(153.0f / 255.0f, 255.0f / 255.0f, 51.0f / 255.0f)));
				Grass.RenderModelGrass();
			}

			for (int i = 0; i < smallSize; i++)
			{
				grassTranslate = glm::translate(glm::mat4(1.0f), smallLocations[i]);
				grassScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.10f));
				letterParentgrass = grassTranslate * grassScale;
				grassGroupMatrix = groupMatrix * letterParentgrass;
				glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &grassGroupMatrix[0][0]);
				glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(153.0f / 255.0f, 255.0f / 255.0f, 51.0f / 255.0f)));
				Grass.RenderModelGrass();
			}

			// This is for the shortshrub
			glm::mat4 shortshrubTranslate;
			glm::mat4 shortshrubScale;
			glm::mat4 shortshrubGroupMatrix;

			shortshrubTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.35, 0.0, 0.5));
			shortshrubScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.12f));
			glm::mat4 letterParentshortshrub = shortshrubTranslate * shortshrubScale;
			shortshrubGroupMatrix = groupMatrix * letterParentshortshrub;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &shortshrubGroupMatrix[0][0]);
			skinMaterial.loadToShader();
			skinMaterial.bindTexture();
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(255.0f / 255.0f, 153.0f / 255.0f, 204.0f / 255.0f)));
			ShortShrub.RenderModelBleacher();

			shortshrubTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.15, 0.0, 0.5));
			shortshrubScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			letterParentshortshrub = shortshrubTranslate * shortshrubScale;
			shortshrubGroupMatrix = groupMatrix * letterParentshortshrub;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &shortshrubGroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(204.0f / 255.0f, 153.0f / 255.0f, 102.0f / 255.0f)));
			ShortShrub.RenderModelBleacher();

			shortshrubTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.55, 0.0, 0.5));
			shortshrubScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.11f));
			letterParentshortshrub = shortshrubTranslate * shortshrubScale;
			shortshrubGroupMatrix = groupMatrix * letterParentshortshrub;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &shortshrubGroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(255.0f / 255.0f, 153.0f / 255.0f, 204.0f / 255.0f)));
			ShortShrub.RenderModelBleacher();

			shortshrubTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.75, 0.0, 0.5));
			shortshrubScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			letterParentshortshrub = shortshrubTranslate * shortshrubScale;
			shortshrubGroupMatrix = groupMatrix * letterParentshortshrub;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &shortshrubGroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(204.0f / 255.0f, 153.0f / 255.0f, 102.0f / 255.0f)));
			ShortShrub.RenderModelBleacher();

			// This is for the tallshrub
			glm::mat4 tallshrubTranslate;
			glm::mat4 tallshrubScale;
			glm::mat4 tallshrubGroupMatrix;

			tallshrubTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.15, 0.0, 0.5));
			tallshrubScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			glm::mat4 letterParenttallshrub = tallshrubTranslate * tallshrubScale;
			tallshrubGroupMatrix = groupMatrix * letterParenttallshrub;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tallshrubGroupMatrix[0][0]);
			skinMaterial.loadToShader();
			skinMaterial.bindTexture();
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(255.0f / 255.0f, 153.0f / 255.0f, 204.0f / 255.0f)));
			TallShrub.RenderModelBleacher();


			tallshrubTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.35, 0.0, 0.5));
			tallshrubScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.13f));
			letterParenttallshrub = tallshrubTranslate * tallshrubScale;
			tallshrubGroupMatrix = groupMatrix * letterParenttallshrub;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tallshrubGroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(204.0f / 255.0f, 153.0f / 255.0f, 102.0f / 255.0f)));
			TallShrub.RenderModelBleacher();

			tallshrubTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.55, 0.0, 0.5));
			tallshrubScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			letterParenttallshrub = tallshrubTranslate * tallshrubScale;
			tallshrubGroupMatrix = groupMatrix * letterParenttallshrub;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tallshrubGroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(255.0f / 255.0f, 153.0f / 255.0f, 204.0f / 255.0f)));
			TallShrub.RenderModelBleacher();

			tallshrubTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.75, 0.0, 0.5));
			tallshrubScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.12f));
			letterParenttallshrub = tallshrubTranslate * tallshrubScale;
			tallshrubGroupMatrix = groupMatrix * letterParenttallshrub;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &tallshrubGroupMatrix[0][0]);
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(204.0f / 255.0f, 153.0f / 255.0f, 102.0f / 255.0f)));
			TallShrub.RenderModelBleacher();



			//****************


			// This is the red clay court
			glBindVertexArray(unitCubeAO);
			bleacherTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -0.07, -0.0));
			bleacherScale = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 1.0f, 50.00015f) );
			bleacherParent = bleacherTranslate * bleacherScale;
			bleacherGroupMatrix = groupMatrix * bleacherParent;
			
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &bleacherGroupMatrix[0][0]);
			courtMaterial.loadToShader();
			courtMaterial.bindTexture();
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3((float)137/255, (float)72/255,(float)62/255)));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			//******************
		}

		
		//FireWorks(fireworkPosition, unitCubeAO, courtMaterial, worldMatrixLocation, colorLocation);

		//FireWorks(glm::vec3(fireworkPosition.x+.75, fireworkPosition.y, fireworkPosition.z), unitCubeAO, courtMaterial, worldMatrixLocation, colorLocation);
		//FireWorks(glm::vec3(fireworkPosition.x - .75, fireworkPosition.y, fireworkPosition.z), unitCubeAO, courtMaterial, worldMatrixLocation, colorLocation);
		
		if (glfwGetTime() >= 48) { //bigger TIME first
			FireWorks(glm::vec3(fireworkPosition.x + .75, fireworkPosition.y, fireworkPosition.z - .36), unitCubeAO, courtMaterial, worldMatrixLocation, colorLocation, true);

			FireWorks(glm::vec3(fireworkPosition.x - .75, fireworkPosition.y, fireworkPosition.z + .36), unitCubeAO, courtMaterial, worldMatrixLocation, colorLocation, true);

			FireWorks(glm::vec3(fireworkPosition.x + .75, fireworkPosition.y, fireworkPosition.z + .36), unitCubeAO, courtMaterial, worldMatrixLocation, colorLocation, true);

			FireWorks(glm::vec3(fireworkPosition.x - .75, fireworkPosition.y, fireworkPosition.z - .36), unitCubeAO, courtMaterial, worldMatrixLocation, colorLocation, true);
		}
		else if (glfwGetTime() >= 40 ) {
			FireWorks(glm::vec3(fireworkPosition.x + .75, fireworkPosition.y, fireworkPosition.z - .36), unitCubeAO, courtMaterial, worldMatrixLocation, colorLocation,false);

			FireWorks(glm::vec3(fireworkPosition.x - .75, fireworkPosition.y, fireworkPosition.z + .36), unitCubeAO, courtMaterial, worldMatrixLocation, colorLocation, false);

			FireWorks(glm::vec3(fireworkPosition.x + .75, fireworkPosition.y, fireworkPosition.z + .36), unitCubeAO, courtMaterial, worldMatrixLocation, colorLocation, false);

			FireWorks(glm::vec3(fireworkPosition.x - .75, fireworkPosition.y, fireworkPosition.z - .36), unitCubeAO, courtMaterial, worldMatrixLocation, colorLocation, false);
		}


		//FireWorks(fireworkPosition, unitCubeAO, courtMaterial, worldMatrixLocation, colorLocation);
		//
		//FireWorks(fireworkPosition, unitCubeAO, courtMaterial, worldMatrixLocation, colorLocation);
        // Draw and animate the crowd and the walking figures in from of the bleachers
		crowd.animateCrowd(); //just to trigger animation
		crowd.drawCrowd(4);
		crowd.walker();

		// Move the fingers for the players
		playerArm1.flexFingers();
        playerArm2.flexFingers();

		// Handle camera auto rotation
		if (shouldRotateCamera) {
			if (camCounter < 90 && camCounter >=-90)
			{
				rotationMatrixW *= glm::rotate(glm::mat4(1.0f), glm::radians(0.55f), glm::vec3(.0f, 1.0f, 0.0f));
				camCounter+=0.55f;
				if (camCounter >=90)
					camCounter = -270.0f;
			}
			else if (camCounter < -90)
			{
				rotationMatrixW *= glm::rotate(glm::mat4(1.0f), glm::radians(0.55f), glm::vec3(.0f, -1.0f, 0.0f));
				camCounter+=0.55f;
			}
		}

        glfwSwapBuffers(window);
        // Get inputs
		glfwPollEvents();
	}
    // Shut down irrKlang
    audioEngine->drop();

	// Shutdown GLFW
	glfwTerminate();

	return 0;
}


void FireWorks(glm::vec3 position, GLuint unitCube,Material courtMaterial,GLint worldMatrixLocation,GLint colorLocation,bool explode) {

	glBindVertexArray(unitCube);
	//if not exploding do this
	if (!explode) {

		glm::mat4 bleacherTranslate = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 bleacherScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, .25f, 0.25f));
		glm::mat4 bleacherParent = bleacherTranslate * bleacherScale;
		glm::mat4 bleacherGroupMatrix = groupMatrix * bleacherParent;

		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &bleacherGroupMatrix[0][0]);
		courtMaterial.loadToShader();
		courtMaterial.bindTexture();

		//set a random colour for main firework
		glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3((float)glm::linearRand(-75, -110), (float)glm::linearRand(-75, -110), (float)glm::linearRand(-75, -110))));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		for (int i = 0; i < 5; i++) {
			//generate smaller cubes to act as fire trails
			bleacherTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(position.x + (float)glm::linearRand(-0.10, .1), position.y - (float)glm::linearRand(0.1, .3), position.z));
			bleacherScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, .05f, 0.05f));
			glm::mat4 bleacherRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)glm::linearRand(0.0, 90.0)), glm::vec3(0, 1, 0));
			bleacherParent = bleacherTranslate * bleacherScale * bleacherRotate;
			bleacherGroupMatrix = groupMatrix * bleacherParent;

			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &bleacherGroupMatrix[0][0]);
			courtMaterial.loadToShader();
			courtMaterial.bindTexture();
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3((float)glm::linearRand(-75, -110), (float)glm::linearRand(-75, -110), (float)glm::linearRand(-75, -110))));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
	else {
		//else EXPLODE!!!!!!!
		for (int i = 0; i < 145; i++) { //generate many random cubes using linear rand to offset position
			glm::mat4 bleacherTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(position.x + (float)glm::linearRand(-0.30, .3), position.y - (float)glm::linearRand(0.1, .3), position.z + (float)glm::linearRand(-0.30, .3)));
			glm::mat4 bleacherScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, .05f, 0.05f));
			glm::mat4  bleacherRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)glm::linearRand(0.0, 90.0)), glm::vec3(0, 1, 0));
			glm::mat4 bleacherParent = bleacherTranslate * bleacherScale * bleacherRotate;
			glm::mat4 bleacherGroupMatrix = groupMatrix * bleacherParent;

			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &bleacherGroupMatrix[0][0]);
			courtMaterial.loadToShader();
			courtMaterial.bindTexture();
			glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3((float)glm::linearRand(-75, -110), (float)glm::linearRand(-75, -110), (float)glm::linearRand(-75, -110))));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}



	}

	glBindVertexArray(0);
	

}

/**
Create a index buffer object for the sphere
@param vertexIndices: The indices for the vertices of the sphere
@param vertices: The vertices of the sphere
@param normals: The normals of the sphere
@param UVs: The UVs of the sphere
@return The IBO
*/
GLuint createVertexArrayElementObject2(std::vector<int> vertexIndices, std::vector<glm::vec3> vertices,
                                       std::vector<glm::vec3> normals, std::vector<glm::vec2> UVs)
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) (vertexIndices.size() * sizeof(unsigned int)), &vertexIndices.front(), GL_STATIC_DRAW);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) (vertices.size() * sizeof(glm::vec3)), &vertices.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0,                     // attribute 0 matches aPos in Vertex Shader
                          3,                     // size
                          GL_FLOAT,              // type
                          GL_FALSE,              // normalized?
                          1 * sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
                          (void*) nullptr        // array buffer offset
    );
    glEnableVertexAttribArray(0);

    //Normals VBO setup taken from lab
    GLuint normals_VBO;
    glGenBuffers(1, &normals_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) (normals.size() * sizeof(glm::vec3)), &normals.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), (GLvoid*) nullptr);
    glEnableVertexAttribArray(1);

    //UVs VBO setup
    GLuint uvs_VBO;
    glGenBuffers(1, &uvs_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) (UVs.size() * sizeof(glm::vec3)), &UVs.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 1 * sizeof(glm::vec3), (GLvoid*) nullptr);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return vertexArrayObject;
}

/**
Create a vertex array object with positions, normals, and UVs
@return The VAO
*/
GLuint createVertexArrayObject2()
{
    // Create VAO
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Create VBO
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

    // Enable position vectors
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedNormalledVertex), (void*) nullptr);
    glEnableVertexAttribArray(0);

    // Enable normal vectors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedNormalledVertex), (void*) sizeof(glm::vec3));
    glEnableVertexAttribArray(1);

    // Enable UV vectors
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedNormalledVertex), (void*) (2 * sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return vertexArrayObject;
}

/**
 * Update the light parameters with each run of the main loop
 * @param newPosition The new position for the light
 * @param newFocus The new focus for the light
 * @param scene The SceneObject to draw the light to
 * @param shaderProg The shader program to use
 * @param i The i value to pass to the SceneObj.DrawLight method
 * @param applyShadow If this is a shadow pass, do not draw the light
 */
void updateLight(glm::vec3 newPosition, glm::vec3 newFocus, SceneObjects scene, GLuint shaderProg, bool applyShadow) {
    // light parameters
    glm::vec3 lightPosition(-.30f, .30f, .0f); // the location of the light in 3D space
    glm::vec3 lightFocus(0.0, 0.0, 0.0);      // the point in 3D space the light "looks" at
    glm::vec3 lightDirection = glm::normalize(newFocus - newPosition);
    float lightNearPlane = 0.1f;
    float lightFarPlane = 180.0f;

    // Get light-related uniform locations
    GLint lightPositionLoc = glGetUniformLocation(shaderProg, "lightPosition");
    GLint lightDirectionLoc = glGetUniformLocation(shaderProg, "lightDirection");
    GLint lightViewProjMatrixLoc = glGetUniformLocation(shaderProg, "lightViewProjMatrix");

    // Set light position on scene shader
    glUniform3fv(lightPositionLoc, 1, &newPosition[0]);

    // Set light direction on scene shader
    glUniform3fv(lightDirectionLoc, 1, &lightDirection[0]);

    // Create light space matrix
    glm::mat4 lightProjectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, lightNearPlane, lightFarPlane);
    glm::mat4 lightViewMatrix = glm::lookAt(newPosition, newFocus, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;

    // Send light space matrix to shader
    glUniformMatrix4fv(lightViewProjMatrixLoc, 1, GL_FALSE, &lightSpaceMatrix[0][0]);

    // If this is the shadow pass, don't draw the light
    if (!applyShadow)
        scene.DrawLight(newPosition, glm::vec3(0.0f, 1.0f, 0.0f));
}

/**
Create a vertex array object for the grid
@param vertexArray: A pointer to the vertex array to use
@param arraySize: How many entries are in the vertex array
@return The VAO
*/
GLuint createVertexArrayObject(const glm::vec3* vertexArray, int arraySize)
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, arraySize, vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*) nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return vertexArrayObject;
}

/** Set up the shadow map
 *
 * @param DEPTH_MAP_TEXTURE_SIZE: The size of the depth map texture. Should be a power of 2 and cover viewport size, but not more
 * @param depth_map_texture: The depth map texture
 * @param depth_map_fbo: The depth map frame buffer object
 */
void setUpShadowMap(const unsigned int& DEPTH_MAP_TEXTURE_SIZE, GLuint &depth_map_texture, GLuint &depth_map_fbo) {
    glGenTextures(1, &depth_map_texture);
    glBindTexture(GL_TEXTURE_2D, depth_map_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, (GLsizei) DEPTH_MAP_TEXTURE_SIZE,
                 (GLsizei) DEPTH_MAP_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glGenFramebuffers(1, &depth_map_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map_texture, 0);
    glReadBuffer(GL_NONE);
    glDrawBuffer(GL_NONE);
    glBindTexture(GL_TEXTURE_2D, 0);
    GLint kDepthMap = glGetUniformLocation(shaderProgram, "shadowMap");
    glUniform1i(kDepthMap, 2);
}

/// Set up the lighting by sending necessary values to the shader program
void setUpLighting() {
    // Set some lighting variables
    float lightAngleOuter = 10.0;
    float lightAngleInner = 0.01;
    float lightNearPlane = 0.01f;
    float lightFarPlane = 180.0f;
    glm::vec3 lightPosition(-0.0f, 30.0f, .0f); // The location of the light in 3D space
    glm::vec3 lightFocus(0.0, -0.01, .0f);      // The point in 3D space the light "looks" at
    glm::vec3 lightDirection = glm::normalize(lightFocus - lightPosition);
    glm::mat4 lightProjectionMatrix = glm::ortho(-10.5f, 10.50f, -10.50f, 10.50f, lightNearPlane, lightFarPlane); 
    glm::mat4 lightViewMatrix = glm::lookAt(lightPosition, lightFocus, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;

    // Get lighting-related uniform locations
    GLint lightViewProjMatrixLoc = glGetUniformLocation( shaderProgram, "lightViewProjMatrix");
    GLint lightNearPlaneLoc      = glGetUniformLocation( shaderProgram, "lightNearPlane");
    GLint lightFarPlaneLoc       = glGetUniformLocation( shaderProgram, "lightFarPlane");
    GLint lightPositionLoc       = glGetUniformLocation( shaderProgram, "lightPosition");
    GLint lightDirectionLoc      = glGetUniformLocation( shaderProgram, "lightDirection");
    GLint lightCutoffInnerLoc    = glGetUniformLocation( shaderProgram, "lightCutoffInner");
    GLint lightCutoffOuterLoc    = glGetUniformLocation( shaderProgram, "lightCutoffOuter");
    GLint lightColorLoc          = glGetUniformLocation( shaderProgram, "lightColor");

    // Send values to shader program
    glUniform1f(lightCutoffInnerLoc, cos(glm::radians(lightAngleInner)));
    glUniform1f(lightCutoffOuterLoc, cos(glm::radians(lightAngleOuter)));
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
    glUniformMatrix4fv(lightViewProjMatrixLoc, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
    glUniform1f(lightNearPlaneLoc, lightNearPlane);
    glUniform1f(lightFarPlaneLoc, lightFarPlane);
    glUniform3fv(lightPositionLoc, 1, &lightPosition[0]);
    glUniform3fv(lightDirectionLoc, 1, &lightDirection[0]);
}

/** Handle playing the sounds
 * 
 * @param currentTime: The current time according to glfwGetTime() 
 */ 
void handleSounds(double currentTime) {
	
	if (currentTime >= 1.5 && currentTime < 1.6 && !soundPlayed)
	{
		ball.playBounceSound();
		soundPlayed = true;
	}
	else if (currentTime >= 1.7 && currentTime < 1.8)
		soundPlayed = false;

    if (currentTime >= 3 && currentTime < 3.1 && !soundPlayed)
    {
        ball.playSound();
        soundPlayed = true;
    }
    else if (currentTime >= 3.2 && currentTime < 3.3)
        soundPlayed = false;

	else if (currentTime >= 5 && currentTime < 5.1 && !soundPlayed)
	{
		ball.playBounceSound();
		soundPlayed = true;
	}
	else if (currentTime >= 5.2 && currentTime < 5.3)
		soundPlayed = false;

    else if (currentTime >= 6 && currentTime < 6.1 && !soundPlayed)
    {
        ball.playSound();
        soundPlayed = true;
    }
    else if (currentTime >= 6.2 && currentTime < 6.3)
        soundPlayed = false;


	else if (currentTime >= 8 && currentTime < 8.1 && !soundPlayed)
	{
		ball.playBounceSound();
		soundPlayed = true;
	}
	else if (currentTime >= 8.2 && currentTime < 8.3)
		soundPlayed = false;

    else if (currentTime >= 9 && currentTime < 9.1 && !soundPlayed)
    {
        SceneObj.playCrowdSound(true);
        soundPlayed = true;
    }
    else if (currentTime >= 9.2 && currentTime < 9.3)
        soundPlayed = false;

	else if (currentTime >= 14.5 && currentTime < 14.6 && !soundPlayed)
	{
		ball.playBounceSound();
		soundPlayed = true;
	}
	else if (currentTime >= 14.7 && currentTime < 14.8)
		soundPlayed = false;

    else if (currentTime >= 16 && currentTime < 16.1 && !soundPlayed)
    {
        ball.playSound();
        soundPlayed = true;
    }
    else if (currentTime >= 16.2 && currentTime < 16.3)
        soundPlayed = false;

	else if (currentTime >= 18 && currentTime < 18.1 && !soundPlayed)
	{
		ball.playBounceSound();
		soundPlayed = true;
	}
	else if (currentTime >= 18.2 && currentTime < 18.3)
		soundPlayed = false;

    else if (currentTime >= 19 && currentTime < 19.1 && !soundPlayed)
    {
        ball.playSound();
        soundPlayed = true;
    }
    else if (currentTime >= 19.2 && currentTime < 19.3)
        soundPlayed = false;

	else if (currentTime >= 21 && currentTime < 21.1 && !soundPlayed)
	{
		ball.playBounceSound();
		soundPlayed = true;
	}
	else if (currentTime >= 21.2 && currentTime < 21.3)
		soundPlayed = false;

    else if (currentTime >= 22 && currentTime < 22.1 && !soundPlayed)
    {
        SceneObj.playCrowdSound(false);
        soundPlayed = true;
    }
    else if (currentTime >= 22.2 && currentTime < 22.3)
        soundPlayed = false;

	else if (currentTime >= 26.5 && currentTime < 26.6 && !soundPlayed)
	{
		ball.playBounceSound();
		soundPlayed = true;
	}
	else if (currentTime >= 26.7 && currentTime < 26.8)
		soundPlayed = false;

    else if (currentTime >= 28 && currentTime < 28.1 && !soundPlayed)
    {
        ball.playSound();
        soundPlayed = true;
    }
    else if (currentTime >= 28.2 && currentTime < 28.3)
        soundPlayed = false;

	else if (currentTime >= 30 && currentTime < 30.1 && !soundPlayed)
	{
		ball.playBounceSound();
		soundPlayed = true;
	}
	else if (currentTime >= 30.2 && currentTime < 30.3)
		soundPlayed = false;

    else if (currentTime >= 31 && currentTime < 31.1 && !soundPlayed)
    {
        ball.playSound();
        soundPlayed = true;
    }
    else if (currentTime >= 31.2 && currentTime < 31.3)
        soundPlayed = false;

	else if (currentTime >= 33 && currentTime < 33.1 && !soundPlayed)
	{
		ball.playBounceSound();
		soundPlayed = true;
	}
	else if (currentTime >= 33.2 && currentTime < 33.3)
		soundPlayed = false;

    else if (currentTime >= 34 && currentTime < 34.1 && !soundPlayed)
    {
        ball.playSound();
        soundPlayed = true;
    }
    else if (currentTime >= 34.2 && currentTime < 34.3)
        soundPlayed = false;

	else if (currentTime >= 36 && currentTime < 36.1 && !soundPlayed)
	{
		ball.playBounceSound();
			soundPlayed = true;
	}
	else if (currentTime >= 36.2 && currentTime < 36.3)
		soundPlayed = false;

    else if (currentTime >= 37 && currentTime < 37.1 && !soundPlayed)
    {
        ball.playSound();
        soundPlayed = true;
    }
    else if (currentTime >= 37.2 && currentTime < 37.3)
        soundPlayed = false;


	else if (currentTime >= 39 && currentTime < 39.1 && !soundPlayed)
	{
		ball.playBounceSound();
		soundPlayed = true;
	}
	else if (currentTime >= 39.2 && currentTime < 39.3)
		soundPlayed = false;

    else if (currentTime >= 40 && currentTime < 40.1 && !soundPlayed)
    {
        SceneObj.playCrowdSound(true);
        soundPlayed = true;
    }
    else if (currentTime >= 40.2 && currentTime < 40.3)
        soundPlayed = false;
}

/** Handle incrementing the scoreboard
 *
 * @param currentTime: The current time according to glfwGetTime()
 * @param redScore: A reference to the red player's score
 * @param blueScore: A reference to the blue player's score
 */
void handleScoring(double currentTime, int& redScore, int& blueScore) {
    if (currentTime >= 9.0 && currentTime < 9.1 && !scoreIncremented)
    {
        blueScore = 15;
        scoreIncremented = true;
    }
    else if (currentTime >= 9.1 && currentTime < 9.2)
        scoreIncremented = false;

    else if (currentTime >= 22.0 && currentTime < 22.1 && !scoreIncremented)
    {
        redScore = 15;
        scoreIncremented = true;
    }
    else if (currentTime >= 22.1 && currentTime < 22.2)
        scoreIncremented = false;

    else if (currentTime >= 40.0 && currentTime < 40.1 && !scoreIncremented)
    {
        blueScore = 30;
        scoreIncremented = true;
    }
    else if (currentTime >= 40.1 && currentTime < 40.2)
        scoreIncremented = false;
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
@param vertex The name of the vertex shader file
@param fragment The name of the fragment shader file
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
	int state_ESC   = glfwGetKey(window, GLFW_KEY_ESCAPE);
	int state_UP    = glfwGetKey(window, GLFW_KEY_UP);
	int state_DOWN  = glfwGetKey(window, GLFW_KEY_DOWN);
	int state_LEFT  = glfwGetKey(window, GLFW_KEY_LEFT);
	int state_RIGHT = glfwGetKey(window, GLFW_KEY_RIGHT);
	int state_HOME  = glfwGetKey(window, GLFW_KEY_HOME);
	int state_P     = glfwGetKey(window, GLFW_KEY_P);
    int state_L     = glfwGetKey(window, GLFW_KEY_L);
	int state_T     = glfwGetKey(window, GLFW_KEY_T);
	int state_B     = glfwGetKey(window, GLFW_KEY_B);
	int state_X     = glfwGetKey(window, GLFW_KEY_X);
	int state_Y     = glfwGetKey(window, GLFW_KEY_Y);

	// If ESC is pressed, window should close
	if (state_ESC == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

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

    else if (state_L == GLFW_PRESS)
        renderAs = GL_LINES;

	else if (state_T == GLFW_PRESS)
		renderAs = GL_TRIANGLES;

	// If HOME is pressed, remove translations, rotations, and scalings
	else if (state_HOME == GLFW_PRESS) {		
		GroupMatrixScale = glm::vec3(1.0f);
		rotationMatrixW = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 initViewMatrix = glm::lookAt(eye, center, up);
		glm::mat4 projectionMatrix = glm::perspective(FOV, AR, near, far);
		setProjectionMatrix(shaderProgram, projectionMatrix);
		setViewMatrix(shaderProgram, initViewMatrix);
	}

	// If b is pressed, toggle shadows
	else if (state_B == GLFW_PRESS)
		shouldApplyShadows = !shouldApplyShadows;

	// If x is pressed, toggle textures
	else if (state_X == GLFW_PRESS)
		shouldApplyTextures = !shouldApplyTextures;

	// If y is pressed, toggle auto camera rotation
	else if (state_Y == GLFW_PRESS)
		shouldRotateCamera = !shouldRotateCamera;
}

/**
GLFW callback function for handling mouse button and position
*/
void mouseCursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
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

		glm::mat4 initViewMatrix = glm::lookAt(eye, glm::vec3(translateW, translateY, 0.0f), up);
		setViewMatrix(shaderProgram, initViewMatrix);

		lastMousePosY = yPos;
	}

	else if (state_RIGHT == GLFW_PRESS)
	{
		double dx = xPos - lastMousePosX;

		if (dx < 0)
			translateW -= .005;
		else if (dx > 0)
			translateW += .005;

		glm::mat4 initViewMatrix = glm::lookAt(eye, glm::vec3(translateW + center.x, translateY + center.y, 0.0f), up);
		setViewMatrix(shaderProgram, initViewMatrix);
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

		glm::mat4 projectionMatrix = glm::perspective(translateZ + FOV, (float)WIDTH / (float)HEIGHT, .01f, 50.0f);
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


// Randomly determines a location for the grass to spawn on sides
glm::vec3 getRandomLocationSides() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> disX(-2.0, 2.0);
	std::uniform_real_distribution<> disZ(-1.4, 1.4);

	glm::vec3 location;

	do {
		location.x = disX(gen);
	} while (location.x > -0.8 && location.x < 0.8); // Ensuring that the x value is in the specified range

	location.z = disZ(gen);
	location.y = 0; // or any other value you want

	//std::cout << location.x << location.y << /*location*/.z << std::endl;


	return location;
}


// Randomly determines a location for the grass to spawn in front
glm::vec3 getRandomLocationFront() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> disX(-0.80, 0.80);
	std::uniform_real_distribution<> disZ(0.4, 1.4);

	glm::vec3 location;

	 // Ensuring that the x value is in the specified range
	location.x = disX(gen);
	location.z = disZ(gen);
	location.y = 0; // or any other value you want

	//std::cout << location.x << location.y << location.z << std::endl;


	return location;
}
