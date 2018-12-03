#pragma warning(disable:4996)
#include "Marco.h"

Marco::Marco() {
	for (int k = 0; k < MAX_KEY_NUM; k++)
		keyboardState[k] = false;
	spriteIdx = 0;

	state = State::IDLE;
	direction = Direction::RIGHT;
	pos = glm::vec2(INIT_LOC_X, INIT_LOC_Y);
	moveSpeed.x = 0.025f;
	moveSpeed.y = 0.1f;
	mapLocationX = INIT_MAP_LOC_X;
	canMove = true;
	atCentre = false;

	blinkingCounter = 0;
}

Marco::~Marco() {}

void Marco::setKeyboard(int key, bool state) {
	keyboardState[key] = state;
}

bool Marco::getKeyboard(int key) {
	if (key < 0)
		key = 0;

	if (key >= MAX_KEY_NUM)
		key = MAX_KEY_NUM - 1;

	return keyboardState[key];
}

void Marco::reborn() {
	state = State::IDLE;
	direction = Direction::RIGHT;
	pos = glm::vec2(0.0, INIT_LOC_Y);
	blinkingCounter = 0;
}

glm::vec2 Marco::getScreenPosition() {
	return pos;
}

int Marco::getSpriteId() {
	return spriteIdx;
}

void Marco::initSprite() {
	blinkingLocation = glGetUniformLocation(program, "blinkingcounter");

	// Initial LEFT
	// Initial Left to Right
	for (int i = 0; i < 4; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Left/LtoR/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		LtoRSprite[i].Init(filename, 0);
	}
	// Initial idle left
	for (int i = 0; i < 1; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Left/Idle/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		idleLeftSprite[i].Init(filename, 0);
	}
	// Initial stop left
	for (int i = 0; i < 8; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Left/Stop/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		stopLeftSprite[i].Init(filename, 0);
	}
	// Initial walk left
	for (int i = 0; i < 13; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Left/Walk/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		walkLeftSprite[i].Init(filename, 0);
	}
	// Initial up left
	for (int i = 0; i < 5; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Left/Up/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		upLeftSprite[i].Init(filename, 0);
	}
	// Initial idle up left
	for (int i = 0; i < 1; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Left/IdleUp/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		idleUpLeftSprite[i].Init(filename, 0);
	}
	// Initial down left
	for (int i = 0; i < 5; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Left/Down/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		downLeftSprite[i].Init(filename, 0);
	}
	// Initial jump left
	for (int i = 0; i < 9; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Left/Jump/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		jumpLeftSprite[i].Init(filename, 0);
	}
	// Initial fall left
	for (int i = 0; i < 9; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Left/Fall/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		fallLeftSprite[i].Init(filename, 0);
	}
	// Initial knife left
	for (int i = 0; i < 8; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Left/Knife/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		knifeLeftSprite[i].Init(filename, 0);
	}
	// Initial gun left
	for (int i = 0; i < 12; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Left/Shoot/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		shootLeftSprite[i].Init(filename, 0);
	}
	// Initial gun up left
	for (int i = 0; i < 12; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Left/UpShoot/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		shootUpLeftSprite[i].Init(filename, 0);
	}
	// Initial grenade left
	for (int i = 0; i < 7; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Left/Grenade/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		grenadeLeftSprite[i].Init(filename, 0);
	}
	// Initial RIGHT
	// Initial Right to Left
	for (int i = 0; i < 4; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Right/RtoL/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		RtoLSprite[i].Init(filename, 0);
	}
	// Initial idle right
	for (int i = 0; i < 1; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Right/Idle/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		idleRightSprite[i].Init(filename, 0);
	}
	// Initial stop right
	for (int i = 0; i < 8; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Right/Stop/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		stopRightSprite[i].Init(filename, 0);
	}
	// Initial walk right
	for (int i = 0; i < 13; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Right/Walk/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		walkRightSprite[i].Init(filename, 0);
	}
	// Initial up right
	for (int i = 0; i < 5; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Right/Up/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		upRightSprite[i].Init(filename, 0);
	}
	// Initial idle up right
	for (int i = 0; i < 1; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Right/IdleUp/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		idleUpRightSprite[i].Init(filename, 0);
	}
	// Initial down right
	for (int i = 0; i < 5; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Right/Down/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		downRightSprite[i].Init(filename, 0);
	}
	// Initial jump right
	for (int i = 0; i < 9; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Right/Jump/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		jumpRightSprite[i].Init(filename, 0);
	}
	// Initial fall right
	for (int i = 0; i < 9; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Right/Fall/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		fallRightSprite[i].Init(filename, 0);
	}
	// Initial knife right
	for (int i = 0; i < 8; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Right/Knife/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		knifeRightSprite[i].Init(filename, 0);
	}
	// Initial gun right
	for (int i = 0; i < 12; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Right/Shoot/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		shootRightSprite[i].Init(filename, 0);
	}
	// Initial gun up right
	for (int i = 0; i < 12; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Right/UpShoot/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		shootUpRightSprite[i].Init(filename, 0);
	}
	// Initial grenade right
	for (int i = 0; i < 7; i++) {
		std::stringstream stream;
		stream << "../media/Marco/Right/Grenade/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		grenadeRightSprite[i].Init(filename, 0);
	}
}

