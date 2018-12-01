#pragma once

#include <string>
#include "vgl.h"

#define GLM_SWIZZLE
#include <GLM/glm/glm.hpp>
#include <GLM/glm/gtc/matrix_transform.hpp>
#include <GLM/glm/gtc/type_ptr.hpp>
#include <GLM/glm/gtx/rotate_vector.hpp>

typedef struct _TextureData
{
	_TextureData() : width(0), height(0), data(0) {}
	int width;
	int height;
	unsigned char* data;
} TextureData;


class Sprite2D
{
public:
	Sprite2D();
	~Sprite2D();

	bool Init(std::string fileName, int FPS);
	void Enable();
	void Disable();

	float GetFPS();
	float GetAspect();
	int GetCount();
	int GetIndex();
	glm::mat4 GetModelMat();

private:
	float spriteAspect;
	float spriteFPS;
	int spriteCount;
	int subWidth;
	int subHeight;
	GLuint textureID;
	glm::mat4 scaleMat;
};

