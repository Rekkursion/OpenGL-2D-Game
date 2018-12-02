#pragma once
#include "Object.h"
#include <string>
#include <vector>
#define SCENE_WIDTH_IN_SCREEN	0.04731f

static std::string enemyType[] =
{
	"Dally", "Gun", "Knife"
};
enum Arms
{
	DALLY = 0, PISTOL, SABER
};
static Arms armsList[3] = {Arms::DALLY, Arms::PISTOL, Arms::SABER};
enum Death
{
	STAB = 0, SHOT, BOMB
};
enum Side
{
	LSIDE = 0, RSIDE
};

// Range: 0.0 - 0.925
static float mapLocationList[] = {
	0.013, 0.105, 0.152, 0.382, 0.727, 0.255, 0.413, 0.436, 0.086, 0.835, 0.233,
	0.274, 0.867, 0.177, 0.762, 0.754, 0.820, 0.299, 0.589, 0.762
};

class Enemy : public Object {
public:
	static int enemyNum;

	// Born
	Enemy();
	Enemy(Arms, Side, float, float);

	// Death
	~Enemy();

	void setScreenPosX(float);

	// Left
	void dallyLeft(void);
	void pistolLeft(void);
	void saberLeft(void);
	void restLeft(void);
	void deathLeft(Death);

	// Right
	void dallyRight(void);
	void pistolRight(void);
	void saberRight(void);
	void restRight(void);
	void deathRight(Death);

	Arms type;
	Side side;
	float screenLocationX = 0.0;
	float screenLocationY = 0.0;
	float mapLocationX = 0.0;
	float mapLocationY = 0.0;

	int step = 0;
private:
	void initSprite(void);

	// Left
	Sprite2D dallyLeftSprite[28];
	Sprite2D pistolLeftSprite[12];
	Sprite2D saberLeftSprite[12];
	Sprite2D restLeftSprite[7];
	Sprite2D deathKnifeLeftSprite[16];
	Sprite2D deathGunLeftSprite[15];
	Sprite2D deathGrenadeLeftSprite[10];

	// Right
	Sprite2D dallyRightSprite[28];
	Sprite2D pistolRightSprite[12];
	Sprite2D saberRightSprite[12];
	Sprite2D restRightSprite[7];
	Sprite2D deathKnifeRightSprite[16];
	Sprite2D deathGunRightSprite[15];
	Sprite2D deathGrenadeRightSprite[10];
};