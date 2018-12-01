#pragma once
#include <iostream>
#include <string>
#include "vgl.h"

class InitOpenGL
{
public:
	InitOpenGL();
	~InitOpenGL();
public:
	static int width;
	static int height;
public:
	virtual void initialize(const char*, int, int, int, int);
	virtual void dumpInformation();
	virtual void display(bool);
	virtual void start(void);
	virtual void finish(void);
	virtual void update(void);
	virtual void reshape(int, int);
protected:
	static InitOpenGL* target;
	static void displayFunc(void);
	static void reshapeFunc(int, int);
	static void closeFunc(void);
	static void idleFunc();
};

