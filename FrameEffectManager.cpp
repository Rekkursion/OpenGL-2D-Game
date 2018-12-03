#include "FrameEffectManager.h"

FrameEffectManager* FrameEffectManager::instance = NULL;


FrameEffectManager::FrameEffectManager(): windowWidth(600), windowHeight(440) {
	endingPivot = 3.0f;
}

FrameEffectManager::~FrameEffectManager() {}

void FrameEffectManager::setWindowData(int w, int h) {
	windowWidth = w;
	windowHeight = h;
}

void FrameEffectManager::initFrameBufferData(const char* vs, const char* fs) {
	char** vsSource2 = loadShaderSource(vs);
	char** fsSource2 = loadShaderSource(fs);

	if (vsSource2 == NULL || fsSource2 == NULL)
		return;

	glDeleteFramebuffers(1, &fbo);
	glDeleteProgram(program);

	program = glCreateProgram();

	GLuint vs2 = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vs2, 1, vsSource2, NULL);
	glShaderSource(fs2, 1, fsSource2, NULL);
	freeShaderSource(vsSource2);
	freeShaderSource(fsSource2);
	glCompileShader(vs2);
	glCompileShader(fs2);
	shaderLog(vs2);
	shaderLog(fs2);

	//Attach Shader to program
	glAttachShader(program, vs2);
	glAttachShader(program, fs2);
	glLinkProgram(program);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(window_positions), window_positions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4, (const GLvoid*)(sizeof(GL_FLOAT) * 2));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenFramebuffers(1, &fbo);

	endingPivotLocation = glGetUniformLocation(program, "pivot");
	endingPivot = 3.0f;
}

void FrameEffectManager::createRenderbufferAndTexture4Framebuffer() {
	// delete old renderbuffer and texture
	glDeleteRenderbuffers(1, &rbo);
	glDeleteTextures(1, &texture);

	// bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// gen texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	// gen renderbuffer
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// attach renderbuffer to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// check if framebuffer is completely created
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		puts("Create framebuffer successfully :)");
	else
		puts("Create framebuffer failed :(");

	// unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameEffectManager::enable() {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameEffectManager::disable() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}

void FrameEffectManager::effectRender() {
	endingPivot -= ENDING_PIVOT_DECREMENT;
	if (endingPivot < 0.0f)
		endingPivot = 0.0f;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(vao);
	glUseProgram(program);
	glUniform1f(endingPivotLocation, endingPivot);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

bool FrameEffectManager::isAnimationEnd() {
	return (endingPivot <= 0.0000000001);
}