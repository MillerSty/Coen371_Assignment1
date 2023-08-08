#include "Letters.h"

#define white glm::vec3(1,1,1)
#define black glm::vec3(0,0,0)
#define blue glm::vec3(0,0,1)
#define red glm::vec3(1,0,0)
//ifdef clock white=red
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



Letters::Letters(int cubeVao,glm::mat4 worldMatrix) {
	this->cubeVao = cubeVao;
	this->groupMatrix = worldMatrix;
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);

	//colours should be set before while loops if we want 
	//random
//colour[0] = glm::vec3(1.0f, 0.0f, 0.0f);
//colour[1] = glm::vec3(0.0f, 1.0f, 0.0f);
//colour[2] = glm::vec3(0.0f, 0.0f, 1.0f);
}
Letters::Letters(int cubeVao, int shaderProgram, glm::mat4 worldMatrix, glm::vec3 numPosition, Material plastic,int renderAs) {
	this->cubeVao = cubeVao;
	this->groupMatrix = worldMatrix;
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->numPosition = numPosition;
	this->plastic = plastic;
	this->shaderProgram = shaderProgram;
	this->renderAs = renderAs;
}


bool Letters::Scoreboard(int number,bool isClock,bool isBlue) {
	plastic.loadToShader();
	plastic.bindTexture();
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	glm::mat4 worldMatrix;

	glm::mat4 partScale;
	glm::mat4 partTranslate;
	glm::mat4 partRo;
	glm::mat4 partMatrix;

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

	glBindVertexArray(cubeVao);
	boardTranslate = glm::translate(glm::mat4(1.0f),position);
	boardRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	//boardRotate *= glm::rotate(glm::mat4(1.0f), glm::radians((float)90), glm::vec3(1.0f, .0f, .0f));
	boardScale = glm::scale(glm::mat4(1.0f), glm::vec3(.15f, .15f, .15f) * scale);

	glm::mat4 boardParenty = boardTranslate * boardScale * boardRotate;
	this->boardParent = boardParenty;
	BoardGroupMatrix = this->groupMatrix * this->boardParent;

	//seven segment jawneroonie
	//0	-> c												
	//right side bot								// x'=y    y'=z   z'=x
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(6.50f, 6.250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, -.0f, .0f));	// x'=y    y'=z   z'=x
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = BoardGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colours));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	Letters right(this->cubeVao, this->shaderProgram, BoardGroupMatrix, numPosition = glm::vec3(.2f, -0.05f, 0.05f), this->plastic,this->renderAs);
	Letters left(this->cubeVao, this->shaderProgram, BoardGroupMatrix, numPosition = glm::vec3(-.1f, -0.05f, 0.05f), this->plastic, this->renderAs);
	
	glBindVertexArray(0);
	
	//int check, check1, check2;
	int lefty = number / 10;
	int righty = number % 10;
	int puase = glfwGetTime();

	right.DrawNumber(righty);
	left.DrawNumber(lefty);

	return true;
}
bool Letters::DrawNumber(int number) {
	//seven segment
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	glm::mat4 worldMatrix;
	glm::mat4 letterTranslate;
	glm::mat4 letterRotate;
	glm::mat4 letterScale;
	glm::mat4 LetterGroupMatrix;
	glm::mat4 partScale;
	glm::mat4 partTranslate;
	glm::mat4 partRo;
	glm::mat4 partMatrix;
	float scale = 3.0f;	
	//glm::vec3 colours[7]= zero ;
	std::vector<glm::vec3> colours;
	//colours( zero);
	switch (number) {
	case(0):colours= zero; break;
	case(1):colours = one;    break;
	case(2):colours = two;	 break;
	case(3):colours = three;	 break;
	case(4):colours = four;	 break;
	case(5):colours = five;	 break;
	case(6):colours = six;	 break;
	case(7):colours = seven;	 break;
	case(8):colours = eight;	 break;
	case(9):colours = nine;	 break;
	default: break;
	
	}
	//glm::vec3 colours[7]= eight ;
	//heirarchy is fucked up

	//what if we define 7 vectors/ 8 arrays and select with a switch statment?
	//these inherit from a board 
	glBindVertexArray(cubeVao);
	letterTranslate = glm::translate(glm::mat4(1.0f), numPosition);
	letterRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)90), glm::vec3(.0f, .0f, 1.0f));
	letterRotate *= glm::rotate(glm::mat4(1.0f), glm::radians((float)90), glm::vec3(1.0f, .0f, .0f));
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.15f, .15f, .15f)*scale);

	glm::mat4 letterParent =  letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix = this->groupMatrix * letterParent;

	//seven segment jawneroonie
	//0	-> c												
	//right side bot								// x'=y    y'=z   z'=x
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(3.50f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));	// x'=y    y'=z   z'=x
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colours[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	
	//1 -> b
	//right side top
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(3.5f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.350f, .0f, .0f));	// x'=y    y'=z   z'=x
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colours[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	
	//2 -> e 
	//left side bot								// x'=y    y'=z   z'=x
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(3.50f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, -.30f));	// x'=y    y'=z   z'=x
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(1.0f, .0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colours[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	//3 -> f
	//left side top
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(3.5f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.350f, .0f, -.30f));	// x'=y    y'=z   z'=x
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colours[3]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);


	
	//letter parent reusing translation
	letterRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)90), glm::vec3(-1.0f, .0f, .0f));
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.17f, .15f, .15f) * scale);
	
	letterParent = letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix = this->groupMatrix * letterParent;

	//4 -> d
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(3.5f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.13f, -.0f, -.23f));	// x'=x    y'=z   z'=y
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colours[4]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);
	
	//5 -> a
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(3.5f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.13f, -.0f, .55f));	// x'=x    y'=z   z'=y
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colours[5]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);

	//middle bar rescale use same translate and rotate
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.075f, .15f, .15f) * scale);
	letterParent = letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix = this->groupMatrix * letterParent;

	//6 -> g 
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(3.5f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, -.0f, .2f));	// x'=x    y'=z   z'=y
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colours[6]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(renderAs, 0, 36);



	glBindVertexArray(0);

	return true;

}


