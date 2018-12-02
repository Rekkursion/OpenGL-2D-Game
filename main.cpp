#include <iostream>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include "Marco.h"
#include "Enemy.h"
#include "Scene.h"
#include "PointSprite.h"
#include "FrameEffectManager.h"

using namespace glm;
using namespace std;

#define CGI

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
void menuEvents(int option);
void fboParamMenuEvents(int option);

enum DisplayMode { 
	NORMAL,
	POINT_SPRITE_NUMBER,
	POINT_SPRITE_SPEED,
	POINT_SPRITE_DIRECTION
};
DisplayMode displayMode = DisplayMode::NORMAL;

Marco marco;
Scene scene;
//Enemy enemy(Arms::PISTOL, Side::RSIDE, 0.0f, -0.8f);
vector<Enemy> enemyList;

#ifdef CGI
PointSprite pointSprite_snow_list[POINT_SPRITE_NUM];
FrameEffectManager* fboManager = FrameEffectManager::getInstance();
#endif // CGI

int timerSpeed = 45;
int windowWidth = 600;
int windowHeight = 440;

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

	// Menu
	int fboParamMenu = glutCreateMenu(fboParamMenuEvents);
	glutAddMenuEntry("Number", 0);
	glutAddMenuEntry("Speed", 1);
	glutAddMenuEntry("Direction", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutCreateMenu(menuEvents);
	glutAddMenuEntry("Normal mode", 9999);
	glutAddSubMenu("FBO effect parameters", fboParamMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

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

	// Enemies
	cout << "EnemyNum: " << sizeof(mapLocationList) / sizeof(float) << endl;
	enemyList.clear();
	enemyList.resize(sizeof(mapLocationList) / sizeof(float));
	for (int k = 0; k < enemyList.size(); k++) {
		Enemy ene(armsList[(k % 3)], Side::RSIDE, 0.0, -0.8);
		ene.initialize("../shader/Enemy/vertex.vs.glsl", "../shader/Enemy/fragment.fs.glsl");
		ene.setScreenPosX(scene.getPosX());
		enemyList[k] = ene;
	}

	#ifdef CGI
	// Point Sprite - Snow
	for (int k = 0; k < POINT_SPRITE_NUM; k++)
		pointSprite_snow_list[k].pointSpriteInit("../shader/PointSprite/Point_Sprite.vs.glsl", "../shader/PointSprite/Point_Sprite.fs.glsl", (((k & 1) == 1) ? "../media/snow.png" : "../media/snowflake.png"), (float)k);

	// Framebuffer
	fboManager->initFrameBufferData("../shader/FBO/gray.vs.glsl", "../shader/FBO/gray.fs.glsl");
	#endif // CGI
}
// GLUT callback Functions
// Display Function
void My_Display() {
	//std::cout << "State: " << stateSymbols[marco.state] << std::endl;
	//std::cout << "display mode: " << displayMode << std::endl;

	if(marco.blinkingCounter >= 0)
		marco.blinkingCounter++;

	if (marco.blinkingCounter >= BLINK_TIME)
		marco.blinkingCounter = -1;

	#ifdef CGI
	fboManager->enable();
	#endif // CGI

	// =================================================

	// Initial background
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// =================================================

	// Scene render
	scene.render();
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(0);

	// =================================================

	// Pointsprite render
	#ifdef CGI
	for (int k = 0; k < POINT_SPRITE_NUM; k++)
		pointSprite_snow_list[k].render(proj_matrix);
	#endif // CGI

	// =================================================

	// Enemy render
	for (int k = 0; k < enemyList.size(); k++) {
		enemyList[k].setScreenPosX(scene.getPosX());

		bool replay = false;
		if (enemyList[k].step == 0) {
			if (enemyList[k].type == Arms::DALLY) {
				if (enemyList[k].step == 0) {
					if (enemyList[k].side == LSIDE) {
						enemyList[k].dallyLeft();
						replay = enemyList[k].nextFrame(28 * 2);
					}
					else if (enemyList[k].side == RSIDE) {
						enemyList[k].dallyRight();
						replay = enemyList[k].nextFrame(28 * 2);
					}

				}
			}
			else if (enemyList[k].type == Arms::PISTOL) {
				if (enemyList[k].side == LSIDE) {
					enemyList[k].pistolLeft();
					replay = enemyList[k].nextFrame(12 * 2);
				}
				else if (enemyList[k].side == RSIDE) {
					enemyList[k].pistolRight();
					replay = enemyList[k].nextFrame(12 * 2);
				}
			}
			else if (enemyList[k].type == Arms::SABER) {
				if (enemyList[k].side == LSIDE) {
					enemyList[k].saberLeft();
					replay = enemyList[k].nextFrame(12 * 2);
				}
				else if (enemyList[k].side == RSIDE) {
					enemyList[k].saberRight();
					replay = enemyList[k].nextFrame(12 * 2);
				}
			}
			if (replay) {
				enemyList[k].initSpriteIndex();
				enemyList[k].step++;
			}
		}
		else {
		if (enemyList[k].side == LSIDE) {
			enemyList[k].restLeft();
			replay = enemyList[k].nextFrame(7 * 2);
		}
		else if (enemyList[k].side == RSIDE) {
			enemyList[k].restRight();
			replay = enemyList[k].nextFrame(7 * 2);
		}
		if (replay) {
			enemyList[k].initSpriteIndex();
			enemyList[k].step++;
			if (enemyList[k].step > 3)
				enemyList[k].step = 0;
		}
	}
	}

	// =================================================

	// Marco Action
	bool isFinished = false;

	// Left
	if (marco.getKeyboard(GLUT_KEY_LEFT)) {
		if (marco.state == State::JUMP || marco.state == State::FALL) {
			marco.direction = Direction::LEFT;
		}

		else {
			marco.state = State::WALK;

			if (marco.direction == RIGHT) {
				marco.direction = LEFT;
				marco.RtoL();
				marco.nextFrame(4);
			}
			else {
				marco.walkLeft();
				marco.nextFrame(13);
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
	else if (marco.getKeyboard(GLUT_KEY_RIGHT)) {
		if (marco.state == State::JUMP || marco.state == State::FALL) {
			marco.direction = Direction::RIGHT;
		}

		else {
			marco.state = State::WALK;

			if (marco.direction == LEFT) {
				marco.direction = RIGHT;
				marco.LtoR();
				marco.nextFrame(4);
			}
			else {
				marco.walkRight();
				marco.nextFrame(13);
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
	else if (marco.getKeyboard(GLUT_KEY_UP)) {
		cout << marco.state << endl;

		if (marco.state != State::IDLEUP) {
			if (marco.direction == LEFT) {
				marco.upLeft();
				marco.nextFrame(5);
			}
			else if (marco.direction == RIGHT) {
				marco.upRight();
				marco.nextFrame(5);
			}
			marco.state = State::IDLEUP;
		}
		else if (marco.state == State::IDLEUP) {
			if (marco.direction == LEFT) {
				marco.idleUpLeft();
				marco.nextFrame(1);
			}
			else if (marco.direction == RIGHT) {
				marco.idleUpRight();
				marco.nextFrame(1);
			}
			marco.state = State::IDLEUP;
		}
		else if (marco.state == State::GUNUP) {
			cout << "GUNUP\n";
			if (marco.direction == LEFT) {
				marco.shootUpLeft();
				isFinished = marco.nextFrame(12);
			}
			else if (marco.direction == RIGHT) {
				marco.shootUpRight();
				isFinished = marco.nextFrame(12);
			}
			if (isFinished) {
				marco.state = State::IDLEUP;
				marco.initSpriteIndex();
			}
			else
				marco.state = State::GUNUP;
		}
	}

	// Idle
	else {
		if (marco.state == State::WALK) {
			if (marco.direction == LEFT) {
				marco.stopLeft();
				marco.nextFrame(8);
			}
			else if (marco.direction == RIGHT) {
				marco.stopRight();
				marco.nextFrame(8);
			}
			if (marco.state == State::WALK) {
				marco.state = State::STOP;
			}
		}
		else if (marco.state == State::IDLEUP) {
			if (marco.direction == LEFT) {
				marco.downLeft();
				marco.nextFrame(5);
			}
			else if (marco.direction == RIGHT) {
				marco.downRight();
				marco.nextFrame(5);
			}
			marco.state = State::IDLE;
		}
		else if (marco.state == State::JUMP) {

		}
		else if (marco.state == State::FALL) {

		}
		else if (marco.state == State::STOP) {

		}
		else if (marco.state == State::KNIFE) {

		}
		else if (marco.state == State::GUN) {
		}
		else if (marco.state == State::GRENADE) {

		}

		// Idle
		else {
			if (marco.direction == LEFT) {
				marco.idleLeft();
				marco.nextFrame(1);
			}
			else if (marco.direction == RIGHT) {
				marco.idleRight();
				marco.nextFrame(1);
			}
		}
	}

	// =================================================

	// Jump
	if (marco.state == State::JUMP) {
		if (marco.direction == LEFT) {
			marco.jumpLeft();
			isFinished = marco.nextFrame(9);
		}
		else if (marco.direction == RIGHT) {
			marco.jumpRight();
			isFinished = marco.nextFrame(9);
		}

		if (isFinished) {
			//puts("Jump up finish");
			marco.state = State::FALL;
			marco.initSpriteIndex();
		}
		else
			marco.state = State::JUMP;
	}

	// Fall
	else if (marco.state == State::FALL) {
		if (marco.direction == LEFT) {
			marco.fallLeft();
			isFinished = marco.nextFrame(9);
		}
		else if (marco.direction == RIGHT) {
			marco.fallRight();
			isFinished = marco.nextFrame(9);
		}

		if (isFinished) {
			marco.state = State::STOP;
			marco.initSpriteIndex();
		}
		else {
			marco.state = State::FALL;
		}
	}

	// Stop
	else if (marco.state == State::STOP) {
		if (marco.direction == LEFT) {
			marco.stopLeft();
			isFinished = marco.nextFrame(8);
		}
		else if (marco.direction == RIGHT) {
			marco.stopRight();
			isFinished = marco.nextFrame(8);
		}
		if (isFinished) {
			marco.state = State::IDLE;
			marco.initSpriteIndex();
		}
	}

	// Knife attack
	if (marco.state == State::KNIFE) {
		if (marco.direction == LEFT) {
			marco.knifeLeft();
			isFinished = marco.nextFrame(8);
		}
		else if (marco.direction == RIGHT) {
			marco.knifeRight();
			isFinished = marco.nextFrame(8);
		}

		if (isFinished) {
			marco.state = State::IDLE;
			marco.initSpriteIndex();
		}
	}

	// Gun attack
	if (marco.state == State::GUN) {
		if (marco.direction == LEFT) {
			marco.shootLeft();
			isFinished = marco.nextFrame(12);
		}
		else if (marco.direction == RIGHT) {
			marco.shootRight();
			isFinished = marco.nextFrame(12);
		}
		if (isFinished) {
			marco.state = State::IDLE;
			marco.initSpriteIndex();
		}
	}

	// Grenade attack
	if (marco.state == State::GRENADE) {
		if (marco.direction == LEFT) {
			marco.grenadeLeft();
			isFinished = marco.nextFrame(7);
		}
		else if (marco.direction == RIGHT) {
			marco.grenadeRight();
			isFinished = marco.nextFrame(7);
		}
		if (isFinished) {
			marco.state = State::IDLE;
			marco.initSpriteIndex();
		}
	}

	// =================================================

	#ifdef CGI
	fboManager->disable();
	fboManager->effectRender();
	#endif // CGI

	glutSwapBuffers();
	return;
}

// Reshape Function
void My_Reshape(int width, int height)
{
	aspect = width * 1.0f / height;
	glViewport(0, 0, width, height);

	float viewportAspect = (float)width / (float)height;
	proj_matrix = glm::perspective(80.0f, viewportAspect, -100.0f, 100.0f);

	windowWidth = width;
	windowHeight = height;

	#ifdef CGI
	fboManager->setWindowData(windowWidth, windowHeight);
	fboManager->createRenderbufferAndTexture4Framebuffer();
	#endif // CGI
}

unsigned char lastKey = 0;
// Normal keyboard key press down
void My_KeyboardDown(unsigned char key, int x, int y) {

	if (key == lastKey && lastKey == ' ') {
		marco.setKeyboard(key, false);
		return;
	}
	else if (key == lastKey && (lastKey == 'z' || lastKey == 'Z')) {
		marco.setKeyboard(key, false);
		return;
	}
	else if (key == lastKey && (lastKey == 'x' || lastKey == 'X')) {
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
		case 'z':
		case 'Z':
			if (marco.state == State::IDLE || marco.state == State::WALK) {
				//puts("Gun or Knife");
				marco.setKeyboard(key, true);
				//marco.state = State::KNIFE;
				marco.state = State::GUN;
				marco.initSpriteIndex();
			}
			else if (marco.state == State::IDLEUP) {
				puts("Gun up");
				marco.setKeyboard(key, true);
				marco.state = State::GUNUP;
				marco.initSpriteIndex();
			}
			break;
		case 'x':
		case 'X':
			if (marco.state == State::IDLE || marco.state == State::WALK) {
				puts("Grenade");
				marco.setKeyboard(key, true);
				marco.state = State::GRENADE;
				marco.initSpriteIndex();
			}
			break;
	}

	lastKey = key;
}

// Normal keyboard key hop up
void My_KeyboardUp(unsigned char key, int x, int y) {
	switch (key) {
		case ' ':
			marco.setKeyboard(key, false);
			lastKey = 0;
			break;
		case 'z':
		case 'Z':
			cout << "Attack finish" << endl;
			marco.setKeyboard(key, false);
			lastKey = 0;
			break;
		case 'x':
		case 'X':
			marco.setKeyboard(key, false);
			lastKey = 0;
			break;
	}
}

// Special keyboard key press down
void My_SpecialKeyDown(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT:
			marco.setKeyboard(key, true);
			break;
		case GLUT_KEY_RIGHT:
			marco.setKeyboard(key, true);
			break;
		case GLUT_KEY_UP:
			if (displayMode == DisplayMode::NORMAL) {
				marco.setKeyboard(key, true);
			}
			break;
	}
}

// Special keyboard key hop up
void My_SpecialKeyUp(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_LEFT:
			marco.setKeyboard(key, false);
			break;
		case GLUT_KEY_RIGHT:
			marco.setKeyboard(key, false);
			break;
		case GLUT_KEY_UP:
			if (displayMode == DisplayMode::NORMAL) {
				marco.setKeyboard(key, false);
			}

			#ifdef CGI
			// increase stars num
			else if (displayMode == DisplayMode::POINT_SPRITE_NUMBER) {

				for (int k = 0; k < POINT_SPRITE_NUM; k++) {
					pointSprite_snow_list[k].increaseStarsNum();
					pointSprite_snow_list[k].pointSpriteInit("../shader/PointSprite/Point_Sprite.vs.glsl", "../shader/PointSprite/Point_Sprite.fs.glsl", (((k & 1) == 1) ? "../media/snow.png" : "../media/snowflake.png"), (float)k);
				}
			}

			// increase speed
			else if (displayMode == DisplayMode::POINT_SPRITE_SPEED) {
				for (int k = 0; k < POINT_SPRITE_NUM; k++) {
					pointSprite_snow_list[k].increaseStarsSpeed();
				}
			}

			// increase angle (to left)
			else if (displayMode == DisplayMode::POINT_SPRITE_DIRECTION) {
				for (int k = 0; k < POINT_SPRITE_NUM; k++) {
					pointSprite_snow_list[k].increaseStarsAngle();
				}
			}
			#endif // CGI

			break;
		case GLUT_KEY_DOWN:
			if (displayMode == DisplayMode::NORMAL) {}

			#ifdef CGI
			// decrease stars num
			else if (displayMode == DisplayMode::POINT_SPRITE_NUMBER) {

				for (int k = 0; k < POINT_SPRITE_NUM; k++) {
					pointSprite_snow_list[k].decreaseStarsNum();
					pointSprite_snow_list[k].pointSpriteInit("../shader/PointSprite/Point_Sprite.vs.glsl", "../shader/PointSprite/Point_Sprite.fs.glsl", (((k & 1) == 1) ? "../media/snow.png" : "../media/snowflake.png"), (float)k);
				}
			}

			// decrease speed
			else if (displayMode == DisplayMode::POINT_SPRITE_SPEED) {
				for (int k = 0; k < POINT_SPRITE_NUM; k++) {
					pointSprite_snow_list[k].decreaseStarsSpeed();
				}
			}

			// decrease angle (to right)
			else if (displayMode == DisplayMode::POINT_SPRITE_DIRECTION) {
				for (int k = 0; k < POINT_SPRITE_NUM; k++) {
					pointSprite_snow_list[k].decreaseStarsAngle();
				}
			}
			#endif // CGI

			break;

		case GLUT_KEY_F1:
			marco.reborn();
			break;
	}
}

// Timer function
void My_Timer(int val) {
	glutPostRedisplay();
	glutTimerFunc(timerSpeed, My_Timer, val);
}

// Mouse press function
void My_Mouse(int button, int state, int x, int y) {
	//	m_camera.mouseEvents(button, state, x, y);
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
		}
		else if (state == GLUT_UP) {
			printf("Mouse %d is released at (%d, %d)\n", button, x, y);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		printf("Mouse %d is pressed\n", button);
	}
	printf("%d %d %d %d\n", button, state, x, y);
}

// Mouse moving function
void My_Mouse_Moving(int x, int y) {
	//m_camera.mouseMoveEvent(x, y);
}

void menuEvents(int option) {
	switch (option) {
		case 9999:
			displayMode = DisplayMode::NORMAL;
			break;
		default:
			displayMode = DisplayMode::NORMAL;
			break;
	}

	return;
}

void fboParamMenuEvents(int option) {
	switch (option) {
		case 0: displayMode = DisplayMode::POINT_SPRITE_NUMBER; break;
		case 1: displayMode = DisplayMode::POINT_SPRITE_SPEED; break;
		case 2: displayMode = DisplayMode::POINT_SPRITE_DIRECTION; break;
	}

	return;
}