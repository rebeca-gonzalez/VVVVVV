#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Belt.h"
#include "Checkpoint.h"
#include "Player.h"
#include "Enemy.h"
#include "GhostHor.h"
#include "SkeletonHor.h"
#include "StopHor.h"
#include "SpinHor.h"
#include "WheelHor.h"
#include "WheelVer.h"
#include "NumbersHor.h"
#include "GhostVer.h"
#include "SoldierVer.h"
#include "SoldierHor.h"
#include "BowtieHor.h"
#include "YesHor.h"
#include "PlatformBlueHor.h"
#include "PlatformGreenHor.h"
#include "PlatformBlueVer.h"
#include "TextureQuad.h"
#include "SceneState.h"



// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	//enum State { MENU, LEVEL1, LEVEL2, INSTRUCTIONS, CREDITS };

	void init();
	void update(int deltaTime);
	void render();
	void setState(SceneState::State st);

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	vector<Enemy *> enemies;
	vector<Belt *> belts;
	vector<Enemy *> platforms;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	void createEnemy(Enemy* enemy, int x, int y);
	void createPlatform(Enemy* platform, int x, int y);
	void spawnPlayer();
	void spawnEnemies();
	int max(int a, int b);
	int abs(int a);
	glm::ivec2 move_camera;
	glm::ivec2 pos_clock;

	SceneState::State state;
	bool spawned;

	Texture texs[6];
	TextureQuad *texQuad, *texQuadReloj;
};


#endif // _SCENE_INCLUDE

