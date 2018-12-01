#include "Scene.h"



Scene::Scene() {
	pos = glm::vec2(0.0f);
	moveSpeed = glm::vec2(0.0015f);

	atHead = true;
	atTail = false;
	canMove = false;
}

Scene::~Scene()
{
}

void Scene::initSprite(void) {
	std::string filename = "../media/Map.png";
	background[0].Init(filename, 0);
}

void Scene::render(void) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	background[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	background[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}

void Scene::moveBG(bool isPositive) {
	if (isPositive && canMove) {
		pos.x += moveSpeed.x;
		if (pos.x > TEX_SIZE_WIDTH)
			pos.x = TEX_SIZE_WIDTH;
	}

	if (!isPositive && pos.x > 0.0 && canMove) {
		pos.x -= moveSpeed.x;
		if (pos.x < 0.0)
			pos.x = 0.0;
	}

	atHead = (pos.x == 0.0);
	atTail = (fabs(pos.x - TEX_SIZE_WIDTH) <= 0.01);

	return;
}