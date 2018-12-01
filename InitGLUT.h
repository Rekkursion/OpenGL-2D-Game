#pragma once
#include <iostream>
#include "vgl.h"

class InitGLUT
{
public:
	InitGLUT();
	~InitGLUT();

	void initialize();
	bool isKeyPress(int);
public:
	// Enumerate of mouse button
	enum Mouse_Btn 
	{
		LEFT = 0,
		MIDDLE = 1,
		RIGHT = 2
	};
	// Enumerate of mouse state
	enum Mouse_State 
	{
		PRESS = 0,
		UP = 1
	};
	// Enumerate of special key
	enum SpecialKey 
	{
		KEY_F1 = 1,
		KEY_F2 = 2,
		KEY_F3 = 3,
		KEY_F4 = 4,
		KEY_F5 = 5,
		KEY_F6 = 6,
		KEY_F7 = 7,
		KEY_F8 = 8,
		KEY_F9 = 9,
		KEY_F10 = 10,
		KEY_F11 = 11,
		KEY_F12 = 12,
		KEY_ENTER = 13,
		KEY_ESC = 27,
		KEY_SPACE = 32,
		KEY_LEFT = 100,
		KEY_UP = 101,
		KEY_RIGHT = 102,
		KEY_DOWN = 103,
		KEY_LEFT_SHIFT = 112,
		KEY_RIGHT_SHIFT = 113,
		KEY_LEFT_CTRL = 114,
		KEY_RIGHT_CTRL = 115,
		KEY_LEFT_ALT = 116,
		KEY_RIGHT_ALT = 117,
		KEY_D1 = 49,
		KEY_D2 = 50,
		KEY_D3 = 51,
		KEY_D4 = 52,
		KEY_D5 = 53,
		KEY_D6 = 54,
		KEY_D7 = 55,
		KEY_D8 = 56,
		KEY_D9 = 57,
		KEY_D0 = 48,
		KEY_DELETE = 127,
	};
protected:
	virtual void keyDown(int);
	virtual void keyUp(int);
	virtual void keyPress(int);
	virtual void inputMouse(int, int, int, int);
protected:
	bool keyboard[256];

	static InitGLUT* target;
	static void keyboardFunc(unsigned char, int, int);
	static void keyboardUpFunc(unsigned char, int, int);
	static void specialKeyboardFunc(int, int, int);
	static void specialKeyboardUpFunc(int, int, int);
	static void motionFunc(int, int);
	static void mouseFunc(int, int, int, int);
private:
	void inputKeyboard(int, bool);
};