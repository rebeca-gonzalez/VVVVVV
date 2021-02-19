#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES_LEVEL1 3
#define INIT_PLAYER_Y_TILES_LEVEL1 26

#define INIT_PLAYER_X_TILES_LEVEL2 43
#define INIT_PLAYER_Y_TILES_LEVEL2 86


#define GHOST_HOR_SIZE 4
#define GHOST_VER_SIZE 1
#define STOP_HOR_SIZE 2
#define SKELETON_HOR_SIZE 3
#define SOLDIER_VER_SIZE 1
#define SOLDIER_HOR_SIZE 6
#define BOWTIE_HOR_SIZE 4
#define YES_HOR_SIZE 3
#define SPIN_HOR_SIZE 10
#define WHEEL_HOR_SIZE 2
#define WHEEL_VER_SIZE 3
#define NUMBERS_HOR_SIZE 8
#define ENEMIES_SIZE GHOST_HOR_SIZE+GHOST_VER_SIZE+STOP_HOR_SIZE+SKELETON_HOR_SIZE+SOLDIER_VER_SIZE+SOLDIER_HOR_SIZE+BOWTIE_HOR_SIZE+YES_HOR_SIZE+SPIN_HOR_SIZE+WHEEL_HOR_SIZE+NUMBERS_HOR_SIZE+WHEEL_VER_SIZE

#define PLATFORMS_BLUE_SIZE_HOR 5
#define PLATFORMS_GREEN_SIZE_HOR 3
#define PLATFORMS_SPEED 2

