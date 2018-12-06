#pragma once
#include "Object.h"
#include <cmath>

#define DOT_LIGHT_SCREEN_RADIUS		(0.37f)
#define DOT_LIGHT_SCREEN_ANGLE_SPEED	(3.14f / 64.0f)

class DotLight: public Object {
public:
	DotLight();
	DotLight(float, float);
	~DotLight();

	void initialize(const char*, const char*);
	void render(float, float);
	float getScreenPosZ();

private:
	void initSprite();

	float radAngle;
	float radius;
	glm::vec3 screenPos3D;
	//GLint screenPos3DLocation;

	Sprite2D dotLightSprite;
};

