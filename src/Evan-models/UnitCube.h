#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <iostream>

using namespace glm;

/*
 * Code taken from lab 3
 */
GLuint createUnitCubeVAO(vec3 leftColor,
                     vec3 backColor,
                     vec3 bottomColor,
                     vec3 frontColor,
                     vec3 rightColor,
                     vec3 topColor);