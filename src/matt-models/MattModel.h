#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Material.h"

class MattModel
{
private:
	GLuint unitCubeVAO;
	GLuint sphereVAO;
	GLuint shaderProgram;
	glm::mat4 groupMatrix;
	glm::mat4 worldMatrix;
	glm::vec3 initialPosition;
	glm::vec3 initialScale;
	glm::vec3 translationRandom;
	glm::vec3 translationModel;
	float rotationAngle;
	int renderAs = GL_TRIANGLES;
	int sphereVertCount;

	Material racketTexture;
	Material armTexture;
	Material ballTexture;

public:
	MattModel();

	void setGroupMatrix(glm::mat4 groupMat);
	glm::mat4 getGroupMatrix();
	void setWorldMatrix(glm::mat4 worldMat);
	glm::mat4 getWorldMatrix();
	void setShaderProgram(GLuint shaderProg);
	GLuint getShaderProgram();
	void setInitialPosition(glm::vec3 postion);
	glm::vec3 getInitialPosition();
	void setInitialScale(glm::vec3 scale);
	glm::vec3 getInitialScale();
	void setVAO(GLuint cubeVao, GLuint sphereVao);
	GLuint getCubeVAO();
	GLuint getSphereVAO();
	glm::vec3 getTranslationRandom();
	void setTranslationRandom(glm::vec3 trans);
	glm::vec3 getTranslationModel();
	void setTranslationModel(glm::vec3 trans);
	void setRotationAngle(float angle);
	float getRotationAngle();
	void setRenderAs(int as);
	int getRenderAs();
	void setSphereVertCount(int count);
	int getSphereVertCount();

	void setMaterials(Material racket, Material arm, Material ball);

	void resetModel();

	void drawModel();
};
