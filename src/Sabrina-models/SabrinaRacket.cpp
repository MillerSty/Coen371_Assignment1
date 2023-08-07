#include "SabrinaRacket.h"

SabrinaRacket::SabrinaRacket()
{
	worldMatrix = glm::mat4(1.0f);
	initialScale = glm::vec3(0.2f);
	renderAs = GL_TRIANGLES;

	this->groupMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(.35f, 0.30f, -0.20f));
	//mattRacket = racket;
}

void SabrinaRacket::setArmColor(glm::vec3 color) { armColor = color; }

glm::vec3 SabrinaRacket::getArmColor() { return armColor; }

void SabrinaRacket::setGroupMatrix(glm::mat4 groupMat) { groupMatrix = groupMat; }

glm::mat4 SabrinaRacket::getGroupMatrix() { return groupMatrix; }

void SabrinaRacket::setWorldMatrix(glm::mat4 worldMat) { worldMatrix = worldMat; }

glm::mat4 SabrinaRacket::getWorldMatrix() { return worldMatrix; }

void SabrinaRacket::setShaderProgram(GLuint shaderProg) { shaderProgram = shaderProg; }

void SabrinaRacket::setInitialPosition(glm::vec3 postion) { initialPosition = postion; }

glm::vec3 SabrinaRacket::getInitialPosition() { return initialPosition; }

void SabrinaRacket::setInitialScale(glm::vec3 scale) { initialScale = scale; }

glm::vec3 SabrinaRacket::getInitialScale() { return initialScale; }

GLuint SabrinaRacket::getShaderProgram() { return shaderProgram; }

void SabrinaRacket::setVAO(int vao) { unitCubeVAO = vao; }

int SabrinaRacket::getVAO() { return unitCubeVAO; }

void SabrinaRacket::setRenderAs(int as) { renderAs = as; }

int SabrinaRacket::getRenderAs() { return renderAs; }

float scaleConversion = 4.281f;


void SabrinaRacket::drawRacket()
{
	drawracketpole();
	drawracketpoleright1();
	drawracketpoleright2();
	drawracketpoleright3();
	drawracketpoleright4();
	drawracketpoleleft1();
	drawracketpoleleft2();
	drawracketpoleleft3();
	drawracketpoleleft4();
	drawracketmidtop();
	//drawracketmidtop2();
	//drawracketgrid();
}

void SabrinaRacket::drawracketpole()
{
	glm::mat4 racketpole = worldMatrix * groupMatrix;
	glm::mat4 racketpoleTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 racketpoleScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 6.0f, 0.5f) / scaleConversion);
	glm::mat4 racketpoleTransform = racketpoleTranslate * racketpoleScale;
	racketpole *= racketpoleTransform;

	glBindVertexArray(unitCubeVAO);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &racketpole[0][0]);

	// Set to beige
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

	glDrawArrays(renderAs, 0, 36);
	glBindVertexArray(0);
}

void SabrinaRacket::drawracketpoleright1()
{
	glm::mat4 racketpoleright1 = worldMatrix * groupMatrix;
	glm::mat4 racketpoleright1Translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, .4f, 0.0f) / scaleConversion);
	glm::mat4 racketpoleright1Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 5.0f, 0.5f) / scaleConversion);
	glm::mat4 racketpoleright1Rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 racketpoleright1Transform = racketpoleright1Translate * racketpoleright1Rotation * racketpoleright1Scale;
	racketpoleright1 *= racketpoleright1Transform;

	glBindVertexArray(unitCubeVAO);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &racketpoleright1[0][0]);

	// Set to beige
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

	glDrawArrays(renderAs, 0, 36);
	glBindVertexArray(0);
}

