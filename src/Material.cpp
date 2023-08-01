#include "Material.h"


Material::Material(float diff, float spec, float ambient, float shiny, GLuint texture, int shader) {
	diffuseStrength = diff;
	specularStrength = spec;
	ambientStrength = ambient;
	shininessStrength = shiny;
	textureId = texture;
	shaderProgram = shader;

}

void Material::loadToShader( ) {
	GLint materialDiffuse = glGetUniformLocation(this->shaderProgram, "mats.diffuseStrength");
	GLint materialSpec = glGetUniformLocation(this->shaderProgram, "mats.specularStrength");
	GLint materialAmbient = glGetUniformLocation(this->shaderProgram, "mats.ambientStrength");
	GLint materialShine = glGetUniformLocation(this->shaderProgram, "mats.shininessStrength");

	glUniform1f(materialDiffuse, this->diffuseStrength);
	glUniform1f(materialSpec, this->specularStrength);
	glUniform1f(materialAmbient, this->ambientStrength);
	glUniform1f(materialShine, this->shininessStrength);
};

void Material::bindTexture() {
	glActiveTexture(GL_TEXTURE0);
	GLuint textureLocation = glGetUniformLocation(shaderProgram, "textureSampler");
	glBindTexture(GL_TEXTURE_2D, this->textureId);
	glUniform1i(textureLocation, 0);

}

void Material::resetShader() {
	GLint materialDiffuse = glGetUniformLocation(this->shaderProgram, "mats.diffuseStrength");
	GLint materialSpec = glGetUniformLocation(this->shaderProgram, "mats.specularStrength");
	GLint materialAmbient = glGetUniformLocation(this->shaderProgram, "mats.ambientStrength");
	GLint materialShine = glGetUniformLocation(this->shaderProgram, "mats.shininessStrength");

	glUniform1f(materialDiffuse,.5f);
	glUniform1f(materialSpec,.5f);
	glUniform1f(materialAmbient, .5f);
	glUniform1f(materialShine, 32.0f);
};