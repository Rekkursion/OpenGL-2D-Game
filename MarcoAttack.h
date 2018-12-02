#pragma once
#include "Object.h"

static std::string marcoAttack[] =
{
	"Dally", "Gun", "Knife"
};
enum MAttack
{
	BULLET, PINEAPPLE
};
enum Orientation
{
	L, R
};

class MarcoAttack : public Object
{
public:
	MarcoAttack();
	~MarcoAttack();

	// Bullet
	void saberLeft(void);
	void restLeft(void);

	// Pineapple
	// Left
	void bulletFlyLeft(void);
	void bulletExplodeLeft(void);
	void pineappleFlyLeft(void);
	void pineappleExplodeLeft(void);
	// Right
	void bulletFlyRight(void);
	void bulletExplodeRight(void);
	void pineappleFlyRight(void);
	void pineappleExplodeRight(void);

};