// Note for all Draw functions they follow the same format, declare matrice locations and mat4 variables. 
// then scale, rotate, translate x3 so we get triple ply effect on letters
bool Letters::DrawJ() {
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	glm::mat4 worldMatrix;
	glm::mat4 letterTranslate;
	glm::mat4 letterRotate;
	glm::mat4 letterScale;
	glm::mat4 LetterGroupMatrix;
	glm::mat4 partScale;
	glm::mat4 partTranslate;
	glm::mat4 partRo;
	glm::mat4 partMatrix;

	glBindVertexArray(cubeVao);

	// J has little notch, long bottom, long Straight,little top 
	letterTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.2f, .0f, .0f));
	letterRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.15f, .15f, .15f));

	glm::mat4 letterParent = this->partParent * letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix =   this->groupMatrix * letterParent;

	//long jawn triple Vert
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(7.0f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	//long triple jawn Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//long jawn triple Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, -.055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

		//long jawn triple Hor
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f, 7.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, .0f, -.35f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	////***
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f, 7.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, .055f, -.35f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	
	//***
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f, 7.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, -.055f, -.35f)); 
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(1.0f, .0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
			//Short Bot Hook

	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(2.5f, .250f, 1.f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.15f, .0f, -.65f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	//triple jawn
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.15f, .055f, -.65f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.15f, -.055f, -.65f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	//top hook
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f, 2.5f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.4f, .0f, -.07f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, 1.0f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//top hook
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.4f, .055f, -.07f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//top hook
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.4f, -.0550f, -.07f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	return true;
}
bool Letters::DrawI() {
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	glm::mat4 worldMatrix;
	glm::mat4 letterTranslate;
	glm::mat4 letterRotate;
	glm::mat4 letterScale;
	glm::mat4 LetterGroupMatrix;
	glm::mat4 partScale;
	glm::mat4 partTranslate;
	glm::mat4 partRo;
	glm::mat4 partMatrix;

	glBindVertexArray(cubeVao);

	letterTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.2f, .0f, .0f));
	letterRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.15f, .15f, .15f));

	glm::mat4 letterParent = this->partParent * letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix = this->groupMatrix * letterParent;

	//long jawn triple Vert
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(7.0f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	//long triple jawn Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//long jawn triple Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, -.055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	return true;
}
bool Letters::DrawM() {
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	glm::mat4 worldMatrix;
	glm::mat4 letterTranslate;
	glm::mat4 letterRotate;
	glm::mat4 letterScale;
	glm::mat4 LetterGroupMatrix;
	glm::mat4 partScale;
	glm::mat4 partTranslate;
	glm::mat4 partRo;
	glm::mat4 partMatrix;

	glBindVertexArray(cubeVao);


	letterTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.2f, .0f, .0f));
	letterRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.15f, .15f, .15f));

	glm::mat4 letterParent = this->partParent * letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix = this->groupMatrix * letterParent;

	//long jawn triple Hor
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f, 4.5f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.15f, .0f, .0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//tripled jawn
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.15f, -.055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//tripled
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.15f, .055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

		//long jawn triple Vert
	//double foor loop for doing the three parallel lines of M
	for (float i = -.2f; i <= .2f; i += .2f) {
		partScale = glm::scale(glm::mat4(1.0f), glm::vec3(4.5f, .250f, 1.0f));
		partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.05f, .0f, i));
		partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
		partMatrix = partTranslate * partScale * partRo;
		worldMatrix = LetterGroupMatrix * partMatrix;

		glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
		
		for (int j = 0; j < 2; j++) {
			if (j== 0) {
				partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.05f, -.055f, i));
				partMatrix = partTranslate * partScale * partRo;
				worldMatrix = LetterGroupMatrix * partMatrix;
				glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
				glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
				glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
			}			
			else if (j==1) {
				partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.05f, .055, i));
				partMatrix = partTranslate * partScale * partRo;
				worldMatrix = LetterGroupMatrix * partMatrix;
				glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
				glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
				glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
			}
		}
	}

	return true;
}
bool Letters::DrawL() {
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	glm::mat4 worldMatrix;
	glm::mat4 letterTranslate;
	glm::mat4 letterRotate;
	glm::mat4 letterScale;
	glm::mat4 LetterGroupMatrix;
	glm::mat4 partScale;
	glm::mat4 partTranslate;
	glm::mat4 partRo;
	glm::mat4 partMatrix;

	glBindVertexArray(cubeVao);


	letterTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.2f, .0f, .0f));
	letterRotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	letterScale = glm::scale(glm::mat4(1.0f), glm::vec3(.15f, .15f, .15f));

	glm::mat4 letterParent = this->partParent * letterTranslate * letterScale * letterRotate;
	LetterGroupMatrix = this->groupMatrix * letterParent;

	//long jawn triple Vert
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(7.0f, .250f, 1.0f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .0f, .0f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//jawn Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, .055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	// tripled Vert
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, -.055f, .0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	//long jawn triple Hor
	partScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, .250f, 4.5f));
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, .0f, .25f));
	partRo = glm::rotate(glm::mat4(1.0f), glm::radians((float)0), glm::vec3(.0f, .0f, 1.0f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;

	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[0]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//tripled jawn
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, .055f, .25f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[1]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);
	//tripled
	partTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-.3f, -.055f, .25f));
	partMatrix = partTranslate * partScale * partRo;
	worldMatrix = LetterGroupMatrix * partMatrix;
	glUniform3fv(colorLocation, 1, glm::value_ptr(colour[2]));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawElements(renderAs, 36, GL_UNSIGNED_INT, 0);

	return true;
}