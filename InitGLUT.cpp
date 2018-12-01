#include "InitGLUT.h"

InitGLUT::InitGLUT()
{
}

InitGLUT::~InitGLUT()
{
}
// Initial GLUT
void InitGLUT::initialize()
{
	target = this;

	glutKeyboardFunc(keyboardFunc);
	glutKeyboardUpFunc(keyboardUpFunc);
	glutSpecialFunc(specialKeyboardFunc);
	glutSpecialUpFunc(specialKeyboardUpFunc);
	glutMotionFunc(motionFunc);
	glutMouseFunc(mouseFunc);
}
// Get enumerate number
bool InitGLUT::isKeyPress(int key)
{
	return keyboard[key];
}
// Keyboard key press down
void InitGLUT::keyDown(int key)
{
	switch (key)
	{
	case KEY_ESC:
		glutLeaveMainLoop();
		break;
	case KEY_F4:
		glutFullScreenToggle();
	default:
		break;
	}
	glutPostRedisplay();
}
// Keyboard key hop up
void InitGLUT::keyUp(int key)
{
	glutPostRedisplay();
}
// Keyboard key pressing
void InitGLUT::keyPress(int key)
{
	glutPostRedisplay();
}
// Mouse press
void InitGLUT::inputMouse(int button, int state, int x, int y)
{
	glutPostRedisplay();
}
// Keyboard function
void InitGLUT::keyboardFunc(unsigned char key, int x, int y)
{
	target->inputKeyboard(key, true);
}
// Keyboard up function
void InitGLUT::keyboardUpFunc(unsigned char key, int x, int y)
{
	target->inputKeyboard(key, false);
}
// Keyboard special key function
void InitGLUT::specialKeyboardFunc(int key, int x, int y)
{
	target->inputKeyboard(key, true);
}
// Keyboard special key up function 
void InitGLUT::specialKeyboardUpFunc(int key, int x, int y)
{
	target->inputKeyboard(key, false);
}
// Mouse motion function
void InitGLUT::motionFunc(int x, int y)
{
	target->inputMouse(0, 0, x, y);
}
// Mouse press function
void InitGLUT::mouseFunc(int button, int state, int x, int y)
{
	target->inputMouse(button, state, x, y);
}
// Check keyboard table
void InitGLUT::inputKeyboard(int key, bool status)
{
	if (status)
	{
		if (!keyboard[key])
			keyDown(key);
		else
			keyPress(key);
		keyboard[key] = true;
	}
	else
	{
		keyUp(key);
		keyboard[key] = false;
	}
}