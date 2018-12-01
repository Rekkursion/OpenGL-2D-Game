#include <iostream>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include "Marco.h"
#include "Scene.h"
// Rekkursion
#include "PointSprite.h"

using namespace glm;
using namespace std;

/*const int spriteCount = 3;
const int objectCount = 30;*/

GLuint			program;			//shader program
mat4			proj_matrix;		//projection matrix
mat4			modelMatrix;
float			aspect;

GLuint			textureID;

/*Sprite2D sprite2D;
Sprite2D sprite2D2;

Sprite2D* spriteSheets[spriteCount];
SpriteObject* objects[objectCount];*/

/*GLuint vao;
GLuint vbo;
GLuint ebo;*/

void dumpInformation(void);
void My_Init(void);
void My_Display(void);
void My_Reshape(int, int);
void My_KeyboardDown(unsigned char, int, int);
void My_KeyboardUp(unsigned char, int , int);
void My_SpecialKeyDown(int, int, int);
void My_SpecialKeyUp(int, int, int);
void My_Timer(int);
void My_Mouse(int, int, int, int);
void My_Mouse_Moving(int, int);
// Rekkursion
void initFramebufferData(const char*, const char*);
void createRenderbufferAndTexture4Framebuffer();

Marco marco;
Scene scene;

// Rekkursion
PointSprite pointSprite_snow_list[POINT_SPRITE_NUM];

int timerSpeed = 45;
// Rekkursion
int windowWidth = 600;
int windowHeight = 440;

// Rekkursion
GLuint fbo;
GLuint texture4fbo;
GLuint rbo;
GLuint fboProgram;
GLuint fboVAO;
GLuint fboVBO;
static const GLfloat window_positions[] = {
	 1.0f, -1.0f,  1.0f,  0.0f,
	-1.0f, -1.0f,  0.0f,  0.0f,
	-1.0f,  1.0f,  0.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,  1.0f
};

int main(int argc, char *argv[])
{
	srand(time(NULL));

#ifdef __APPLE__
	//Change working directory to source code path
	chdir(__FILEPATH__("/../Assets/"));
#endif
	// Initialize GLUT and GLEW, then create a window.
	////////////////////
	glutInit(&argc, argv);
#ifdef _MSC_VER
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif

	glutInitWindowPosition(100, 100);
	// Rekkursion
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("METAL SLUG X"); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
#ifdef _MSC_VER
	glewInit();
#endif

	//Print debug information 
	dumpInformation();
	////////////////////

	//Call custom initialize function
	My_Init();

	//Register GLUT callback functions
	////////////////////
	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
	glutSpecialFunc(My_SpecialKeyDown);
	glutSpecialUpFunc(My_SpecialKeyUp);
	glutKeyboardFunc(My_KeyboardDown);
	glutKeyboardUpFunc(My_KeyboardUp);
	glutTimerFunc(16, My_Timer, 0);
	glutMouseFunc(My_Mouse);
	glutPassiveMotionFunc(My_Mouse_Moving);
	glutMotionFunc(My_Mouse_Moving);
	////////////////////

	// Enter main event loop.
	glutMainLoop();

	return 0;
}

