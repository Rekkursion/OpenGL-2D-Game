#pragma warning(disable:4996)
#include "Object.h"
#include "ShaderFunction.h"

Object::Object()
{
	spriteIdx = 0;
	pos = moveSpeed = glm::vec2(0.0);
	canMove = true;
}

Object::~Object()
{
}

void Object::initialize(const char* vertexShader, const char* fragmentShader)
{
	genProgram(vertexShader, fragmentShader);
	genVAO();
	initSprite();

	view = model = projection = glm::mat4(1.0);
}

void Object::genProgram(const char* vertexShader, const char* fragmentShader) {
	//Initialize shaders
	///////////////////////////	
	program = glCreateProgram();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	char** vsSource = loadShaderSource(vertexShader);
	char** fsSource = loadShaderSource(fragmentShader);
	glShaderSource(vs, 1, vsSource, NULL);
	glShaderSource(fs, 1, fsSource, NULL);
	freeShaderSource(vsSource);
	freeShaderSource(fsSource);
	glCompileShader(vs);
	glCompileShader(fs);
	shaderLog(vs);
	shaderLog(fs);

	//Attach Shader to program
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	//Cache uniform variable id
	uniforms.proj_matrix = glGetUniformLocation(program, "um4p");
	uniforms.mv_matrix = glGetUniformLocation(program, "um4mv");

	glUseProgram(program);
}

void Object::genVAO() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(pos) + sizeof(frame), NULL, GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), NULL, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribDivisor(0, 1);

	/*glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(frame), frame);
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 1, GL_INT, 0, (GLvoid*)(sizeof(pos)));
	glVertexAttribDivisor(1, 1);*/

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

bool Object::nextFrame(const int MAX_FRAME) {
	bool isFinished = false;
	spriteIdx++;

	// finish
	if (spriteIdx >= MAX_FRAME) {
		isFinished = true;
		spriteIdx = 0;
	}

	return isFinished;
}

void Object::initSpriteIndex()
{
	spriteIdx = 0;
}

void Object::initSprite()
{
}