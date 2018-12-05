#include "Enemy.h"

int Enemy::enemyNum = 0;

// Constructor
Enemy::Enemy() {
	type = Arms::DALLY;
	side = Side::RSIDE;
	pos = glm::vec2(0.0, -0.8);
	isDieing = false;
}

Enemy::Enemy(Arms t = DALLY, Side d = RSIDE, float InitX = 0.0, float InitY = -0.8, float sceneLocX = 0.0)
{
	spriteIdx = 0;

	type = t;
	side = d;

	pos = glm::vec2(InitX, InitY);
	//moveSpeed = glm::vec2(0.055f, 0.1f);
	mapLocation.x = mapLocationList[enemyNum];
	mapLocation.y = InitY;
	//setScreenPosX(sceneLocX);
	//setScreenPosY(mapLocation.y);

	enemyNum++;
	if (enemyNum >= (sizeof(mapLocationList) / sizeof(float)))
		enemyNum = (sizeof(mapLocationList) / sizeof(float)) - 1;

	moveSpeed = glm::vec2(0.0005f, 0.0f);
	step = -1;

	isDieing = false;
}
// Distructor
Enemy::~Enemy()
{
}

void Enemy::setScreenPosX(float mapPosX) {
	Object::setScreenPosX(mapPosX);

	float dis_has_dir = mapLocation.x - mapPosX;
	if (dis_has_dir < 0)
		side = Side::LSIDE;
	else
		side = Side::RSIDE;
}

float Enemy::getMapLocationX() {
	return mapLocation.x;
}

void Enemy::killed(Death death_type) {
	isDieing = true;
	deathType = death_type;
	initSpriteIndex();
}

int Enemy::getSpriteId() {
	return spriteIdx;
}

// Initial Sprite
void Enemy::initSprite() {
	// LEFT
	// The categories of arms
	// Dilly-Dally
	if (type == Arms::DALLY) {
		for (int i = 0; i < 28; i++) {
			std::stringstream stream;
			stream << "../media/Enemy/Dally/Left/" << i << ".png";
			std::string filename = "";
			stream >> filename;
			dallyLeftSprite[i].Init(filename, 0);
		}
	}
	// Gun
	else if (type == Arms::PISTOL) {
		for (int i = 0; i < 12; i++) {
			std::stringstream stream;
			stream << "../media/Enemy/Gun/Left/" << i << ".png";
			std::string filename = "";
			stream >> filename;
			pistolLeftSprite[i].Init(filename, 0);
		}
	}
	// Knife
	else if (type == Arms::SABER) {
		for (int i = 0; i < 23; i++) {
			std::stringstream stream;
			stream << "../media/Enemy/Knife/Left/" << i << ".png";
			std::string filename = "";
			stream >> filename;
			saberLeftSprite[i].Init(filename, 0);
		}
	}
	// Rest
	for (int i = 0; i < 7; i++) {
		std::stringstream stream;
		stream << "../media/Enemy/Rest/Left/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		restLeftSprite[i].Init(filename, 0);
	}
	// Walk
	for (int i = 0; i < 12; i++) {
		std::stringstream stream;
		stream << "../media/Enemy/Walk/Left/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		walkLeftSprite[i].Init(filename, 0);
	}
	// Death reason
	// Grenade
	for (int i = 0; i < 10; i++) {
		std::stringstream stream;
		stream << "../media/Enemy/Death/Left/Grenade/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		deathGrenadeLeftSprite[i].Init(filename, 0);
	}
	// Gun
	for (int i = 0; i < 15; i++) {
		std::stringstream stream;
		stream << "../media/Enemy/Death/Left/Gun/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		deathGunLeftSprite[i].Init(filename, 0);
	}
	// Knife
	for (int i = 0; i < 16; i++) {
		std::stringstream stream;
		stream << "../media/Enemy/Death/Left/Knife/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		deathKnifeLeftSprite[i].Init(filename, 0);
	}

	// RIGHT
	// The categories of arms
	// Dilly-Dally
	if (type == Arms::DALLY) {
		for (int i = 0; i < 28; i++) {
			std::stringstream stream;
			stream << "../media/Enemy/Dally/Right/" << i << ".png";
			std::string filename = "";
			stream >> filename;
			dallyRightSprite[i].Init(filename, 0);
		}
	}
	// Gun
	else if (type == Arms::PISTOL) {
		for (int i = 0; i < 12; i++) {
			std::stringstream stream;
			stream << "../media/Enemy/Gun/Right/" << i << ".png";
			std::string filename = "";
			stream >> filename;
			pistolRightSprite[i].Init(filename, 0);
		}
	}
	// Knife
	else if (type == Arms::SABER) {
		for (int i = 0; i < 23; i++) {
			std::stringstream stream;
			stream << "../media/Enemy/Knife/Right/" << i << ".png";
			std::string filename = "";
			stream >> filename;
			saberRightSprite[i].Init(filename, 0);
		}
	}
	// Rest
	for (int i = 0; i < 7; i++) {
		std::stringstream stream;
		stream << "../media/Enemy/Rest/Right/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		restRightSprite[i].Init(filename, 0);
	}
	// Walk
	for (int i = 0; i < 12; i++) {
		std::stringstream stream;
		stream << "../media/Enemy/Walk/Right/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		walkRightSprite[i].Init(filename, 0);
	}
	// Death reason
	// Grenade
	for (int i = 0; i < 10; i++) {
		std::stringstream stream;
		stream << "../media/Enemy/Death/Right/Grenade/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		deathGrenadeRightSprite[i].Init(filename, 0);
	}
	// Gun
	for (int i = 0; i < 15; i++) {
		std::stringstream stream;
		stream << "../media/Enemy/Death/Right/Gun/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		deathGunRightSprite[i].Init(filename, 0);
	}
	// Knife
	for (int i = 0; i < 16; i++) {
		std::stringstream stream;
		stream << "../media/Enemy/Death/Right/Knife/" << i << ".png";
		std::string filename = "";
		stream >> filename;
		deathKnifeRightSprite[i].Init(filename, 0);
	}
}

