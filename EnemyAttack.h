#pragma once
#include "Object.h"

enum EAttack {
	BLINK
};

class EnemyAttack : public Object {
public:
	static float BULLET_ATTACK_GAP_X;
	static float BULLET_ATTACK_GAP_Y;
	static float KNIFE_ATTACK_GAP_X_BIGGER;
	static float KNIFE_ATTACK_GAP_X_SMALLER;
	static float KNIFE_ATTACK_GAP_Y;

	EnemyAttack();
	EnemyAttack(EAttack, Orientation, float, float);
	~EnemyAttack();

	bool nextFrame(void);

	//Blink
	// Left
	void blinkFlyLeft(float);
	// Right
	void blinkFlyRight(float);
	// Explosion
	void bulletExplode(float);

	EAttack attack;
	Action action;
	Orientation orient;
	int serviveTimes;

private:
	void initSprite(void);
	// Fly
	Sprite2D blinkFlySprite[2];
	// Explosion
	Sprite2D blinkExplosionSprite[6];
};