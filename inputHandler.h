#pragma once
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs
#include <glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <gtc/type_ptr.hpp>

class inputHandler {
private:
	glm::vec3 john;

};

void KeyInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	switch (key) {
	case(GLFW_KEY_A):
		//sub case 1
		//sub case 2
	case(GLFW_KEY_D):
		//sub case 1
		//sub case 2
	case(GLFW_KEY_W):
	case(GLFW_KEY_S):
	case(GLFW_KEY_U):
	case(GLFW_KEY_J):
	case(GLFW_KEY_HOME):
	case(GLFW_KEY_SPACE):
	case(GLFW_KEY_LEFT):
	case(GLFW_KEY_RIGHT):
	case(GLFW_KEY_UP):
	case(GLFW_KEY_DOWN):
	case(GLFW_KEY_P):
	case(GLFW_KEY_L):
	case(GLFW_KEY_T):
	//mouse1
	// mouse2
	// mouse3
		//19- 20 with mouse
	
		break;
	}
}