#pragma once

#include "vgl.h"
#include <GLM/glm/glm.hpp>
#include <iostream>

#define COUNTER_UP_BOUND		500
#define POINT_SPRITE_NUM		50
#define INIT_STARS_NUM			500
#define STARS_NUM_INCREMENT		1000
#define MAX_STARS_NUM			10000
#define STARS_ANGLE_INCREMENT	1.0f

class PointSprite {
public:
	PointSprite();
	~PointSprite();

	void pointSpriteInit(const char*, const char*, const char*, float);
	void render(glm::mat4);
	void increaseStarsNum();
	void decreaseStarsNum();
	void increaseStarsSpeed();
	void decreaseStarsSpeed();
	void increaseStarsAngle();
	void decreaseStarsAngle();

	GLuint pointSpriteProgram;
	GLuint pointSpriteVS;
	GLuint pointSpriteFS;
	GLuint pointSpriteVAO;
	GLuint pointSpriteVBO;
	GLuint pointSpriteTexture;
	GLint pointSpritePorjectionLocation;
	GLint pointSpriteTimeLocation;
	GLint pointSpriteAngleLocation;

	//uniform id
	struct {
		GLint  mv_matrix;
		GLint  proj_matrix;
	} uniforms;

	int counter;
	int offsetY;

	int starsNum;
	int starsSpeed;
	float starsAngle;
};