Scene::Scene()
{
	map = NULL;
	player = NULL;
	state = SceneState::MENU;
	spawned = false;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/tiled2.csv", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, belts);
	Checkpoint::instance().init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

		glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
		glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		texQuad = TextureQuad::createTextureQuad(geom, texCoords, texProgram);
		texs[0].loadFromFile("images/menu_principal.png", TEXTURE_PIXEL_FORMAT_RGBA);
		texs[0].setMagFilter(GL_NEAREST);
		texs[1].loadFromFile("images/instrucciones.png", TEXTURE_PIXEL_FORMAT_RGBA);
		texs[1].setMagFilter(GL_NEAREST);
		texs[2].loadFromFile("images/creditos.png", TEXTURE_PIXEL_FORMAT_RGBA);
		texs[2].setMagFilter(GL_NEAREST);
		texs[3].loadFromFile("images/jugar.png", TEXTURE_PIXEL_FORMAT_RGBA);
		texs[3].setMagFilter(GL_NEAREST);
		texs[4].loadFromFile("images/victoria.png", TEXTURE_PIXEL_FORMAT_RGBA);
		texs[4].setMagFilter(GL_NEAREST);

		glm::vec2 geom2[2] = { glm::vec2(0.f, 0.f), glm::vec2(27.f, 37.f) };
		texs[5].loadFromFile("images/reloj.png", TEXTURE_PIXEL_FORMAT_RGBA);
		texs[5].setMagFilter(GL_NEAREST);
		texQuadReloj = TextureQuad::createTextureQuad(geom2, texCoords, texProgram);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::spawnPlayer() {
	if (state == SceneState::LEVEL1 || state == SceneState::LEVEL2) {
		player = new Player();
		glm::vec2 pos;

		if (state == SceneState::LEVEL1) {
			pos = glm::vec2(INIT_PLAYER_X_TILES_LEVEL1, INIT_PLAYER_Y_TILES_LEVEL1);
			pos_clock = glm::ivec2(0.f, 0.f);
		}
		else {
			pos = glm::vec2(INIT_PLAYER_X_TILES_LEVEL2, INIT_PLAYER_Y_TILES_LEVEL2);
			pos_clock = glm::ivec2(1, 2);
		}

		player->init(glm::ivec2(SCREEN_X, SCREEN_Y), glm::vec2(pos.x, pos.y), texProgram);
		player->setPosition(glm::vec2(pos.x * map->getTileSize(), pos.y * map->getTileSize()));
		player->setTileMap(map);
	}
}

void Scene::spawnEnemies() {
	if (!spawned) {
		spawned = true;
		for (int i = 0; i < GHOST_HOR_SIZE; ++i) {
			GhostHor *enemy = new GhostHor();
			if (i < 2) createEnemy(enemy, 5, (i + 1) * 5);
			else {
				if (i == 2) createEnemy(enemy, 20, 22);
				else createEnemy(enemy, 30, 25);
			}
		}
		for (int i = 0; i < GHOST_VER_SIZE; ++i) {
			GhostVer *enemy = new GhostVer();
			createEnemy(enemy, 10, 10);
		}
		for (int i = 0; i < SKELETON_HOR_SIZE; ++i) {
			SkeletonHor *enemy = new SkeletonHor();
			if (i == 0) createEnemy(enemy, 42, 23);
			else if (i == 1) createEnemy(enemy, 59, 10);
			else if (i == 2) createEnemy(enemy, 70, 10);
		}

		for (int i = 0; i < STOP_HOR_SIZE; ++i) {
			StopHor *enemy = new StopHor();
			if (i == 0) createEnemy(enemy, 55, 52);
			else if (i == 1) createEnemy(enemy, 61, 39);
		}

		for (int i = 0; i < SOLDIER_VER_SIZE; ++i) {
			SoldierVer *enemy = new SoldierVer();
			if (i == 0) createEnemy(enemy, 34, 34);
		}

		for (int i = 0; i < SOLDIER_HOR_SIZE; ++i) {
			SoldierHor *enemy = new SoldierHor();
			if (i == 0) createEnemy(enemy, 28, 51);
			else if (i == 1) createEnemy(enemy, 5, 57);
			else if (i == 2) createEnemy(enemy, 2, 31);
			else if (i == 3) createEnemy(enemy, 2, 33);
			else if (i == 4) createEnemy(enemy, 6, 31);
			else if (i == 5) createEnemy(enemy, 6, 33);
		}

		for (int i = 0; i < BOWTIE_HOR_SIZE; ++i) {
			BowtieHor *enemy = new BowtieHor();
			if (i == 0) createEnemy(enemy, 5, 69);
			else if (i == 1) createEnemy(enemy, 34, 63);
			else if (i == 2) createEnemy(enemy, 5, 75);
			else if (i == 3) createEnemy(enemy, 34, 81);
		}

		for (int i = 0; i < YES_HOR_SIZE; ++i) {
			YesHor *enemy = new YesHor();
			if (i == 0) createEnemy(enemy, 49, 78);
			else if (i == 1) createEnemy(enemy, 68, 73);
			else if (i == 2) createEnemy(enemy, 76, 84);
		}

		for (int i = 0; i < SPIN_HOR_SIZE; ++i) {
			SpinHor *enemy = new SpinHor();
			if (i == 0) createEnemy(enemy, 89, 62);
			else if (i == 1) createEnemy(enemy, 91, 68);
			else if (i == 2) createEnemy(enemy, 114, 68);
			else if (i == 3) createEnemy(enemy, 92, 83);
			else if (i == 4) createEnemy(enemy, 116, 78);
			else if (i == 5) createEnemy(enemy, 82, 83);
			else if (i == 6) createEnemy(enemy, 82, 79);
			else if (i == 7) createEnemy(enemy, 82, 75);
			else if (i == 8) createEnemy(enemy, 82, 71);
			else if (i == 9) createEnemy(enemy, 82, 67);
		}

		for (int i = 0; i < WHEEL_HOR_SIZE; ++i) {
			WheelHor *enemy = new WheelHor();
			if (i == 0) createEnemy(enemy, 125, 85);
			else if (i == 1) createEnemy(enemy, 128, 68);
		}

		for (int i = 0; i < WHEEL_VER_SIZE; ++i) {
			WheelVer *enemy = new WheelVer();
			if (i == 0) createEnemy(enemy, 134, 75);
			else if (i == 1) createEnemy(enemy, 131, 80);
			else if (i == 2) createEnemy(enemy, 128, 75);
		}

		for (int i = 0; i < NUMBERS_HOR_SIZE; ++i) {
			NumbersHor *enemy = new NumbersHor();
			if (i == 0) createEnemy(enemy, 91, 35);
			else if (i == 1) createEnemy(enemy, 95, 35);
			else if (i == 2) createEnemy(enemy, 101, 31);
			else if (i == 3) createEnemy(enemy, 86, 54);
			else if (i == 4) createEnemy(enemy, 90, 54);
			else if (i == 5) createEnemy(enemy, 94, 54);
			else if (i == 6) createEnemy(enemy, 93, 43);
			else if (i == 7) createEnemy(enemy, 114, 45);
		}

		for (int i = 0; i < PLATFORMS_BLUE_SIZE_HOR; ++i) {
			PlatformBlueHor *platform = new PlatformBlueHor(PLATFORMS_SPEED);
			if (i == 0) createPlatform(platform, 20, 10);
			else if (i == 1) createPlatform(platform, 8, 70);
			else if (i == 2) createPlatform(platform, 8, 63);
			else if (i == 3) createPlatform(platform, 8, 75);
			else if (i == 4) createPlatform(platform, 8, 82);
		}

		for (int i = 0; i < PLATFORMS_GREEN_SIZE_HOR; ++i) {
			PlatformGreenHor *platform = new PlatformGreenHor(PLATFORMS_SPEED);
			if (i == 0) createPlatform(platform, 46, 67);
			else if (i == 1) createPlatform(platform, 124, 43);
			else if (i == 2) createPlatform(platform, 100, 56);
		}
	}
}

void Scene::createEnemy(Enemy* enemy, int x, int y) {
	enemies.push_back(enemy);
	enemy->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	enemy->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
	enemy->setTileMap(map);
}

void Scene::createPlatform(Enemy* platform, int x, int y) {
	platforms.push_back(platform);
	platform->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	platform->setPosition(glm::vec2(x * map->getTileSize(), y * map->getTileSize()));
	platform->setTileMap(map);
}

int Scene::abs(int a) {
	if (a > 0) return a;
	return a * -1;
}

int Scene::max(int a, int b){
	if (abs(a) > abs(b)) return a;
	else return b;
}

void Scene::setState(SceneState::State st) {
	bool load = false;
	Checkpoint::instance().setPosition(glm::ivec2(-16.f, -16.f*map->getTileSize()));
	if (st == SceneState::LEVEL2) projection = glm::ortho(640.f*1.f, 640.f*1.f + float(SCREEN_WIDTH - 1), 480.f*2.f + float(SCREEN_HEIGHT - 1), 480.f*2.f);
	else projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	if (state != SceneState::LEVEL1 && state != SceneState::LEVEL2 && (st == SceneState::LEVEL1 || st == SceneState::LEVEL2)) load = true;
	state = st;
	spawnPlayer();
	spawnEnemies();
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (state == SceneState::LEVEL1 || state == SceneState::LEVEL2) {
		move_camera = player->update(deltaTime);
		if (move_camera.x != -1) {
			float x = move_camera.x;
			float y = move_camera.y;
			pos_clock.x = x;
			pos_clock.y = y;
			projection = glm::ortho(640.f*x, 640.f*x + float(SCREEN_WIDTH - 1), 480.f*y + float(SCREEN_HEIGHT - 1), 480.f*y);
		}
		int collision_enemy = 0;
		for (int i = 0; i < platforms.size(); ++i) {
			collision_enemy = max(platforms[i]->update(deltaTime, player->getUpperPosition(), player->getLowerPosition(), player->getDir()), collision_enemy);
		}
		if (collision_enemy != 0 && collision_enemy != 1) {
			player->movingPlatform(collision_enemy, PLATFORMS_SPEED);
		}
		collision_enemy = 0;
		bool za_warudo = player->getZaWarudo();
		for (int i = 0; i < ENEMIES_SIZE; ++i) {
			collision_enemy = max(enemies[i]->update(deltaTime, player->getUpperPosition(), player->getLowerPosition(), za_warudo), collision_enemy);
		}
		if (collision_enemy == 1) player->playerDies();
		for (int i = 0; i < belts.size(); ++i) {
			belts[i]->update(deltaTime);
		}
		Checkpoint::instance().update(deltaTime);
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	if (state == SceneState::LEVEL1 || state == SceneState::LEVEL2) {
		map->render();
		if (player->isZaWarudoAvailable()) {
			modelview = glm::translate(glm::mat4(1.0f), glm::vec3(640.f*pos_clock.x, 480.f*pos_clock.y, 0.f));
			texProgram.setUniformMatrix4f("modelview", modelview);
			texQuadReloj->render(texs[5]);
			modelview = glm::mat4(1.0f);
			texProgram.setUniformMatrix4f("modelview", modelview);
		}
		Checkpoint::instance().render();
		for (int i = 0; i < ENEMIES_SIZE; ++i) {
			enemies[i]->render();
		}
		player->render();
		for (int i = 0; i < belts.size(); ++i) {
			belts[i]->render();
		}
		for (int i = 0; i < platforms.size(); ++i) {
			platforms[i]->render();
		}
	}
	else if (state == SceneState::MENU) {
		texQuad->render(texs[0]);
	}
	else if (state == SceneState::INSTRUCTIONS) {
		texQuad->render(texs[1]);
	}
	else if (state == SceneState::CREDITS) {
		texQuad->render(texs[2]);
	}
	else if (state == SceneState::PLAY) {
		texQuad->render(texs[3]);
	}
	else if (state == SceneState::WIN) {
		texQuad->render(texs[4]);
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

