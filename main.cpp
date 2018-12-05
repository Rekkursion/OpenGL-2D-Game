#include <iostream>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include "Marco.h"
#include "MarcoAttack.h"
#include "Enemy.h"
#include "EnemyAttack.h"
#include "Scene.h"
#include "PointSprite.h"
#include "FrameEffectManager.h"
#include "NumberObject.h"

using namespace glm;
using namespace std;

#define CGI
#define MAX_TIME_DIGIT 4

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
void My_Counter(int);
void My_Mouse(int, int, int, int);
void My_Mouse_Moving(int, int);
void menuEvents(int);
void restartMenuEvents(int);
void fboParamMenuEvents(int);
void fboChoiceMenuEvents(int);

enum DisplayMode { 
	NORMAL,
	POINT_SPRITE_NUMBER,
	POINT_SPRITE_SPEED,
	POINT_SPRITE_DIRECTION,
	ONE_ENEMY
};
DisplayMode displayMode = DisplayMode::NORMAL;

Marco marco;
Scene scene;
vector<Enemy> enemyList;
NumberObject digitOne, digitTen;
NumberObject timeDigits[MAX_TIME_DIGIT];

vector<MarcoAttack> mAttack;
vector<EnemyAttack> eAttack;

#ifdef CGI
PointSprite pointSprite_snow_list[POINT_SPRITE_NUM];
FrameEffectManager* fboManager = FrameEffectManager::getInstance();
#endif // CGI

enum FBOChoice {
	NONE = 0,
	MORE_BLACK_WITH_FOG,
	MORE_BLACK,
	FOG,
	MARCO_DEATH,
	ENDING,
	SHOW_MISSION_COMPLETE
};

