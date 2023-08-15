#include "Material.h"

Material::Material(float diff, float spec, float ambient, float shiny, GLuint texture, int shader) :
diffuseStrength(diff), specularStrength(spec), ambientStrength(ambient), shininessStrength(shiny), textureId(texture), shaderProgram(shader) {}

void Material::loadToShader( ) const {
    // Get necessary uniform locations
	GLint materialDiffuse = glGetUniformLocation(shaderProgram, "mats.diffuseStrength");
	GLint materialSpec = glGetUniformLocation(shaderProgram, "mats.specularStrength");
	GLint materialAmbient = glGetUniformLocation(shaderProgram, "mats.ambientStrength");
	GLint materialShine = glGetUniformLocation(shaderProgram, "mats.shininessStrength");

    // Set relevant uniform values
	glUniform1f(materialDiffuse, diffuseStrength);
	glUniform1f(materialSpec, specularStrength);
	glUniform1f(materialAmbient, ambientStrength);
	glUniform1f(materialShine, shininessStrength);
};

void Material::bindTexture() const {
    // Set active texture to 0
    glActiveTexture(GL_TEXTURE0);

    // Get necessary uniform location
	GLint textureLocation = glGetUniformLocation(shaderProgram, "textureSampler");

    // Bind the given texture to texture 0
	glBindTexture(GL_TEXTURE_2D, textureId);

    // Send result to shader
	glUniform1i(textureLocation, 0);
}

void Material::resetShader() const {
    // Get necessary uniform locations
	GLint materialDiffuse = glGetUniformLocation(shaderProgram, "mats.diffuseStrength");
	GLint materialSpec = glGetUniformLocation(shaderProgram, "mats.specularStrength");
	GLint materialAmbient = glGetUniformLocation(shaderProgram, "mats.ambientStrength");
	GLint materialShine = glGetUniformLocation(shaderProgram, "mats.shininessStrength");

    // Set relevant uniform values
	glUniform1f(materialDiffuse,.5f);
	glUniform1f(materialSpec,.5f);
	glUniform1f(materialAmbient, .5f);
	glUniform1f(materialShine, 32.0f);
}
