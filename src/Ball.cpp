#include "Ball.h"

Ball::Ball()
{
	worldMatrix = glm::mat4(1.0f);
	translationModel = glm::vec3(0.0f);
}

void Ball::setGroupMatrix(glm::mat4 groupMat) { groupMatrix = groupMat; }

glm::mat4 Ball::getGroupMatrix() { return groupMatrix; }

void Ball::setWorldMatrix(glm::mat4 worldMat) { worldMatrix = worldMat; }

glm::mat4 Ball::getWorldMatrix() { return worldMatrix; }

void Ball::setShaderProgram(GLuint shaderProg) { shaderProgram = shaderProg; }

GLuint Ball::getShaderProgram() { return shaderProgram; }

void Ball::setInitialPosition(glm::vec3 postion) { initialPosition = postion; }

glm::vec3 Ball::getInitialPosition() { return initialPosition; }

void Ball::setVAO(GLuint vao) { VAO = vao; }

GLuint Ball::getVAO() { return VAO; }

glm::vec3 Ball::getTranslationModel() { return translationModel; }

void Ball::setTranslationModel(glm::vec3 trans) { translationModel = trans; }

void Ball::setRenderAs(int as) { renderAS = as; }

int Ball::getRenderAs() { return renderAS; }

void Ball::setSphereVertCount(int count) { sphereVertCount = count; }

int Ball::getSphereVertCount() { return sphereVertCount; }

void Ball::setMaterial(Material material) { ballTexture = material; }

void Ball::setSoundEngine(irrklang::ISoundEngine *engine) { audioEngine = engine; }

irrklang::ISoundEngine *Ball::getSoundEngine() { return audioEngine; }

void Ball::resetModel() { translationModel -= translationModel; }

void Ball::drawBall()
{
	// Bind sphere VAO
	glBindVertexArray(VAO);

	// Set color
	glm::vec3 SPHERE_COLOR(0.0f, 1.0f, 0.0f);
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, &SPHERE_COLOR[0]);

	// Apply texture
	ballTexture.bindTexture();
	ballTexture.loadToShader();

	// Set transformations
	glm::mat4 modelMat = groupMatrix;

	glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.35f));
	
//	position = translationModel + initialPosition;

    // Translate only in the XY plane
	glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0f), translationModel + initialPosition);

	modelMat *= modelTranslate * modelScale;

	// Draw geometry
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &modelMat[0][0]);
	glDrawElements(renderAS, sphereVertCount, GL_UNSIGNED_INT, nullptr);

	// Unbind sphere VAO
	glBindVertexArray(0);
}

void Ball::playSound() {
    // Get a random number between 1 and 3 for which ball sound to play.
    // Makes it sound more organic to have random different sounds
    // Code from https://stackoverflow.com/a/5891824
    int whichSound = (rand() % 3);

    audioEngine->play2D(BALL_SOUNDS[whichSound]);

    // This is how we would apply sound effects like reverb, in case we wanted to
    /*
    irrklang::ISound* sound = audioEngine->play2D(BALL_SOUNDS[whichSound], false, false, true, irrklang::ESM_AUTO_DETECT, true);
    irrklang::ISoundEffectControl* fx = sound->getSoundEffectControl();
    fx->enableWavesReverbSoundEffect(0.0f, 0.0f, 1000.0f, 0.001f);

    // Since we're using sound effects, must manually drop the sounds to avoid memory leaks
    sound->drop();
     */
}

void Ball::playBounceSound()
{
	int whichSound = (rand() % 4);

	audioEngine->play2D(BOUNCE_SOUND[whichSound]);
}

void Ball::playBigCrowdSound()
{
	audioEngine->play2D(BIG_CROWD_SOUND, true);
}


