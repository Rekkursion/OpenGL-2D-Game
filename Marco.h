#pragma once
#include "Object.h"
#include <string>
#include <iostream>

#define MAX_KEY_NUM		256
#define INIT_LOC_X		(-0.5f)
#define INIT_LOC_Y		(-0.8f)
#define TEX_SIZE_WIDTH	0.17f

static std::string stateSymbols[] = {
	"Idle", "Walk", "Stop", "Jump", "Fall", "JumpForward", "Redirect", "Gun", "Grenade", "Up"
};
enum State {
	IDLE, WALK, STOP, JUMP, FALL, JUMP_FORWARD, REDIRECT, KNIFE, GUN, GRENADE, UP
};
enum Direction {
	LEFT, RIGHT
};

class Marco: public Object
{
public:
	bool atCentre;

	Marco();
	~Marco();
	void setKeyboard(int, bool);
	bool getKeyboard(int);

	// Left
	void LtoR(void);
	void idleLeft(void);
	void stopLeft(void);
	void walkLeft(void);
	void upLeft(void);
	void idleUpLeft(void);
	void downLeft(void);
	void jumpLeft(void);
	void fallLeft(void);

	// Right
	void RtoL(void);
	void idleRight(void);
	void stopRight(void);
	void walkRight(void);
	void upRight(void);
	void idleUpRight(void);
	void downRight(void);
	void jumpRight(void);
	void fallRight(void);

	State state;
	Direction direction;

private:
	void initSprite(void);

	// Left
	Sprite2D LtoRSprite[4];
	Sprite2D idleLeftSprite[1];
	Sprite2D stopLeftSprite[8];
	Sprite2D walkLeftSprite[13];
	Sprite2D upLeftSprite[5];
	Sprite2D idleUpLeftSprite[1];
	Sprite2D downLeftSprite[5];
	Sprite2D jumpLeftSprite[19];
	Sprite2D fallLeftSprite[19];

	// Right
	Sprite2D RtoLSprite[4];
	Sprite2D idleRightSprite[1];
	Sprite2D stopRightSprite[8];
	Sprite2D walkRightSprite[13];
	Sprite2D upRightSprite[5];
	Sprite2D idleUpRightSprite[1];
	Sprite2D downRightSprite[5];
	Sprite2D jumpRightSprite[19];
	Sprite2D fallRightSprite[19];

	bool keyboardState[MAX_KEY_NUM];
};

