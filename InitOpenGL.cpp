#include "InitOpenGL.h"
#pragma warning(disable:4996)

int InitOpenGL::width = 1080;
int InitOpenGL::height = 720;

InitOpenGL::InitOpenGL()
{
}

InitOpenGL::~InitOpenGL()
{
}
// Initalize OpenGL
void InitOpenGL::initialize(
	const char* title, // Title of screen
	int x, int y, // Position of the screen 
	int w, int h // Size of the screen Width & Height
)
{
	int one = 1;
	char *name = new char[5]; // remember to add 1 for null terminator
	strcpy(name, "name");
	
	target = this;
	width = w;
	height = h;

	// Initial GLUT and create window
	glutInit(&one, &name);
	glutInitContextVersion(4, 3);
#ifdef _DEBUG
	glutInitContextFlags(GLUT_DEBUG);
#endif // _DEBUG
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(x, y);
	glutInitWindowSize(width, height);
	glutCreateWindow(title ? title : "OpenGL");
	// Register callbacks & render function
	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	glutIdleFunc(idleFunc);
	glutCloseFunc(closeFunc);

	// Call GLEW initialize function
	if (glewInit())
	{
		// C error catch
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		// Print debug information
		dumpInformation();
	}
}

void InitOpenGL::dumpInformation()
{
	std::cout << "Vender: " << glGetString(GL_VENDOR) << std::endl
		<< "Renderer: " << glGetString(GL_RENDERER) << std::endl
		<< "Version: " << glGetString(GL_VERSION) << std::endl
		<< "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void InitOpenGL::display(bool autoRedraw = true)
{
	glutSwapBuffers();
	if (autoRedraw)
	{
		glutPostRedisplay();
	}
}
// Start OpenGL
void InitOpenGL::start()
{
	glutMainLoop();
}
// Final
void InitOpenGL::finish()
{

}
// Update
void InitOpenGL::update()
{
}
// Reshape function
void InitOpenGL::reshape(int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, width, height);
}
// Display function
void InitOpenGL::displayFunc()
{
	target->display();
}
// Reshape Function
void InitOpenGL::reshapeFunc(int w, int h)
{
	target->reshape(w, h);
}
// Close Function
void InitOpenGL::closeFunc()
{
	target->finish();
}
// Idle function
void InitOpenGL::idleFunc()
{
	target->update();
}