// LEFT
// Dilly-Dally function
void Enemy::dallyLeft() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	dallyLeftSprite[spriteIdx / 2].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	dallyLeftSprite[spriteIdx / 2].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Gun function
void Enemy::pistolLeft() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	pistolLeftSprite[spriteIdx / 2].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	pistolLeftSprite[spriteIdx / 2].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Knife function
void Enemy::saberLeft() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	saberLeftSprite[spriteIdx / 2].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	saberLeftSprite[spriteIdx / 2].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Rest function
void Enemy::restLeft() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	restLeftSprite[spriteIdx / 2].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	restLeftSprite[spriteIdx / 2].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Walk function
void Enemy::walkLeft(float sceneLocX) {
	mapLocation.x += moveSpeed.x;
	setScreenPosX(sceneLocX);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	walkLeftSprite[spriteIdx / 2].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	walkLeftSprite[spriteIdx / 2].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Death function
void Enemy::deathLeft() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	if (deathType == Death::BOMB) {
		deathGrenadeLeftSprite[spriteIdx / 2].Enable();
		glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
		glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		deathGrenadeLeftSprite[spriteIdx / 2].Disable();
	}
	else if (deathType == Death::SHOT) {
		deathGunLeftSprite[spriteIdx / 2].Enable();
		glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
		glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		deathGunLeftSprite[spriteIdx / 2].Disable();
	}
	else if (deathType == Death::STAB) {
		deathKnifeLeftSprite[spriteIdx / 2].Enable();
		glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
		glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		deathKnifeLeftSprite[spriteIdx / 2].Disable();
	}

	glBindVertexArray(0);
	glUseProgram(0);
}

// RIGHT
// Dilly-Dally function
void Enemy::dallyRight() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	dallyRightSprite[spriteIdx / 2].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	dallyRightSprite[spriteIdx / 2].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Gun function
void Enemy::pistolRight() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	pistolRightSprite[spriteIdx / 2].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	pistolRightSprite[spriteIdx / 2].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Knife function
void Enemy::saberRight() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	saberRightSprite[spriteIdx / 2].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	saberRightSprite[spriteIdx / 2].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Rest function
void Enemy::restRight() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	restRightSprite[spriteIdx / 2].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	restRightSprite[spriteIdx / 2].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Walk function
void Enemy::walkRight(float sceneLocX) {
	mapLocation.x -= moveSpeed.x;
	setScreenPosX(sceneLocX);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	walkRightSprite[spriteIdx / 2].Enable();
	glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
	glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	walkRightSprite[spriteIdx / 2].Disable();

	glBindVertexArray(0);
	glUseProgram(0);
}
// Death function
void Enemy::deathRight() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), &pos);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(pos), sizeof(GL_INT) * objectCount, frame);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Update shaders' input variable
	glUseProgram(program);

	glBindVertexArray(vao);

	if (deathType == Death::BOMB) {
		deathGrenadeRightSprite[spriteIdx].Enable();
		glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
		glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		deathGrenadeRightSprite[spriteIdx].Disable();
	}
	else if (deathType == Death::SHOT) {
		deathGunRightSprite[spriteIdx].Enable();
		glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
		glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		deathGunRightSprite[spriteIdx].Disable();
	}
	else if (deathType == Death::STAB) {
		deathKnifeRightSprite[spriteIdx].Enable();
		glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, &(view * model)[0][0]);
		glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, &(projection)[0][0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		deathKnifeRightSprite[spriteIdx].Disable();
	}

	glBindVertexArray(0);
	glUseProgram(0);
}