void SabrinaRacket::drawracketpoleright3()
{
	glm::mat4 racketpoleright3 = worldMatrix * groupMatrix;
	glm::mat4 racketpoleright3Translate = glm::translate(glm::mat4(1.0f), glm::vec3(.3f, 1.0f, 0.0f) / scaleConversion);
	glm::mat4 racketpoleright3Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 2.0f, 0.5f) / scaleConversion);
	glm::mat4 racketpoleright3Rotation = glm::rotate(glm::mat4(1.0f), glm::radians(35.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 racketpoleright3Transform = racketpoleright3Translate * racketpoleright3Rotation * racketpoleright3Scale;
	racketpoleright3 *= racketpoleright3Transform;


	glBindVertexArray(unitCubeVAO);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &racketpoleright3[0][0]);

	// Set to beige
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

	glDrawArrays(renderAs, 0, 36);
	glBindVertexArray(0);
}

void SabrinaRacket::drawracketpoleright2()
{
	glm::mat4 racketpoleright2 = worldMatrix * groupMatrix;
	glm::mat4 racketpoleright2Translate = glm::translate(glm::mat4(1.0f), glm::vec3(.33f, .73f, 0.0f) / scaleConversion);
	glm::mat4 racketpoleright2Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 5.0f, 0.5f) / scaleConversion);
	glm::mat4 racketpoleright2Transform = racketpoleright2Translate * racketpoleright2Scale;
	racketpoleright2 *= racketpoleright2Transform;

	glBindVertexArray(unitCubeVAO);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &racketpoleright2[0][0]);

	// Set to beige
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

	glDrawArrays(renderAs, 0, 36);
	glBindVertexArray(0);
}

void SabrinaRacket::drawracketpoleright4()
{
	glm::mat4 racketpoleright4 = worldMatrix * groupMatrix;
	glm::mat4 racketpoleright4Translate = glm::translate(glm::mat4(1.0f), glm::vec3(.2f, 1.1f, 0.0f) / scaleConversion);
	glm::mat4 racketpoleright4Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 2.0f, 0.5f) / scaleConversion);
	glm::mat4 racketpoleright4Rotation = glm::rotate(glm::mat4(1.0f), glm::radians(70.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 racketpoleright4Transform = racketpoleright4Translate * racketpoleright4Rotation * racketpoleright4Scale;
	racketpoleright4 *= racketpoleright4Transform;

	glBindVertexArray(unitCubeVAO);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &racketpoleright4[0][0]);

	// Set to beige
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

	glDrawArrays(renderAs, 0, 36);
	glBindVertexArray(0);
}

void SabrinaRacket::drawracketpoleleft1()
{
	glm::mat4 racketpoleleft1 = worldMatrix * groupMatrix;
	glm::mat4 racketpoleleft1Translate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.20f, .5f, 0.0f) / scaleConversion);
	glm::mat4 racketpoleleft1Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 5.0f, 0.5f) / scaleConversion);
	glm::mat4 racketpoleleft1Rotation = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 racketpoleleft1Transform = racketpoleleft1Translate * racketpoleleft1Rotation * racketpoleleft1Scale;
	racketpoleleft1 *= racketpoleleft1Transform;

	glBindVertexArray(unitCubeVAO);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &racketpoleleft1[0][0]);

	// Set to beige
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

	glDrawArrays(renderAs, 0, 36);
	glBindVertexArray(0);
}

void SabrinaRacket::drawracketpoleleft2()
{
	glm::mat4 racketpoleleft2 = worldMatrix * groupMatrix;
	glm::mat4 racketpoleleft2Translate = glm::translate(glm::mat4(1.0f), glm::vec3(-.33f, .8f, 0.0f) / scaleConversion);
	glm::mat4 racketpoleleft2Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 5.0f, 0.5f) / scaleConversion);
	glm::mat4 racketpoleleft2Transform = racketpoleleft2Translate * racketpoleleft2Scale;
	racketpoleleft2 *= racketpoleleft2Transform;

	glBindVertexArray(unitCubeVAO);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &racketpoleleft2[0][0]);

	// Set to beige
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

	glDrawArrays(renderAs, 0, 36);
	glBindVertexArray(0);
}

