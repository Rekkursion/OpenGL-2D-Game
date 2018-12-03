#include "MarcoAttack.h"

MarcoAttack::MarcoAttack(MAttack t = BULLET, Orientation d = R, float initX = 0.0f, float initY = 0.0f, float scenePosX = 0.0f) {
	attack = t;
	orient = d;
	pos.x = initX;
	pos.y = initY;
	mapLocationX = Object::getMapLocation(scenePosX).x;
	mapLocationY = Object::getMapLocation(scenePosX).y;
	if (t == MAttack::BULLET) {
		moveSpeed.x = 0.1f;
		moveSpeed.y = 0.0f;
		mapOffsetX = 0.004f;
	}
	else if (t == MAttack::PINEAPPLE) {
		mapOffsetX = 0.0007f;
	}
	action = Action::FLY;
	serviveTimes = 0;
	canMove = true;
}

MarcoAttack::~MarcoAttack() {}

glm::vec2 MarcoAttack::getMapLocation(float sceneLocX, glm::vec2 cur) {
	return glm::vec2((((cur.x + 1.0f) / 2.0f) * 0.04731f) + sceneLocX - 0.022f, cur.y);
}

bool MarcoAttack::nextFrame() {
	spriteIdx++;
	// Fly
	if (action == Action::FLY) {
		serviveTimes++;
		if (attack == MAttack::BULLET) {
			spriteIdx = 1;
			// Timeout
			if (serviveTimes >= 19) {
				action = Action::EXPLODE;
				initSpriteIndex();
			}
		}
		else if (attack == MAttack::PINEAPPLE) {
			// Timeout
			if (serviveTimes >= 28) {
				action = Action::EXPLODE;
				initSpriteIndex();
			}
		}
	}
	// Explosion
	else if (action == Action::EXPLODE) {
		bool isFinished = false;
		if (attack == MAttack::BULLET) {
			if (spriteIdx >= 11) {
				isFinished = true;
				initSprite();
			}
		}
		else if (attack == MAttack::PINEAPPLE) {
			if (spriteIdx >= 20) {
				isFinished = true;
				initSprite();
			}
		}
		return isFinished;
	}
	return false;
}

void MarcoAttack::initSprite() {
	// Bullet
	if (attack == MAttack::BULLET) {
		if (orient == Orientation::L) {
			for (int i = 0; i < 2; i++) {
				std::stringstream stream;
				stream << "../media/Attack/Bullet/Fly/Left/" << i << ".png";
				std::string filename = "";
				stream >> filename;
				bulletFlyLeftSprite[i].Init(filename, 0);
			}
		}
		else if (orient == Orientation::R) {
			for (int i = 0; i < 2; i++) {
				std::stringstream stream;
				stream << "../media/Attack/Bullet/Fly/Right/" << i << ".png";
				std::string filename = "";
				stream >> filename;
				bulletFlyRightSprite[i].Init(filename, 0);
			}
		}
		else if (orient == Orientation::U) {
			for (int i = 0; i < 2; i++) {
				std::stringstream stream;
				stream << "../media/Attack/Bullet/Fly/Up/" << i << ".png";
				std::string filename = "";
				stream >> filename;
				bulletFlyUpSprite[i].Init(filename, 0);
			}
		}
		// Explosion
		for (int i = 0; i < 11; i++) {
			std::stringstream stream;
			stream << "../media/Attack/Bullet/Explosion/" << i << ".png";
			std::string filename = "";
			stream >> filename;
			bulletExplosionSprite[i].Init(filename, 0);
		}
	}
	// Pineapple
	else if (attack == MAttack::PINEAPPLE) {
		if (orient == Orientation::L) {
			for (int i = 0; i < 28; i++) {
				std::stringstream stream;
				stream << "../media/Attack/Pineapple/Fly/Left/" << i << ".png";
				std::string filename = "";
				stream >> filename;
				pineappleFlyLeftSprite[i].Init(filename, 0);
			}
		}
		else if (orient == Orientation::R) {
			for (int i = 0; i < 28; i++) {
				std::stringstream stream;
				stream << "../media/Attack/Pineapple/Fly/Right/" << i << ".png";
				std::string filename = "";
				stream >> filename;
				pineappleFlyRightSprite[i].Init(filename, 0);
			}
		}
		// Explosion
		for (int i = 0; i < 20; i++) {
			std::stringstream stream;
			stream << "../media/Attack/Pineapple/Explosion/" << i << ".png";
			std::string filename = "";
			stream >> filename;
			pineappleExplosionSprite[i].Init(filename, 0);
		}
	}
}