// Show information
void dumpInformation()
{
	std::cout << "Vender: " << glGetString(GL_VENDOR) << std::endl
		<< "Renderer: " << glGetString(GL_RENDERER) << std::endl
		<< "Version: " << glGetString(GL_VERSION) << std::endl
		<< "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}
// Initial function
void My_Init()
{
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//Initialize shaders
	marco.initialize("../shader/Marco/vertex.vs.glsl", "../shader/Marco/fragment.fs.glsl");
	scene.initialize("../shader/Scene/vertex.vs.glsl", "../shader/Scene/fragment.fs.glsl");

	// Rekkursion
	// Point Sprite - Snow
	for (int k = 0; k < POINT_SPRITE_NUM; k++)
		pointSprite_snow_list[k].pointSpriteInit("../shader/PointSprite/Point_Sprite.vs.glsl", "../shader/PointSprite/Point_Sprite.fs.glsl", (((k & 1) == 1) ? "../media/snow.png" : "../media/snowflake.png"), (float)k);

	// Rekkursion
	initFramebufferData("../shader/FBO/gray.vs.glsl", "../shader/FBO/gray.fs.glsl");
}
// GLUT callback Functions
// Display Function
void My_Display() {
	//std::cout << "State: " << stateSymbols[marco.state] << std::endl;

	// Rekkursion
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// =================================================

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// =================================================

	scene.render();
	// Rekkursion
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(0);

	// =================================================

	// Rekkursion
	for (int k = 0; k < POINT_SPRITE_NUM; k++)
		pointSprite_snow_list[k].render(proj_matrix);

	// =================================================

	bool isFinished = false;
	// Left
	if (marco.getKeyboard(GLUT_KEY_LEFT)) 
	{
		timerSpeed = 45;
		if (marco.state == State::JUMP || marco.state == State::FALL) {
			marco.direction = Direction::LEFT;
		}

		else {
			marco.state = State::WALK;

			if(marco.direction == RIGHT) {
				marco.direction = LEFT;
				marco.nextFrame(4);
				marco.RtoL();
			}
			else {
				marco.nextFrame(13);
				marco.walkLeft();
			}

			scene.moveBG(false);

			if (scene.atHead) {
				if (marco.atCentre) {
					scene.canMove = false;
					marco.canMove = true;
				}

				else {
					scene.canMove = false;
					marco.canMove = true;
				}
			}
			if (scene.atTail) {
			if (marco.atCentre) {
				scene.canMove = true;
				marco.canMove = false;
			}

			else {
				scene.canMove = false;
				marco.canMove = true;
			}
		}
		}
	}

	// Right
	else if (marco.getKeyboard(GLUT_KEY_RIGHT)) 
	{
		timerSpeed = 45;
		if (marco.state == State::JUMP || marco.state == State::FALL) {
			marco.direction = Direction::RIGHT;
		}

		else {
			marco.state = State::WALK;

			if (marco.direction == LEFT) {
				marco.direction = RIGHT;
				marco.nextFrame(4);
				marco.LtoR();
			}
			else {
				marco.nextFrame(13);
				marco.walkRight();
			}
		
			scene.moveBG(true);

			if (scene.atHead) {
				if (marco.atCentre) {
					scene.canMove = true;
					marco.canMove = false;
				}

				else {
					scene.canMove = false;
					marco.canMove = true;
				}
			}
			if (scene.atTail) {
			if (marco.atCentre) {
				scene.canMove = false;
				marco.canMove = true;
			}

			else {
				scene.canMove = false;
				marco.canMove = true;
			}
		}
		}
	}

	// Up
	else if (marco.getKeyboard(GLUT_KEY_UP)) 
	{
		timerSpeed = 45;
		if (marco.state != State::UP) {
			if (marco.direction == LEFT) {
				marco.nextFrame(5);
				marco.upLeft();
			}
			else if (marco.direction == RIGHT) {
				marco.nextFrame(5);
				marco.upRight();
			}

			marco.state = State::UP;
		}
		else {
			if (marco.direction == LEFT) {
				marco.nextFrame(1);
				marco.idleUpLeft();
			}
			else if (marco.direction == RIGHT) {
				marco.nextFrame(1);
				marco.idleUpRight();
			}
		}
	}

	// Jump
	else if (marco.getKeyboard(' '))
	{
		if (marco.state == State::IDLE || marco.state == State::WALK) 
		{
		}
	}
	
	// Idle
	else {
		if (marco.state == State::WALK) 
		{
			if (marco.direction == LEFT) {
				marco.nextFrame(8);
				marco.stopLeft();
			}
			else if (marco.direction == RIGHT) {
				marco.nextFrame(8);
				marco.stopRight();
			}
			if (marco.state == State::WALK) {
				marco.state = State::IDLE;
			}
		}
		else if (marco.state == State::UP) 
		{
			if (marco.direction == LEFT) {
				marco.nextFrame(5);
				marco.downLeft();
			}
			else if (marco.direction == RIGHT) {
				marco.nextFrame(5);
				marco.downRight();
			}

			marco.state = State::IDLE;
		}
		else if (marco.state == State::JUMP) 
		{

		}
		else if (marco.state == State::FALL) {

		}
		else {
			if (marco.direction == LEFT) {
				marco.nextFrame(1);
				marco.idleLeft();
			}
			else if (marco.direction == RIGHT) {
				marco.nextFrame(1);
				marco.idleRight();
			}
		}
	}

	// =================================================

	if (marco.state == State::JUMP) 
	{
		// Rekkursion
		//timerSpeed = 20;
		timerSpeed = 45;

		if (marco.direction == LEFT) {
			marco.jumpLeft();
			isFinished = marco.nextFrame(19);
		}
		else if (marco.direction == RIGHT) {
			marco.jumpRight();
			isFinished = marco.nextFrame(19);
		}

		if (isFinished) 
		{
			//puts("Jump up finish");
			marco.state = State::FALL;
			marco.initSpriteIndex();
		}
		else
			marco.state = State::JUMP;
	}

	else if (marco.state == State::FALL) 
	{
		// Rekkursion
		//timerSpeed = 20;
		timerSpeed = 45;

		if (marco.direction == LEFT) {
			marco.fallLeft();
			isFinished = marco.nextFrame(19);
		}
		else if (marco.direction == RIGHT) {
			marco.fallRight();
			isFinished = marco.nextFrame(19);
		}

		if (isFinished) {
			marco.state = State::IDLE;
			marco.initSpriteIndex();
		}
		else
		{
			marco.state = State::FALL;
		}
	}

	// =================================================

	// Rekkursion
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture4fbo);
	glBindVertexArray(fboVAO);
	glUseProgram(fboProgram);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glutSwapBuffers();
	return;
}

