#pragma once
#include "Object.h"
#include <string>
#include <iostream>

#define MAX_KEY_NUM			256
#define INIT_MAP_LOC_X		(-0.036f)
#define INIT_MAP_LOC_Y		(-0.8f)
#define TEX_SIZE_WIDTH		(0.17f)
#define BLINK_TIME			70

static std::string stateSymbols[] = {
	"Idle", "Walk", "Stop", "Jump", "Fall", "JumpForward", "Redirect", "Knife" ,"Gun", "Grenade", "Up", "GunUp" , "IdleUp"
};
enum State {
	IDLE, WALK, STOP, JUMP, FALL, JUMP_FORWARD, REDIRECT, KNIFE, GUN, GRENADE, UP, GUNUP, IDLEUP, DEATH_WHICH_ATTACK_FROM_LEFT, DEATH_WHICH_ATTACK_FROM_RIGHT
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

	void reborn();

	int getSpriteId(void);

	// Left
	void LtoR(void);
	void idleLeft(void);
	void stopLeft(void);
	void walkLeft(float);
	void upLeft(void);
	void idleUpLeft(void);
	void downLeft(void);
	void jumpLeft(void);
	void fallLeft(void);
	void knifeLeft(void);
	void shootLeft(void);
	void shootUpLeft(void);
	void grenadeLeft(void);
	void deathLeft(void);

	// Right
	void RtoL(void);
	void idleRight(void);
	void stopRight(void);
	void walkRight(float);
	void upRight(void);
	void idleUpRight(void);
	void downRight(void);
	void jumpRight(void);
	void fallRight(void);
	void knifeRight(void);
	void shootRight(void);
	void shootUpRight(void);
	void grenadeRight(void);
	void deathRight(void);

	State state;
	Direction direction;
	int blinkingCounter;
	bool isDieing;
	bool noDamageStatus;

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
	Sprite2D jumpLeftSprite[9];
	Sprite2D fallLeftSprite[9];
	Sprite2D knifeLeftSprite[8];
	Sprite2D shootLeftSprite[12];
	Sprite2D shootUpLeftSprite[12];
	Sprite2D grenadeLeftSprite[7];
	Sprite2D deathLeftSprite[10];

	// Right
	Sprite2D RtoLSprite[4];
	Sprite2D idleRightSprite[1];
	Sprite2D stopRightSprite[8];
	Sprite2D walkRightSprite[13];
	Sprite2D upRightSprite[5];
	Sprite2D idleUpRightSprite[1];
	Sprite2D downRightSprite[5];
	Sprite2D jumpRightSprite[9];
	Sprite2D fallRightSprite[9];
	Sprite2D knifeRightSprite[8];
	Sprite2D shootRightSprite[12];
	Sprite2D shootUpRightSprite[12];
	Sprite2D grenadeRightSprite[7];
	Sprite2D deathRightSprite[10];

	bool keyboardState[MAX_KEY_NUM];
	GLint blinkingLocation;
};