// Bullet
// Left
void MarcoAttack::bulletFlyLeft(float scenePosX) {
	mapLocationX -= mapOffsetX;
	setScreenPosX(scenePosX);
	//pos.x -= moveSpeed.x;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	bulletFlyLeftSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	bulletFlyLeftSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Right
void MarcoAttack::bulletFlyRight(float scenePosX) {
	mapLocationX += mapOffsetX;
	setScreenPosX(scenePosX);
	//pos.x += moveSpeed.x;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	bulletFlyRightSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	bulletFlyRightSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Up
void MarcoAttack::bulletFlyUp(float scenePosX) {
	pos.y += moveSpeed.y;
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	bulletFlyUpSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	bulletFlyUpSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Explosion
void MarcoAttack::bulletExplode(float scenePosX) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	bulletExplosionSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	bulletExplosionSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}

// Pineapple
// Left
void MarcoAttack::pineappleFlyLeft(float scenePosX) {
	// Move curve
	// High wave start
	if (spriteIdx <= 7) {
		moveSpeed.x = 0.0300f;
		moveSpeed.y = pineappleCurveHighUp[spriteIdx];
	}
	// High wave down
	else if (spriteIdx <= 19) {
		moveSpeed.x = 0.0300f;
		moveSpeed.y = -pineappleCurveHighDown[spriteIdx - 8];
	}
	// Wave trough
	else if (spriteIdx == 20) {
		moveSpeed.x = 0.0f;
		moveSpeed.y = 0.0f;
	}
	// Low wave start
	else if (spriteIdx <= 23) {
		moveSpeed.x = 0.0400f;
		moveSpeed.y = pineappleCurveLowUp[spriteIdx - 21];
	}
	// Low wave down
	else if (spriteIdx <= 26) {
		moveSpeed.x = 0.0400f;
		moveSpeed.y = -pineappleCurveLowDown[spriteIdx - 24];
	}
	else if (spriteIdx == 27) {
		moveSpeed.x = 0.0f;
		moveSpeed.y = 0.0f;
	}
	// Change position
	mapLocationX -= mapOffsetX;
	setScreenPosX(scenePosX);
	//pos.x -= moveSpeed.x;
	pos.y += moveSpeed.y;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	pineappleFlyLeftSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	pineappleFlyLeftSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Right
void MarcoAttack::pineappleFlyRight(float scenePosX) {
	// Move curve
	// High wave start
	if (spriteIdx <= 7) {
		moveSpeed.x = 0.0300f;
		moveSpeed.y = pineappleCurveHighUp[spriteIdx];
	}
	// High wave down
	else if (spriteIdx <= 19) {
		moveSpeed.x = 0.0300f;
		moveSpeed.y = -pineappleCurveHighDown[spriteIdx - 8];
	}
	// Wave trough
	else if (spriteIdx == 20) {
		moveSpeed.x = 0.0f;
		moveSpeed.y = 0.0f;
	}
	// Low wave start
	else if (spriteIdx <= 23) {
		moveSpeed.x = 0.0400f;
		moveSpeed.y = pineappleCurveLowUp[spriteIdx - 21];
	}
	// Low wave down
	else if (spriteIdx <= 26) {
		moveSpeed.x = 0.0400f;
		moveSpeed.y = -pineappleCurveLowDown[spriteIdx - 24];
	}
	else if (spriteIdx == 27) {
		moveSpeed.x = 0.0f;
		moveSpeed.y = 0.0f;
	}
	// Change position
	mapLocationX += mapOffsetX;
	setScreenPosX(scenePosX);
	//pos.x += moveSpeed.x;
	pos.y += moveSpeed.y;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	pineappleFlyRightSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	pineappleFlyRightSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Explosion
void MarcoAttack::pineappleExplode(float scenePosX) {
	setScreenPosX(scenePosX);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	pineappleExplosionSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	pineappleExplosionSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}