void SabrinaRacket::drawracketpoleleft3()
{
	glm::mat4 racketpoleleft3 = worldMatrix * groupMatrix;
	glm::mat4 racketpoleleft3Translate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, 1.1f, 0.0f) / scaleConversion);
	glm::mat4 racketpoleleft3Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 2.0f, 0.5f) / scaleConversion);
	glm::mat4 racketpoleleft3Rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-35.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 racketpoleleft3Transform = racketpoleleft3Translate * racketpoleleft3Rotation * racketpoleleft3Scale;
	racketpoleleft3 *= racketpoleleft3Transform;

	glBindVertexArray(unitCubeVAO);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &racketpoleleft3[0][0]);

	// Set to beige
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

	glDrawArrays(renderAs, 0, 36);
	glBindVertexArray(0);
}

void SabrinaRacket::drawracketpoleleft4()
{
	glm::mat4 racketpoleleft4 = worldMatrix * groupMatrix;
	glm::mat4 racketpoleleft4Translate = glm::translate(glm::mat4(1.0f), glm::vec3(-.2f, 1.20f, 0.0f) / scaleConversion);
	glm::mat4 racketpoleleft4Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 2.0f, 0.5f) / scaleConversion);
	glm::mat4 racketpoleleft4Rotation = glm::rotate(glm::mat4(1.0f), glm::radians(-70.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 racketpoleleft4Transform = racketpoleleft4Translate * racketpoleleft4Rotation * racketpoleleft4Scale;
	racketpoleleft4 *= racketpoleleft4Transform;

	glBindVertexArray(unitCubeVAO);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &racketpoleleft4[0][0]);

	// Set to beige
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

	glDrawArrays(renderAs, 0, 36);
	glBindVertexArray(0);
}

void SabrinaRacket::drawracketmidtop()
{
	glm::mat4 racketmidtop = worldMatrix * groupMatrix;
	glm::mat4 racketmidtopTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 1.20f, 0.0f) / scaleConversion);
	glm::mat4 racketmidtopScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 3.5f, 0.5f) / scaleConversion);
	glm::mat4 racketmidtopRotation = glm::rotate(glm::mat4(1.0f), glm::radians(-105.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 racketmidtopTransform = racketmidtopTranslate * racketmidtopRotation * racketmidtopScale;
	racketmidtop *= racketmidtopTransform;

	glBindVertexArray(unitCubeVAO);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &racketmidtop[0][0]);

	// Set to beige
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

	glDrawArrays(renderAs, 0, 36);
	glBindVertexArray(0);
}

void SabrinaRacket::drawracketmidtop2()
{
	glm::mat4 racketmidtop2 = worldMatrix * groupMatrix;
	glm::mat4 racketmidtopTranslate2 = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .7f, 0.0f) / scaleConversion);
	glm::mat4 racketmidtopScale2 = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 4.0f, 0.5f) / scaleConversion);
	glm::mat4 racketmidtopRotation2 = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 racketmidtopTransform2 = racketmidtopTranslate2 * racketmidtopRotation2 * racketmidtopScale2;
	racketmidtop2 *= racketmidtopTransform2;

	glBindVertexArray(unitCubeVAO);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &racketmidtop2[0][0]);

	// Set to beige
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

	glDrawArrays(renderAs, 0, 36);
	glBindVertexArray(0);
}

void SabrinaRacket::drawracketgrid()
{
	glm::mat4 racketGrid = worldMatrix * groupMatrix;
	glm::mat4 racketGridTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, .50f, 0.0f) / scaleConversion);
	glm::mat4 racketGridRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 racketGridTransform = racketGridTranslate * racketGridRotation;
	racketGrid *= racketGridTransform;


	glBindVertexArray(unitCubeVAO);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 1, GL_FALSE, &racketGrid[0][0]);

	// Set to beige
	glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));

	glDrawArrays(renderAs, 0, 36);
	glBindVertexArray(0);
}