int timerSpeed = 45;
int windowWidth = 600;
int windowHeight = 440;
int remainEnemyNum;
int currentTime;
int fboChoice;
bool isMissionCompleted;
int fboParamMenu;
int fboChoiceMenu;
int eventMenu;
int restartMenu;

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
	glutTimerFunc(1000, My_Counter, 0);
	glutMouseFunc(My_Mouse);
	glutPassiveMotionFunc(My_Mouse_Moving);
	glutMotionFunc(My_Mouse_Moving);
	////////////////////

	// Menu
	fboParamMenu = glutCreateMenu(fboParamMenuEvents);
	glutAddMenuEntry("Number", 0);
	glutAddMenuEntry("Speed", 1);
	glutAddMenuEntry("Direction", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	fboChoiceMenu = glutCreateMenu(fboChoiceMenuEvents);
	glutAddMenuEntry("None", FBOChoice::NONE);
	glutAddMenuEntry("More black with fog", FBOChoice::MORE_BLACK_WITH_FOG);
	glutAddMenuEntry("More black", FBOChoice::MORE_BLACK);
	glutAddMenuEntry("Fog", FBOChoice::FOG);
	glutAddMenuEntry("Marco Death", FBOChoice::MARCO_DEATH);
	glutAddMenuEntry("Ending", FBOChoice::ENDING);

	eventMenu = glutCreateMenu(menuEvents);
	glutAddMenuEntry("Normal mode", 9999);
	glutAddMenuEntry("One enemy", 9998);
	glutAddSubMenu("Point sprite effect parameters", fboParamMenu);
	glutAddSubMenu("Framebuffer object effects", fboChoiceMenu);

	restartMenu = glutCreateMenu(restartMenuEvents);
	glutAddMenuEntry("Restart", 2147);

	glutSetMenu(eventMenu);
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
		Enemy ene(armsList[(k % 3)], Side::RSIDE, 0.0, -0.8, scene.getPosX());
		ene.initialize("../shader/Enemy/vertex.vs.glsl", "../shader/Enemy/fragment.fs.glsl");
		ene.setScreenPosX(scene.getPosX());
		enemyList[k] = ene;
	}
	Enemy::enemyNum++;
	remainEnemyNum = Enemy::enemyNum;

	// Number objects
	digitOne.setNumber(remainEnemyNum % 10);
	digitOne.setLocation(glm::vec2(DIGIT_ONE_LOC_X, DIGIT_LOC_Y));
	digitOne.initialize("../shader/NumberObject/vertex.vs.glsl", "../shader/NumberObject/fragment.fs.glsl");

	digitTen.setNumber(remainEnemyNum / 10);
	digitTen.setLocation(glm::vec2(DIGIT_TEN_LOC_X, DIGIT_LOC_Y));
	digitTen.initialize("../shader/NumberObject/vertex.vs.glsl", "../shader/NumberObject/fragment.fs.glsl");

	currentTime = 0;
	for (int k = 0; k < MAX_TIME_DIGIT; k++) {
		timeDigits[k].setNumber(0);
		timeDigits[k].setLocation(glm::vec2((-0.9f) + ((float)k * 0.1f), DIGIT_LOC_Y));
		timeDigits[k].initialize("../shader/NumberObject/vertex.vs.glsl", "../shader/NumberObject/fragment.fs.glsl");
	}

	#ifdef CGI
	// Point Sprite - Snow
	for (int k = 0; k < POINT_SPRITE_NUM; k++)
		pointSprite_snow_list[k].pointSpriteInit("../shader/PointSprite/Point_Sprite.vs.glsl", "../shader/PointSprite/Point_Sprite.fs.glsl", (((k & 1) == 1) ? "../media/snow.png" : "../media/snowflake.png"), (float)k);

	// Framebuffer
	fboChoice = 0;
	fboManager->initFrameBufferData("../shader/FBO/moreblack_fog.vs.glsl", "../shader/FBO/moreblack_fog.fs.glsl");
	#endif // CGI

	isMissionCompleted = false;
}
// GLUT callback Functions
// Display Function
void My_Display() {
	//std::cout << "state: " << stateSymbols[marco.state] << std::endl;
	//std::cout << "display mode: " << displayMode << std::endl;
	//std::cout << "marco map loc: " << marco.getMapLocationX(scene.getPosX()) << std::endl;
	//std::cout << "scene loc: " << scene.getPosX() << std::endl;

	if (marco.blinkingCounter >= 0)
		marco.blinkingCounter++;

	if (marco.blinkingCounter >= BLINK_TIME) {
		marco.blinkingCounter = -1;
		marco.noDamageStatus = false;
	}

	#ifdef CGI
	if(fboChoice != FBOChoice::NONE)
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

	// not completed, still playing
	if (!isMissionCompleted) {
		// Pointsprite render
		#ifdef CGI
		for (int k = 0; k < POINT_SPRITE_NUM; k++)
			pointSprite_snow_list[k].render(proj_matrix);
		#endif // CGI

		// =================================================

		// Enemy render
		for (vector<Enemy>::iterator it_e = enemyList.begin(); it_e != enemyList.end(); it_e++) {
			it_e->setScreenPosX(scene.getPosX());
			bool replay = false;

			if (fabs(it_e->getScreenPosX()) > 1.2)
				continue;

			// killed
			if (it_e->isDieing) {
				if (it_e->side == Side::LSIDE)
					it_e->deathLeft();
				else
					it_e->deathRight();

				bool isAnimationEnd;
				if (it_e->deathType == Death::SHOT)
					isAnimationEnd = it_e->nextFrame(15);
				else if (it_e->deathType == Death::STAB)
					isAnimationEnd = it_e->nextFrame(16);
				else if (it_e->deathType == Death::BOMB)
					isAnimationEnd = it_e->nextFrame(10);

				if (isAnimationEnd) {
					remainEnemyNum--;
					enemyList.erase(it_e);

					break;
				}

				continue;
			}

			// enemy attack
			if (it_e->step == 0) {
				it_e->state = EnemyState::E_ATTACK;

				if (it_e->type == Arms::DALLY) {
					if (it_e->step == 0) {
						if (it_e->side == LSIDE) {
							it_e->dallyLeft();
							replay = it_e->nextFrame(28 * 2);
						}
						else if (it_e->side == RSIDE) {
							it_e->dallyRight();
							replay = it_e->nextFrame(28 * 2);
						}
					}
				}
				else if (it_e->type == Arms::PISTOL) {
					if (it_e->side == LSIDE) {
						if (it_e->getSpriteId() == 6) {
							EnemyAttack temp(EAttack::BLINK, Orientation::R, it_e->getMapLocationX(), it_e->getMapLocationY());
							temp.initialize("../shader/EnemyAttack/vertex.vs.glsl", "../shader/EnemyAttack/fragment.fs.glsl");
							temp.initSpriteIndex();
							eAttack.push_back(temp);
						}
						it_e->pistolLeft();
						replay = it_e->nextFrame(12 * 2);
					}
					else if (it_e->side == RSIDE) {
						if (it_e->getSpriteId() == 6) {
							EnemyAttack temp(EAttack::BLINK, Orientation::L, it_e->getMapLocationX(), it_e->getMapLocationY());
							temp.initialize("../shader/EnemyAttack/vertex.vs.glsl", "../shader/EnemyAttack/fragment.fs.glsl");
							temp.initSpriteIndex();
							eAttack.push_back(temp);
						}
						it_e->pistolRight();
						replay = it_e->nextFrame(12 * 2);
					}
				}
				else if (it_e->type == Arms::SABER) {
					if (it_e->side == LSIDE) {
						it_e->saberLeft();
						replay = it_e->nextFrame(23 * 2);
					}
					else if (it_e->side == RSIDE) {
						it_e->saberRight();
						replay = it_e->nextFrame(23 * 2);
					}
				}
				if (replay) {
					it_e->initSpriteIndex();
					it_e->step++;
				}
			}

			// enemy idle
			else {
				it_e->state = EnemyState::E_IDLE;

				if (it_e->side == LSIDE) {
					it_e->restLeft();
					replay = it_e->nextFrame(7 * 2);
				}
				else if (it_e->side == RSIDE) {
					it_e->restRight();
					replay = it_e->nextFrame(7 * 2);
				}
				if (replay) {
					it_e->initSpriteIndex();
					it_e->step++;
					if (it_e->step > 3)
						it_e->step = 0;
				}
			}
		}

		// =================================================

		// Clash tesT
		float gapX, gapY;

		// loop for marco attacks
		for (vector<MarcoAttack>::iterator it_m = mAttack.begin(); it_m != mAttack.end(); it_m++) {

			// loop for enemies
			for (vector<Enemy>::iterator it_e = enemyList.begin(); it_e != enemyList.end(); it_e++) {
				// not on screen currently or already killed
				if (fabs(it_e->getScreenPosX()) > 1.2f || it_e->isDieing)
					continue;

				gapX = fabs(it_m->getMapLocationX() - it_e->getMapLocationX());
				gapY = fabs(it_m->getMapLocationY() - it_e->getMapLocationY());

				if (it_m->attack == MAttack::BULLET && gapX < MarcoAttack::BULLET_ATTACK_GAP_X && gapY < MarcoAttack::BULLET_ATTACK_GAP_Y) {
					it_m->action = Action::EXPLODE;
					it_e->killed(Death::SHOT);
				}

				// pineapple explosion detect (not exploded)
				if (it_m->attack == MAttack::PINEAPPLE && gapX < MarcoAttack::GRENADE_ATTACK_GAP_X && gapY < MarcoAttack::GRENADE_ATTACK_GAP_Y && !it_e->isDieing) {
					it_m->initSpriteIndex();
					it_m->action = Action::EXPLODE;
				}
			
				// pinapple damage detect (exploding)
				if (it_m->attack == MAttack::PINEAPPLE && it_m->action == Action::EXPLODE && !it_e->isDieing) {
					gapX = fabs(it_m->getMapLocationX() - it_e->getMapLocationX());
					if (gapX < MarcoAttack::GRENADE_ATTACK_RANGE)
						it_e->killed(Death::BOMB);
				}
			}
		}

		// loop for knife attacks (mutully)
		for (vector<Enemy>::iterator it_e = enemyList.begin(); it_e != enemyList.end(); it_e++) {
			if (fabs(it_e->getScreenPosX()) > 1.2f || it_e->isDieing)
				continue;
			
			// marco knife attack
			gapX = it_e->getMapLocationX() - marco.getMapLocationX();
			if ((marco.direction == Direction::RIGHT && gapX >= 0.0f && fabs(gapX) < MarcoAttack::KNIFE_ATTACK_GAP_RIGHT) ||
				(marco.direction == Direction::LEFT && gapX <= 0.0f && fabs(gapX) < MarcoAttack::KNIFE_ATTACK_GAP_LEFT)) {

				if(marco.state == State::KNIFE)
					it_e->killed(Death::STAB);
			}

			// enemies knife attack
			gapX = it_e->getMapLocationX() - marco.getMapLocationX();
			gapY = it_e->getMapLocationY() - marco.getMapLocationY();
			if (it_e->type == Arms::SABER && it_e->state == EnemyState::E_ATTACK && !it_e->isDieing && it_e->getSpriteId() >= 20 && it_e->getSpriteId() <= 22 && !marco.isDieing && !marco.noDamageStatus) {
				if ((marco.direction == Direction::LEFT && it_e->side == Side::RSIDE && fabs(gapX) < EnemyAttack::KNIFE_ATTACK_GAP_X_BIGGER) ||
					(marco.direction == Direction::LEFT && it_e->side == Side::LSIDE && fabs(gapX) < EnemyAttack::KNIFE_ATTACK_GAP_X_SMALLER) ||
					(marco.direction == Direction::RIGHT && it_e->side == Side::RSIDE && fabs(gapX) < EnemyAttack::KNIFE_ATTACK_GAP_X_SMALLER) ||
					(marco.direction == Direction::RIGHT && it_e->side == Side::LSIDE && fabs(gapX) < EnemyAttack::KNIFE_ATTACK_GAP_X_BIGGER)) {
					if (fabs(gapY) < EnemyAttack::KNIFE_ATTACK_GAP_Y && marco.state != State::JUMP) {
						puts("marco got knife attack");

						marco.initSpriteIndex();
						marco.isDieing = true;

						if(it_e->getMapLocationX() > marco.getMapLocationX())
							marco.state = State::DEATH_WHICH_ATTACK_FROM_RIGHT;
						else
							marco.state = State::DEATH_WHICH_ATTACK_FROM_LEFT;
					}
				}
			}
		}

		// loop for enemy attacks
		for (vector<EnemyAttack>::iterator it_ea = eAttack.begin(); it_ea != eAttack.end(); it_ea++) {

			gapX = fabs(it_ea->getMapLocationX() - marco.getMapLocationX());
			gapY = fabs(it_ea->getMapLocationY() - 0.25 - marco.getMapLocationY());

			if (it_ea->attack == EAttack::BLINK && gapX < EnemyAttack::BULLET_ATTACK_GAP_X && gapY < EnemyAttack::BULLET_ATTACK_GAP_Y && !marco.isDieing && !marco.noDamageStatus) {
				it_ea->action = Action::EXPLODE;

				marco.initSpriteIndex();
				marco.isDieing = true;

				if (it_ea->orient == Orientation::L)
					marco.state = State::DEATH_WHICH_ATTACK_FROM_RIGHT;
				else
					marco.state = State::DEATH_WHICH_ATTACK_FROM_LEFT;
			}
		}

		// =================================================

		// MarcoAttack render
		vector<int> killAttack;
		//cout << mAttack.size() << endl;
		for (int ma = 0; ma < mAttack.size(); ma++) {
			if (mAttack[ma].action == FLY) {
				if (mAttack[ma].attack == BULLET) {
					if (mAttack[ma].orient == L) {
						mAttack[ma].bulletFlyLeft(scene.getPosX());
					}
					else if (mAttack[ma].orient == R) {
						mAttack[ma].bulletFlyRight(scene.getPosX());
					}
					else if (mAttack[ma].orient == U) {
						mAttack[ma].bulletFlyUp(scene.getPosX());
					}
					mAttack[ma].nextFrame();
				}
				else if (mAttack[ma].attack == PINEAPPLE) {
					if (mAttack[ma].orient == L) {
						mAttack[ma].pineappleFlyLeft(scene.getPosX());
					}
					else if (mAttack[ma].orient == R) {
						mAttack[ma].pineappleFlyRight(scene.getPosX());
					}
					mAttack[ma].nextFrame();
				}
			}
			else if (mAttack[ma].action == EXPLODE) {
				bool exploded = false;
				if (mAttack[ma].attack == BULLET) {
					mAttack[ma].bulletExplode(scene.getPosX());
				}
				else if (mAttack[ma].attack == PINEAPPLE) {
					mAttack[ma].pineappleExplode(scene.getPosX());
				}
				exploded = mAttack[ma].nextFrame();
				if (exploded == true) {
					killAttack.push_back(ma);
				}
			}
		}
		// Kill finish attack
		for (int kma = killAttack.size() - 1; kma >= 0; kma--) {
			cout << "delete\n";
			mAttack.erase(mAttack.begin() + killAttack[kma]);
		}

		// =================================================

		// Marco Action
		bool isFinished = false;

		glm::vec2 currentPosition;
		currentPosition = marco.getMapLocation();

		// Left
		if (marco.getKeyboard(GLUT_KEY_LEFT) && !marco.isDieing) {
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
					marco.walkLeft(scene.getPosX());
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
		else if (marco.getKeyboard(GLUT_KEY_RIGHT) && !marco.isDieing) {
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
					marco.walkRight(scene.getPosX());
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
		else if (marco.getKeyboard(GLUT_KEY_UP) && marco.state != State::GUNUP && !marco.isDieing) {
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
			else if (marco.state == State::GUNUP) {
			}
			else if (marco.state == State::GRENADE) {

			}
			else if (marco.state == State::DEATH_WHICH_ATTACK_FROM_LEFT || marco.state == State::DEATH_WHICH_ATTACK_FROM_RIGHT) {}

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
				// Initial bullet left
				if (marco.getSpriteId() == 3) {
					MarcoAttack temp(MAttack::BULLET, Orientation::L, currentPosition.x, currentPosition.y);
					temp.initialize("../shader/Attack/vertex.vs.glsl", "../shader/Attack/fragment.fs.glsl");
					temp.initSpriteIndex();
					mAttack.push_back(temp);
				}
				marco.shootLeft();
				isFinished = marco.nextFrame(12);
			}
			else if (marco.direction == RIGHT) {
				// Initial bullet right
				if (marco.getSpriteId() == 3) {
					MarcoAttack temp(MAttack::BULLET, Orientation::R, currentPosition.x, currentPosition.y);
					temp.initialize("../shader/Attack/vertex.vs.glsl", "../shader/Attack/fragment.fs.glsl");
					temp.initSpriteIndex();
					mAttack.push_back(temp);
				}
				marco.shootRight();
				isFinished = marco.nextFrame(12);
			}
			if (isFinished) {
				marco.state = State::IDLE;
				marco.initSpriteIndex();
			}
		}

		// Gun up Attack
		if (marco.state == State::GUNUP) {
			if (marco.direction == LEFT) {
				marco.shootUpLeft();
				isFinished = marco.nextFrame(12);
			}
			else if (marco.direction == RIGHT) {
				marco.shootUpRight();
				isFinished = marco.nextFrame(12);
			}
			// Initial bullet up
			if (marco.getSpriteId() == 3) {
				MarcoAttack temp(MAttack::BULLET, Orientation::U, currentPosition.x, currentPosition.y);
				temp.initialize("../shader/Attack/vertex.vs.glsl", "../sha\der/Attack/fragment.fs.glsl");
				mAttack.push_back(temp);
			}
			if (isFinished) {
				marco.state = State::IDLEUP;
				marco.initSpriteIndex();
			}
			else
				marco.state = State::GUNUP;
		}

		// Grenade attack
		if (marco.state == State::GRENADE) {
			if (marco.direction == LEFT) {
				// Inital pineapple left
				if (marco.getSpriteId() == 2) {
					MarcoAttack temp(MAttack::PINEAPPLE, Orientation::L, currentPosition.x, currentPosition.y);
					temp.initialize("../shader/Attack/vertex.vs.glsl", "../shader/Attack/fragment.fs.glsl");
					temp.initSpriteIndex();
					mAttack.push_back(temp);
				}
				marco.grenadeLeft();
				isFinished = marco.nextFrame(7);
			}
			else if (marco.direction == RIGHT) {
				if (marco.getSpriteId() == 2) {
					// Inital pineapple right
					cout << "MapPositionX: " << currentPosition.x << endl;
					MarcoAttack temp(MAttack::PINEAPPLE, Orientation::R, currentPosition.x, currentPosition.y);
					temp.initialize("../shader/Attack/vertex.vs.glsl", "../shader/Attack/fragment.fs.glsl");
					temp.initSpriteIndex();
					mAttack.push_back(temp);
				}
				marco.grenadeRight();
				isFinished = marco.nextFrame(7);
			}
			if (isFinished) {
				marco.state = State::IDLE;
				marco.initSpriteIndex();
			}
		}

		// Marco death
		if (marco.isDieing) {
			if (marco.state == State::DEATH_WHICH_ATTACK_FROM_LEFT) {
				marco.deathLeft();
				isFinished = marco.nextFrame(10);
			}
			else if (marco.state == State::DEATH_WHICH_ATTACK_FROM_RIGHT) {
				marco.deathRight();
				isFinished = marco.nextFrame(10);
			}

			if (isFinished) {
				puts("marco killed");
				marco.reborn();
				//scene.moveBG2Head();

				mAttack.clear();
				eAttack.clear();
			}
		}

		// =================================================

		// EnemyAttack render
		killAttack.clear();
		for (int ea = 0; ea < eAttack.size(); ea++) {
			if (eAttack[ea].action == FLY) {
				if (eAttack[ea].attack == BLINK) {
					if (eAttack[ea].orient == L) {
						eAttack[ea].blinkFlyLeft(scene.getPosX());
					}
					else if (eAttack[ea].orient == R) {
						eAttack[ea].blinkFlyRight(scene.getPosX());
					}
					eAttack[ea].nextFrame();
				}
			}
			else if (eAttack[ea].action == EXPLODE) {
				bool exploded = false;
				if (eAttack[ea].attack == BLINK) {
					eAttack[ea].bulletExplode(scene.getPosX());
				}
				exploded = eAttack[ea].nextFrame();
				if (exploded == true) {
					killAttack.push_back(ea);
				}
			}
		}
		// Kill finish enemy attack
		for (int kea = killAttack.size() - 1; kea >= 0; kea--) {
			cout << "delete\n";
			eAttack.erase(eAttack.begin() + killAttack[kea]);
		}
	}

	// =================================================

	if (isMissionCompleted) {
		for (int k = MAX_TIME_DIGIT - 1, tmpTime = currentTime; k >= 0; k--, tmpTime /= 10) {
			timeDigits[k].setNumber(tmpTime % 10);
			timeDigits[k].render();
		}
	}

	#ifdef CGI
	if (fboChoice != FBOChoice::NONE) {
		fboManager->disable();
		fboManager->effectRender();
	}
	#endif // CGI

	// =================================================

	// Number render (number of remained enemies)
	if (!isMissionCompleted) {
		digitOne.setNumber(remainEnemyNum % 10);
		digitOne.render();
		digitTen.setNumber(remainEnemyNum / 10);
		digitTen.render();
		for (int k = MAX_TIME_DIGIT - 1, tmpTime = currentTime; k >= 0; k--, tmpTime /= 10) {
			timeDigits[k].setNumber(tmpTime % 10);
			timeDigits[k].render();
		}
	}

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

				// to choose using gun or knife
				marco.state = State::GUN;
				for (int k = 0; k < enemyList.size() && !(marco.getKeyboard(GLUT_KEY_UP)); k++) {
					float gap = enemyList[k].getMapLocationX() - marco.getMapLocationX();

					if ((marco.direction == Direction::RIGHT && gap >= 0.0f && fabs(gap) < MarcoAttack::KNIFE_ATTACK_GAP_RIGHT) ||
						(marco.direction == Direction::LEFT && gap <= 0.0f && fabs(gap) < MarcoAttack::KNIFE_ATTACK_GAP_LEFT)) {
						marco.state = State::KNIFE;
						break;
					}
				}

				marco.initSpriteIndex();
			}
			else if (marco.state == State::IDLEUP) {
				marco.setKeyboard(key, true);
				marco.state = State::GUNUP;
				marco.initSpriteIndex();
			}
			break;
		case 'x':
		case 'X':
			if (marco.state == State::IDLE || marco.state == State::WALK) {
				marco.setKeyboard(key, true);
				marco.state = State::GRENADE;
				marco.initSpriteIndex();
			}
			break;

		case 'm':
		case 'M':
			std::cout << "marco map loc: " << marco.getMapLocationX() << std::endl;
			std::cout << "marco screen loc: " << marco.getScreenPosX() << std::endl;
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

void My_Counter(int val) {

	// defeat all enemies
	if (remainEnemyNum == 0) {
		if (val == 0) {
			fboChoice = FBOChoice::ENDING;
			fboManager->initFrameBufferData("../shader/FBO/ending.vs.glsl", "../shader/FBO/ending.fs.glsl");
			fboManager->createRenderbufferAndTexture4Framebuffer();

			glutSetMenu(eventMenu);
			glutDetachMenu(GLUT_RIGHT_BUTTON);
		}

		if (fboManager->isAnimationEnd()) {
			if (!isMissionCompleted) {
				scene.enterEndingPhase();
				fboChoice = FBOChoice::SHOW_MISSION_COMPLETE;
				fboManager->initFrameBufferData("../shader/FBO/missioncomplete.vs.glsl", "../shader/FBO/missioncomplete.fs.glsl");
				fboManager->createRenderbufferAndTexture4Framebuffer();
				isMissionCompleted = true;

				for (int k = 0; k < MAX_TIME_DIGIT; k++)
					timeDigits[k].setLocation(glm::vec2((-0.17f) + ((float)k * 0.1f), -0.7f));
				digitOne.setLocation(glm::vec2(0.0f, -10.0f));
				digitTen.setLocation(glm::vec2(0.0f, -10.0f));

				glutTimerFunc(1000, My_Counter, 1);
			}
			else {
				fboChoice = FBOChoice::NONE;
				glutSetMenu(restartMenu);
				glutAttachMenu(GLUT_RIGHT_BUTTON);
			}
		}
		else
			glutTimerFunc(1000, My_Counter, 1);
	}

	else {
		currentTime++;
		glutTimerFunc(1000, My_Counter, 0);
	}

	return;
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
		// Normal mode
		case 9999:
			displayMode = DisplayMode::NORMAL;
			remainEnemyNum = Enemy::enemyNum;
			break;

		// One enemy mode
		case 9998:
			displayMode = DisplayMode::ONE_ENEMY;
			Enemy::enemyNum = remainEnemyNum;
			remainEnemyNum = 1;
			break;

		default:
			displayMode = DisplayMode::NORMAL;
			break;
	}

	return;
}

