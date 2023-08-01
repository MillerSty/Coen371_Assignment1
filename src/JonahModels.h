#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <list>

#define GLEW_STATIC 1   
#include <GL/glew.h>    

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class JonahModels
{
public:
    vec3 initTranslate;
    mat4 armTranslate;
    mat4 armLocal;
    mat4 armRotateMatrix;
    mat4 armParentMatrix;
    mat4 worldMatrix;
    mat4 racketBaseTranslate;
    mat4 racketBaseLocal;
    mat4 racketParentMatrix;
    mat4 upperArmTranslate;
    mat4 upperArmRotate;
    mat4 upperArmLocal;
    mat4 upperArmParent;
    GLuint VAO;

    JonahModels();
    ~JonahModels();

    void drawRacketJ(mat4 armParentMatrix, vec3 modelTranslationVec, GLint colorLocation, GLuint worldMatrixLocation, float rotateAngle, float scaleFactor, float armRotateAngle);
    int createVertexArrayObject();
};