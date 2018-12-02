#pragma once
#pragma warning (disable:4996)
#include <stdio.h>
#include <GLM/glm/glm.hpp>
#include "vgl.h"

// Read shader file
static char** loadShaderSource(const char* file)
{
	FILE* fp = fopen(file, "rb");

	if (!fp) {
		return NULL;
	}

	try {
		fseek(fp, 0, SEEK_END);
		long sz = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char *src = new char[sz + 1];
		fread(src, sizeof(char), sz, fp);
		src[sz] = '\0';
		char **srcp = new char*[1];
		srcp[0] = src;

		return srcp;
	}
	catch (...) {}

	return NULL;
}

//Release 2-dimension array
static void freeShaderSource(char** srcp)
{
	delete srcp[0];
	delete srcp;
}

static void shaderLog(GLuint shader)
{
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		GLchar* errorLog = new GLchar[maxLength];
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		printf("%s\n", errorLog);
		delete[] errorLog;
	}
}