#pragma once
#include <iostream>
#include <sstream>
#include "Sprite2D.h"
#include "SpriteObject.h"

#define SCENE_WIDTH_IN_SCREEN	0.04731f

enum Action {
	FLY, EXPLODE
};
enum Orientation {
	L, R, U
};

class Object
{
public:
	Object();
	~Object();
	virtual void initialize(const char*, const char*);
	virtual bool nextFrame(const int);
	virtual void initSpriteIndex();

	virtual float getMapLocationX();
	virtual float getMapLocationY();
	virtual glm::vec2 getMapLocation();

	virtual void setScreenPosX(float);
	virtual void setScreenPosY(float);
	virtual float getScreenPosX();
	virtual float getScreenPosY();
	virtual glm::vec2 getScreenPos();

	bool canMove;
	GLuint vao;

protected:
	GLuint program;
	GLuint vbo;
	struct
	{
		GLint  mv_matrix;
		GLint  proj_matrix;
	} uniforms;
	glm::mat4 view, model, projection;
	int spriteIdx;

	glm::vec2 pos;
	glm::vec2 moveSpeed;

	//float mapLocationX;
	//float mapLocationY;
	glm::vec2 mapLocation;

	virtual void initSprite(void);
	virtual void genProgram(const char*, const char*);
	virtual void genVAO();
};