// Reshape Function
void My_Reshape(int width, int height)
{
	aspect = width * 1.0f / height;
	glViewport(0, 0, width, height);

	// Rekkursion
	float viewportAspect = (float)width / (float)height;
	proj_matrix = glm::perspective(80.0f, viewportAspect, -100.0f, 100.0f);

	// Rekkursion
	windowWidth = width;
	windowHeight = height;

	// Rekkursion
	createRenderbufferAndTexture4Framebuffer();
}

unsigned char lastKey = 0;
// Normal keyboard key press down
void My_KeyboardDown(unsigned char key, int x, int y) {
	if (key == lastKey && lastKey == ' ') {
		marco.setKeyboard(key, false);
		return;
	}

	switch (key) {
		case ' ':
			if (marco.state == State::IDLE || marco.state == State::WALK) {
				//puts("space");
				marco.setKeyboard(key, true);
				marco.state = State::JUMP;
				marco.initSpriteIndex();
			}
			break;
	}

	lastKey = key;
}

// Normal keyboard key hop up
void My_KeyboardUp(unsigned char key, int x, int y)
{
	switch (key) {
		case ' ':
			marco.setKeyboard(key, false);
			lastKey = 0;
			//marco.setKeyboard(key, (marco.state != State::JUMP));
			//marco.state = State::IDLE;
			break;
	}
}

// Special keyboard key press down
void My_SpecialKeyDown(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		marco.setKeyboard(key, true);
		break;
	case GLUT_KEY_RIGHT:
		marco.setKeyboard(key, true);
		break;
	case GLUT_KEY_UP:
		marco.setKeyboard(key, true);
		break;
	}
}

// Special keyboard key hop up
void My_SpecialKeyUp(int key, int x, int y) 
{
	switch (key) {
		case GLUT_KEY_LEFT:
			marco.setKeyboard(key, false);
			break;
		case GLUT_KEY_RIGHT:
			marco.setKeyboard(key, false);
			break;
		case GLUT_KEY_UP:
			marco.setKeyboard(key, false);
			break;
	}
}

// Timer function
void My_Timer(int val)
{
	glutPostRedisplay();
	glutTimerFunc(timerSpeed, My_Timer, val);
}

// Mouse press function
void My_Mouse(int button, int state, int x, int y)
{
	//	m_camera.mouseEvents(button, state, x, y);
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
		}
		else if (state == GLUT_UP)
		{
			printf("Mouse %d is released at (%d, %d)\n", button, x, y);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		printf("Mouse %d is pressed\n", button);
	}
	printf("%d %d %d %d\n", button, state, x, y);
}

// Mouse moving function
void My_Mouse_Moving(int x, int y) 
{
	//m_camera.mouseMoveEvent(x, y);
}

#include "ShaderFunction.h"

// Rekkursion
void initFramebufferData(const char* vs, const char* fs) {
	fboProgram = glCreateProgram();

	GLuint vs2 = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs2 = glCreateShader(GL_FRAGMENT_SHADER);
	char** vsSource2 = loadShaderSource(vs);
	char** fsSource2 = loadShaderSource(fs);
	glShaderSource(vs2, 1, vsSource2, NULL);
	glShaderSource(fs2, 1, fsSource2, NULL);
	freeShaderSource(vsSource2);
	freeShaderSource(fsSource2);
	glCompileShader(vs2);
	glCompileShader(fs2);
	shaderLog(vs2);
	shaderLog(fs2);

	//Attach Shader to program
	glAttachShader(fboProgram, vs2);
	glAttachShader(fboProgram, fs2);
	glLinkProgram(fboProgram);

	glGenVertexArrays(1, &fboVAO);
	glBindVertexArray(fboVAO);

	glGenBuffers(1, &fboVBO);
	glBindBuffer(GL_ARRAY_BUFFER, fboVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(window_positions), window_positions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 4, (const GLvoid*)(sizeof(GL_FLOAT) * 2));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenFramebuffers(1, &fbo);

	return;
}

// Rekkursion
void createRenderbufferAndTexture4Framebuffer() {

	// delete old renderbuffer and texture
	glDeleteRenderbuffers(1, &rbo);
	glDeleteTextures(1, &texture4fbo);

	// bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// gen texture
	glGenTextures(1, &texture4fbo);
	glBindTexture(GL_TEXTURE_2D, texture4fbo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture4fbo, 0);

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

	return;
}