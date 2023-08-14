#include "Letters.h"

// DEFINE SOME COLORS USING THE PREPROCESSOR
#define white glm::vec3(1,1,1)
#define black glm::vec3(0,0,0)
#define blue glm::vec3(0,0,1)
#define red glm::vec3(1,0,0)
//ifdef clock white=red

// DEFINE SOME NUMBERS USING THH PREPROCESSOR. WORKS LIKE A 7-SEGMENT DISPLAY
#define zero  {white,white,white,white,white,white,black}
#define one  {white,white,black,black,black,black,black}
#define two  {black,white,white,black,white,white,white}
#define three  {white,white,black,black,white,white,white}
#define four  {white,white,black,white,black,black,white}
#define five  {white,black,black,white,white,white,white}
#define six  {white,black,white,white,white,white,white}
#define seven  {white,white,black,black,black,white,black}
#define eight  {white,white,white,white,white,white,white}
#define nine  {white,white,black,white,black,white,white}

Letters::Letters(GLuint vao, int shaderProg, glm::mat4 worldMat, glm::vec3 numPos, Material plasticMat, int as) :
cubeVao(vao), groupMatrix(worldMat), position(glm::vec3(0.0f, 0.0f, 0.0f)),
numPosition(numPos), plastic(plasticMat), shaderProgram(shaderProg), renderAs(as) {}

bool Letters::Scoreboard(int number,bool isClock,bool isBlue) {
    // Load plastic texture
	plastic.loadToShader();
	plastic.bindTexture();

    // Get necessary uniform locations
	GLint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

    // Set some necessary matrices
	glm::mat4 worldMatrix;
	glm::mat4 partScale;
	glm::mat4 partTranslate;
	glm::mat4 partRo;
	glm::mat4 partMat;
	glm::mat4 boardTranslate;
	glm::mat4 boardRotate;
	glm::mat4 boardScale;
	glm::mat4 BoardGroupMatrix;

	float scale = 1.0f;
	//if score .... current code scale
	//else if clock.... different scale
	glm::vec3 colours;
	if (isClock) {
		colours = black;
	}
	else if (isBlue) {
		colours = blue;
	}
	else colours = red;

    // Bind VAO
	glBindVertexArray(cubeVao);

	boardTranslate = glm::translate(glm::mat4(1.0f),position);
	boardRotate = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, .0f, 1.0f));
	boardScale = glm::scale(glm::mat4(1.0f), glm::vec3(.15f, .15f, .15f) * scale);

	glm::mat4 boardParenty = boardTranslate * boardScale * boardRotate;
	boardParent = boardParenty;
	BoardGroupMatrix = groupMatrix * boardParent;

	// SEVEN SEGMENT DISPLAY
	//0	-> c												
	// Right side bottom								// x'=y    y'=z   z'=x
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(6.50f, 6.250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, -.0f, .0f));	// x'=y    y'=z   z'=x
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
    partMat = partTranslate * partScale * partRo;
	worldMatrix = BoardGroupMatrix * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colours));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

    // Create Letter objects representing the right and left numbers of the scoreboard
	Letters right(cubeVao, shaderProgram, BoardGroupMatrix, numPosition = glm::vec3(.2f, -0.05f, 0.05f), plastic,renderAs);
	Letters left(cubeVao, shaderProgram, BoardGroupMatrix, numPosition = glm::vec3(-.1f, -0.05f, 0.05f), plastic, renderAs);

    // Unbind VAO
	glBindVertexArray(0);
	
	int lefty = number / 10;
	int righty = number % 10;
	// int puase = glfwGetTime();

    // Draw the required number to the scoreboard
	right.DrawNumber(righty);
	left.DrawNumber(lefty);

	return true;
}

bool Letters::DrawNumber(int number) {
    // Get necessary uniform locations
	GLint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

    // Set some necessary variables
	glm::mat4 worldMatrix;
	glm::mat4 letterTranslate;
	glm::mat4 letterRotate;
	glm::mat4 letterScale;
	glm::mat4 LetterGroupMatrix;
	glm::mat4 partScale;
	glm::mat4 partTranslate;
	glm::mat4 partRo;
	glm::mat4 partMat;
	float scale = 3.0f;	
	std::vector<glm::vec3> colours;

    // Decide the colors of the lines needed to make a number shape on the scoreboard, as if on a
    // seven segment display
	switch (number) {
        case(0):
            colours = zero;
            break;
        case(1):
            colours = one;
            break;
        case(2):
            colours = two;
            break;
        case(3):
            colours = three;
            break;
        case(4):
            colours = four;
            break;
        case(5):
            colours = five;
            break;
        case(6):
            colours = six;
            break;
        case(7):
            colours = seven;
            break;
        case(8):
            colours = eight;
            break;
        case(9):
            colours = nine;
            break;
        default:
            break;
	}

    // Bind the VAO
	glBindVertexArray(cubeVao);

	letterTranslate = glm::translate(glm::mat4(1.0f), numPosition);
	letterRotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(.0f, .0f, 1.0f));
	letterRotate *= glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, .0f, .0f));
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.15f, .15f, .15f)*scale);

	glm::mat4 letterParent =  letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix = groupMatrix * letterParent;

	// SEVEN SEGMENT DISPLAY
	//0	-> c												
	// Right side bottom								// x'=y    y'=z   z'=x
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(3.50f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));	// x'=y    y'=z   z'=x
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, .0f, 1.0f));
    partMat = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colours[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	
	//1 -> b
	// Right side top
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(3.5f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.350f, .0f, .0f));	// x'=y    y'=z   z'=x
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, .0f, 1.0f));
    partMat = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colours[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	
	//2 -> e 
	// Left side bottom							// x'=y    y'=z   z'=x
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(3.50f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, -.30f));	// x'=y    y'=z   z'=x
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, .0f, .0f));
    partMat = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colours[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	//3 -> f
	// Left side top
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(3.5f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.350f, .0f, -.30f));	// x'=y    y'=z   z'=x
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, .0f, 1.0f));
    partMat = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colours[3]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// The letter parent reuses translation
	letterRotate = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(-1.0f, .0f, .0f));
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.17f, .15f, .15f) * scale);
	
	letterParent = letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix = this->groupMatrix * letterParent;

	//4 -> d
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(3.5f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.13f, -.0f, -.23f));	// x'=x    y'=z   z'=y
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, .0f, 1.0f));
    partMat = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colours[4]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	
	//5 -> a
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(3.5f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.13f, -.0f, .55f));	// x'=x    y'=z   z'=y
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, .0f, 1.0f));
    partMat = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colours[5]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	// Rescale the middle bar. Use same translate and rotate
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.075f, .15f, .15f) * scale);
	letterParent = letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix = this->groupMatrix * letterParent;

	//6 -> g 
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(3.5f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, -.0f, .2f));	// x'=x    y'=z   z'=y
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(.0f, .0f, 1.0f));
    partMat = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMat;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colours[6]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

    // Unbind VAO
	glBindVertexArray(0);

	return true;
}