void restartMenuEvents(int option) {
	switch (option) {
		case 2147:
			// restart
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

void fboChoiceMenuEvents(int option) {
	fboChoice = option;

	switch (option) {
		case FBOChoice::MORE_BLACK_WITH_FOG:
			fboManager->initFrameBufferData("../shader/FBO/moreblack_fog.vs.glsl", "../shader/FBO/moreblack_fog.fs.glsl");
			break;

		case FBOChoice::MORE_BLACK:
			fboManager->initFrameBufferData("../shader/FBO/moreblack.vs.glsl", "../shader/FBO/moreblack.fs.glsl");
			break;

		case FBOChoice::FOG:
			fboManager->initFrameBufferData("../shader/FBO/fog.vs.glsl", "../shader/FBO/fog.fs.glsl");
			break;

		case FBOChoice::MARCO_DEATH:
			fboManager->initFrameBufferData("../shader/FBO/gray.vs.glsl", "../shader/FBO/gray.fs.glsl");
			break;

		case FBOChoice::ENDING:
			remainEnemyNum = 0;
			fboManager->initFrameBufferData("../shader/FBO/ending.vs.glsl", "../shader/FBO/ending.fs.glsl");
			break;

		default:
			fboManager->initFrameBufferData("../shader/FBO/moreblack_fog.vs.glsl", "../shader/FBO/moreblack_fog.fs.glsl");
			break;
	}
	fboManager->createRenderbufferAndTexture4Framebuffer();

	return;
}