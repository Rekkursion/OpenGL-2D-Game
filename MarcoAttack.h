#pragma once
#include "Object.h"

static std::string marcoAttack[] =
{
	"Dally", "Gun", "Knife"
};
enum MAttack {
	BULLET, PINEAPPLE
};
enum Action {
	FLY, EXPLODE
};
enum Orientation {
	L, R, U
};

class MarcoAttack : public Object {
public:
	MarcoAttack(MAttack, Orientation, float, float, float);
	~MarcoAttack();

	glm::vec2 getMapLocation(float sceneLocX, glm::vec2 cur);
	bool nextFrame(void);

	// Bullet
	// Left
	void bulletFlyLeft(float);
	// Right
	void bulletFlyRight(float);
	// Up
	void bulletFlyUp(float);
	// Explosion
	void bulletExplode(float);

	// Pineapple
	// Left
	void pineappleFlyLeft(float);
	// Right
	void pineappleFlyRight(float);
	// Explosion
	void pineappleExplode(float);

	MAttack attack;
	Action action;
	Orientation orient;
	int serviveTimes;

private:
	void initSprite(void);
	float pineappleCurveHighUp[8] = { 0.020, 0.019, 0.018, 0.017, 0.016, 0.015, 0.014, 0.013 };
	float pineappleCurveHighDown[12] = { 0.005, 0.006, 0.008, 0.012, 0.018, 0.026, 0.036, 0.048, 0.062, 0.078, 0.096, 0.116 };
	float pineappleCurveLowUp[3] = { 0.045, 0.030, 0.015 };
	float pineappleCurveLowDown[3] = { 0.015, 0.030, 0.045 };

	// Left
	Sprite2D bulletFlyLeftSprite[2];
	Sprite2D pineappleFlyLeftSprite[28];

	// Right
	Sprite2D bulletFlyRightSprite[2];
	Sprite2D pineappleFlyRightSprite[28];

	// Up
	Sprite2D bulletFlyUpSprite[2];

	// Explosion
	Sprite2D bulletExplosionSprite[11];
	Sprite2D pineappleExplosionSprite[20];

	float mapOffsetX;
};