// LEFT
// Left turn to Right
void Marco::LtoR() {
	pos.x += moveSpeed.x;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	idleLeftSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	idleLeftSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Idle Left
void Marco::idleLeft() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	idleLeftSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	idleLeftSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Stop Left
void Marco::stopLeft() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	stopLeftSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	stopLeftSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Walk Left
void Marco::walkLeft() {
	//std::cout << "pos.x " << pos.x << ", canMove: " << canMove << std::endl;
	atCentre = (fabs(pos.x - 0.0f) <= 0.0015f);

	if (atCentre)
		pos.x = 0.0f;

	if ((fabs(pos.x - 0.0f) > 0.0015f || atCentre) && pos.x > -1.0 && canMove) {
		pos.x -= moveSpeed.x;
		if (pos.x < -1.0 + (TEX_SIZE_WIDTH / 2.0f))
			pos.x = -1.0 + (TEX_SIZE_WIDTH / 2.0f);
	}

	mapLocationX -= 0.0015f;
	if (mapLocationX < 0.0f)
		mapLocationX = 0.0f;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	walkLeftSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	walkLeftSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Up Left
void Marco::upLeft() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	upLeftSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	upLeftSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Up idle Left
void Marco::idleUpLeft() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	idleUpLeftSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	idleUpLeftSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Down Left
void Marco::downLeft() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	downLeftSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	downLeftSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Jump Left
void Marco::jumpLeft() {
	if (spriteIdx < 5)
		pos.y += moveSpeed.y;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	jumpLeftSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	jumpLeftSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Fall Left
void Marco::fallLeft() {
	if (spriteIdx > 3)
		pos.y -= moveSpeed.y;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	fallLeftSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	fallLeftSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Knife Left
void Marco::knifeLeft() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	knifeLeftSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	knifeLeftSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Gun Left
void Marco::shootLeft() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	shootLeftSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	shootLeftSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Gun Up Left
void Marco::shootUpLeft() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	shootUpLeftSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	shootUpLeftSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Grenade Left
void Marco::grenadeLeft() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	grenadeLeftSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	grenadeLeftSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// RIGHT
// Right turn to Left
void Marco::RtoL() {
	pos.x -= moveSpeed.x;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	idleLeftSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	idleLeftSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Idle Right
void Marco::idleRight() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	idleRightSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	idleRightSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Stop Right
void Marco::stopRight() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	stopRightSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	stopRightSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Walk Right
void Marco::walkRight() {
	//std::cout << "pos.x " << pos.x << ", canMove " << canMove << ", atCentre " << atCentre << std::endl;
	atCentre = (fabs(pos.x - 0.0f) <= 0.015f);

	if (atCentre)
		pos.x = 0.0f;

	if ((fabs(pos.x - 0.0f) > 0.015f || atCentre) && canMove) {
		if (pos.x == 0.0f)
			pos.x += 0.006f;
		pos.x += moveSpeed.x;
		if (pos.x >= 1.0 - (TEX_SIZE_WIDTH / 2.0f))
			pos.x = 1.0 - (TEX_SIZE_WIDTH / 2.0f);
	}

	mapLocationX += 0.0015f;
	//if (mapLocationX < 0.0f)
	//	mapLocationX = 0.0f;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	walkRightSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	walkRightSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Up Right
void Marco::upRight() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	upRightSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	upRightSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Up idle Right
void Marco::idleUpRight() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	idleUpRightSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	idleUpRightSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Down Right
void Marco::downRight() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	downRightSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	downRightSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Jump Right
void Marco::jumpRight() {
	if (spriteIdx < 5)
		pos.y += moveSpeed.y;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	jumpRightSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	jumpRightSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Jump Right
void Marco::fallRight() {
	if (spriteIdx > 3)
		pos.y -= moveSpeed.y;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	fallRightSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	fallRightSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Knife Left
void Marco::knifeRight() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	knifeRightSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	knifeRightSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Gun Right
void Marco::shootRight() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	shootRightSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	shootRightSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Gun Up Right
void Marco::shootUpRight() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	shootUpRightSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	shootUpRightSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Grenade Right
void Marco::grenadeRight() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	grenadeRightSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glUniform1f(blinkingLocation, (float)blinkingCounter);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	grenadeRightSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}