#pragma once
#include "Object.h"

#define TEX_SIZE_WIDTH			0.925f

class Scene: public Object
{
public:
	Scene();
	~Scene();

	void render(void);
	void moveBG(bool);
	float getPosX();

	bool atHead;
	bool atTail;

private:
	void initSprite(void);
	Sprite2D background[1];
};

