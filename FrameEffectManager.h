#pragma once
#include "vgl.h"
#include "ShaderFunction.h"
#include <GLM/glm/glm.hpp>
#include <iostream>
#define ENDING_PIVOT_DECREMENT 0.05

static const GLfloat window_positions[] = {
	 1.0f, -1.0f,  1.0f,  0.0f,
	-1.0f, -1.0f,  0.0f,  0.0f,
	-1.0f,  1.0f,  0.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,  1.0f
};

class FrameEffectManager {
public:
	static FrameEffectManager* getInstance() {
		if (instance == NULL)
			instance = new FrameEffectManager();

		return instance;
	}

	void setWindowData(int, int);
	void initFrameBufferData(const char*, const char*);
	void createRenderbufferAndTexture4Framebuffer();
	void enable();
	void disable();
	void effectRender();
	bool isAnimationEnd();

private:
	static FrameEffectManager* instance;

	FrameEffectManager();
	~FrameEffectManager();

	int windowWidth;
	int windowHeight;

	GLuint program;
	GLuint vao;
	GLuint vbo;
	GLuint fbo;
	GLuint rbo;
	GLuint texture;
	GLint endingPivotLocation;
	float endingPivot; // from 3.0 to 1.0
};

