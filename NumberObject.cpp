#include "NumberObject.h"



NumberObject::NumberObject(): NumberObject(0, glm::vec2(0.0)) {}

NumberObject::NumberObject(int num, glm::vec2 loc) {
	pos = loc;
	setNumber(num);
}

NumberObject::~NumberObject() {}

void NumberObject::setNumber(int num) {
	number = num;
}

int NumberObject::getNumber() {
	return number;
}

void NumberObject::setLocation(glm::vec2 loc) {
	pos = loc;
}

void NumberObject::initSprite() {

	for (int k = 0; k < 10; k++) {
		std::stringstream stream;
		stream << "../media/Numbers/" << k << ".png";
		std::string filename = "";
		stream >> filename;

		numberSprite[k].Init(filename, k);
	}
}

void NumberObject::render() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	numberSprite[number].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	numberSprite[number].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}