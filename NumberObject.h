#pragma once
#include "Object.h"

#define DIGIT_ONE_LOC_X	(0.9)
#define DIGIT_TEN_LOC_X	(0.8)
#define DIGIT_LOC_Y		(0.71)

class NumberObject: public Object {
public:
	NumberObject();
	NumberObject(int, glm::vec2);
	~NumberObject();
	void setNumber(int);
	int getNumber();
	void setLocation(glm::vec2);
	void render();

private:
	void initSprite(void);
	int number;
	Sprite2D numberSprite[10];
};

