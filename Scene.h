#pragma once
#include "Object.h"

#define TEX_SIZE_WIDTH			0.925f

enum ShowingPicture {
	BACKGROUND, MISSION_COMPLETE
};

class Scene: public Object
{
public:
	Scene();
	~Scene();

	void render(void);
	void moveBG(bool);
	void moveBG2Head();
	float getPosX();
	void enterEndingPhase();

	bool atHead;
	bool atTail;
	ShowingPicture showingPic;

private:
	void initSprite(void);
	Sprite2D background[1];
	Sprite2D missionComplete;
};

