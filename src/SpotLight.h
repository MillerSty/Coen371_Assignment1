#pragma once
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class SpotLight
{public:
	SpotLight() = default;

	glm::vec3 lightColor ;
	glm::vec3 lightPosition;
	glm::vec3 lightDirection;

	float AmbInt = 1.0f;
	float DiffInt = 1.0f;
	float lightCutoffInner;
	float lightCutoffOuter;

	
	
};

