#pragma once

#include "vgl.h"
#include <GLM/glm/glm.hpp>
#include <iostream>

#define COUNTER_UP_BOUND	500
#define POINT_SPRITE_NUM	10
#define NUM_STARS			200

class PointSprite {
public:
	PointSprite();
	~PointSprite();

	void pointSpriteInit(const char*, const char*, const char*, float);
	void render(glm::mat4);

	GLuint pointSpriteProgram;
	GLuint pointSpriteVS;
	GLuint pointSpriteFS;
	GLuint pointSpriteVAO;
	GLuint pointSpriteVBO;
	GLuint pointSpriteTexture;
	GLint pointSpritePorjectionLocation;
	GLint pointSpriteTimeLocation;

	//uniform id
	struct {
		GLint  mv_matrix;
		GLint  proj_matrix;
	} uniforms;

	int counter;
	int offsetY;
};

