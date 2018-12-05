#include "EnemyAttack.h"


float EnemyAttack::BULLET_ATTACK_GAP_X = 0.002f;
float EnemyAttack::BULLET_ATTACK_GAP_Y = 0.3f;
float EnemyAttack::KNIFE_ATTACK_GAP_X_BIGGER = 0.013f - 0.002f;
float EnemyAttack::KNIFE_ATTACK_GAP_X_SMALLER = 0.013f;
float EnemyAttack::KNIFE_ATTACK_GAP_Y = 0.25f;

// Constructor
EnemyAttack::EnemyAttack(): EnemyAttack(EAttack::BLINK, Orientation::L, 0.0f, 0.0f) {}
EnemyAttack::EnemyAttack(EAttack t = BLINK, Orientation d = L, float initX = 0.0f, float initY = 0.0f) {
	attack = t;
	orient = d;
	if (t == EAttack::BLINK) {
		moveSpeed.x = 0.002f;
		if (d == Orientation::L) {

			mapLocation.x = initX - 0.0030;
			mapLocation.y = initY + 0.2600;
		}
		else if (d == Orientation::R) {
			mapLocation.x = initX + 0.0030;
			mapLocation.y = initY + 0.2600;
		}
	}
	action = FLY;
	setScreenPosX(mapLocation.x);
	setScreenPosY(mapLocation.y);
	serviveTimes = 0;
}
// Distructor
EnemyAttack::~EnemyAttack() {}

bool EnemyAttack::nextFrame() {
	spriteIdx++;
	// Fly
	if (action == Action::FLY) {
		serviveTimes++;
		if (attack == EAttack::BLINK) {
			spriteIdx = spriteIdx % 2;
			// Timeout
			if (serviveTimes >= 100) {
				action = Action::EXPLODE;
				initSpriteIndex();
			}
		}
	}
	// Explosion
	else if (action == Action::EXPLODE) {
		bool isFinished = false;
		if (attack == EAttack::BLINK) {
			if (spriteIdx >= 6) {
				isFinished = true;
				initSprite();
			}
		}
		return isFinished;
	}
	return false;
}

void EnemyAttack::initSprite() {
	// Blink
	if (attack == EAttack::BLINK) {
		for (int i = 0; i < 2; i++) {
			std::stringstream stream;
			stream << "../media/EnemyAttack/Bullet/Fly/" << i << ".png";
			std::string filename = "";
			stream >> filename;
			blinkFlySprite[i].Init(filename, 0);
		}
		// Explosion
		for (int i = 0; i < 6; i++) {
			std::stringstream stream;
			stream << "../media/EnemyAttack/Bullet/Explosion/" << i << ".png";
			std::string filename = "";
			stream >> filename;
			blinkExplosionSprite[i].Init(filename, 0);
		}
	}
}

//Blink
// Left
void EnemyAttack::blinkFlyLeft(float scenePosX) {
	mapLocation.x -= moveSpeed.x;
	setScreenPosX(scenePosX);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	blinkFlySprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	blinkFlySprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Right
void EnemyAttack::blinkFlyRight(float scenePosX) {
	mapLocation.x += moveSpeed.x;
	setScreenPosX(scenePosX);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	blinkFlySprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	blinkFlySprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Explosion
void EnemyAttack::bulletExplode(float scenePosX) {
	setScreenPosX(scenePosX);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	blinkExplosionSprite[spriteIdx].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	blinkExplosionSprite[spriteIdx].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}