#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define PLAYER_SPEED 3
#define BELT_SPEED 2
#define DEATH_SPEED 0.5f
#define LEVEL_SIZE_X (40*16)
#define LEVEL_SIZE_Y (30*16)

#define SOUNDS_VOLUME 0.8f

enum PlayerAnims
{
	STAND_LEFT_DOWN, STAND_RIGHT_DOWN, STAND_LEFT_UP, STAND_RIGHT_UP,
	MOVE_LEFT_DOWN, MOVE_RIGHT_DOWN, MOVE_LEFT_UP, MOVE_RIGHT_UP,
	DEAD_LEFT_DOWN, DEAD_RIGHT_DOWN, DEAD_LEFT_UP, DEAD_RIGHT_UP,
};


void Player::init(const glm::ivec2 &tileMapPos, glm::vec2 playerIniPos, ShaderProgram &shaderProgram)
{
	falling = false;
	death_in_progress = false;
	on_moving_platform = false;
	za_warudo = true;
	deathTime = -1;
	inmortal = false;
	SoundEngineZaWarudo = createIrrKlangDevice();
	SoundEngineZaWarudo->setSoundVolume(1.0f);

	SoundEngineSave = createIrrKlangDevice();
	SoundEngineJump = createIrrKlangDevice();
	SoundEngineDeath = createIrrKlangDevice();
	SoundEngineSave->setSoundVolume(SOUNDS_VOLUME);
	SoundEngineJump->setSoundVolume(SOUNDS_VOLUME);
	SoundEngineDeath->setSoundVolume(SOUNDS_VOLUME);

	dirY = 1;
	posCheckpoint = glm::ivec2(playerIniPos.x, playerIniPos.y);
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(character_size, glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(12);
	
		sprite->setAnimationSpeed(STAND_LEFT_DOWN, 10);
		sprite->addKeyframe(STAND_LEFT_DOWN, glm::vec2(0.75f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT_DOWN, 10);
		sprite->addKeyframe(STAND_RIGHT_DOWN, glm::vec2(0.f, 0.f));

		sprite->setAnimationSpeed(STAND_LEFT_UP, 10);
		sprite->addKeyframe(STAND_LEFT_UP, glm::vec2(0.25f, 0.5f));

		sprite->setAnimationSpeed(STAND_RIGHT_UP, 10);
		sprite->addKeyframe(STAND_RIGHT_UP, glm::vec2(0.5f, 0.25f));
		
		sprite->setAnimationSpeed(MOVE_LEFT_DOWN, 10);
		sprite->addKeyframe(MOVE_LEFT_DOWN, glm::vec2(0.75f, 0.f));
		sprite->addKeyframe(MOVE_LEFT_DOWN, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT_DOWN, glm::vec2(0.25f, 0.25f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT_DOWN, 10);
		sprite->addKeyframe(MOVE_RIGHT_DOWN, glm::vec2(0., 0.f));
		sprite->addKeyframe(MOVE_RIGHT_DOWN, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT_DOWN, glm::vec2(0.5, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT_UP, 10);
		sprite->addKeyframe(MOVE_LEFT_UP, glm::vec2(0.25f, 0.5f));
		sprite->addKeyframe(MOVE_LEFT_UP, glm::vec2(0.5f, 0.5f));
		sprite->addKeyframe(MOVE_LEFT_UP, glm::vec2(0.75f, 0.5f));

		sprite->setAnimationSpeed(MOVE_RIGHT_UP, 10);
		sprite->addKeyframe(MOVE_RIGHT_UP, glm::vec2(0.5f, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT_UP, glm::vec2(0.75f, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT_UP, glm::vec2(0., 0.5f));

		sprite->setAnimationSpeed(DEAD_RIGHT_DOWN, 10);
		sprite->addKeyframe(DEAD_RIGHT_DOWN, glm::vec2(0.f, 0.75f));

		sprite->setAnimationSpeed(DEAD_RIGHT_UP, 10);
		sprite->addKeyframe(DEAD_RIGHT_UP, glm::vec2(0.5f, 0.75f));

		sprite->setAnimationSpeed(DEAD_LEFT_DOWN, 10);
		sprite->addKeyframe(DEAD_LEFT_DOWN, glm::vec2(0.25f, 0.75f));

		sprite->setAnimationSpeed(DEAD_LEFT_UP, 10);
		sprite->addKeyframe(DEAD_LEFT_UP, glm::vec2(0.75f, 0.75f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

glm::ivec2 Player::update(int deltaTime)
{
	int level_x = posPlayer.x / LEVEL_SIZE_X;
	int level_y = posPlayer.y / LEVEL_SIZE_Y;
	sprite->update(deltaTime);
	if (death_in_progress) {
		if (deathTime == -1) deathTime = glutGet(GLUT_ELAPSED_TIME);
		else {
			if (glutGet(GLUT_ELAPSED_TIME) - deathTime > DEATH_SPEED*1000.f) {
				deathTime = -1;
				executeDeath();
			}
		}
	}
	else {
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			if (falling) {
				if (dirY == 1) sprite->changeAnimation(STAND_LEFT_DOWN);
				else sprite->changeAnimation(STAND_LEFT_UP);
			}
			else if (sprite->animation() != MOVE_LEFT_DOWN && sprite->animation() != MOVE_LEFT_UP) {
				if (dirY == 1) sprite->changeAnimation(MOVE_LEFT_DOWN);
				else sprite->changeAnimation(MOVE_LEFT_UP);
			}
			posPlayer.x -= PLAYER_SPEED;
			if (map->collisionDeadly(posPlayer, character_size, &posPlayer.y)) {
				playerDies();
			}
			if (!death_in_progress) {
				glm::ivec2 checkpoint = map->collisionCheckpoint(posPlayer, character_size);
				if (checkpoint.x != -1) setCheckpoint(checkpoint);
				int bloque_colision = map->collisionMoveLeft(posPlayer, character_size);
				if (bloque_colision == 29) win();
				if (tileBloqueante(bloque_colision))
				{
					posPlayer.x += PLAYER_SPEED;
					if (sprite->animation() == MOVE_LEFT_DOWN || sprite->animation() == STAND_LEFT_DOWN) sprite->changeAnimation(STAND_LEFT_DOWN);
					else sprite->changeAnimation(STAND_LEFT_UP);
				}
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			if (falling) {
				if (dirY == 1) sprite->changeAnimation(STAND_RIGHT_DOWN);
				else sprite->changeAnimation(STAND_RIGHT_UP);
			}
			else if (sprite->animation() != MOVE_RIGHT_DOWN && sprite->animation() != MOVE_RIGHT_UP) {
				if (dirY == 1) sprite->changeAnimation(MOVE_RIGHT_DOWN);
				else sprite->changeAnimation(MOVE_RIGHT_UP);
			}
			posPlayer.x += PLAYER_SPEED;
			if (map->collisionDeadly(posPlayer, character_size, &posPlayer.y)) {
				playerDies();
			}
			if (!death_in_progress) {
				glm::ivec2 checkpoint = map->collisionCheckpoint(posPlayer, character_size);
				if (checkpoint.x != -1) setCheckpoint(checkpoint);
				int bloque_colision = map->collisionMoveRight(posPlayer, character_size);
				if (bloque_colision == 29) win();
				if (tileBloqueante(bloque_colision))
				{
					posPlayer.x -= PLAYER_SPEED;
					if (sprite->animation() == MOVE_RIGHT_DOWN || sprite->animation() == STAND_RIGHT_DOWN) sprite->changeAnimation(STAND_RIGHT_DOWN);
					else sprite->changeAnimation(STAND_RIGHT_UP);
				}
			}
		}
		else
		{
			if (sprite->animation() == MOVE_LEFT_DOWN)
				sprite->changeAnimation(STAND_LEFT_DOWN);
			else if (sprite->animation() == MOVE_RIGHT_DOWN)
				sprite->changeAnimation(STAND_RIGHT_DOWN);
			else if (sprite->animation() == MOVE_LEFT_UP)
				sprite->changeAnimation(STAND_LEFT_UP);
			else if (sprite->animation() == MOVE_RIGHT_UP)
				sprite->changeAnimation(STAND_RIGHT_UP);
		}
		if (!death_in_progress) {
			posPlayer.y += FALL_STEP * dirY;
			if (map->collisionDeadly(posPlayer, character_size, &posPlayer.y)) playerDies();

			int bloque_colision_down = map->collisionMoveDown(posPlayer, character_size, &posPlayer.y, false);
			int bloque_colision_up = map->collisionMoveUp(posPlayer, character_size, &posPlayer.y, false);
			if (bloque_colision_down == 29 || bloque_colision_up == 29) win();
			if (on_moving_platform || tileBloqueante(bloque_colision_down) || tileBloqueante(bloque_colision_up))
			{
				glm::ivec2 checkpoint = map->collisionCheckpoint(posPlayer, character_size);
				if (checkpoint.x!=-1) setCheckpoint(checkpoint);
				if ((bloque_colision_down >= 21 && bloque_colision_down <= 24) || (bloque_colision_up >= 21 && bloque_colision_up <= 24)) posPlayer.x -= BELT_SPEED;
				if ((bloque_colision_down >= 25 && bloque_colision_down <= 28) || (bloque_colision_up >= 25 && bloque_colision_up <= 28)) posPlayer.x += BELT_SPEED;
				if (map->collisionDeadly(posPlayer, character_size, &posPlayer.y)) playerDies();
				falling = false;
				if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !Game::instance().get_up_key())
				{
					SoundEngineJump->play2D("sounds/jump.wav", GL_FALSE);
					dirY *= -1;
					if (on_moving_platform) {
						on_moving_platform = false;
						posPlayer.y += 4*dirY;
						if (map->collisionDeadly(posPlayer, character_size, &posPlayer.y)) playerDies();
					}
					Game::instance().block_up_key();
					switch (sprite->animation()) {
					case STAND_LEFT_DOWN:
						sprite->changeAnimation(STAND_LEFT_UP);
						break;
					case STAND_RIGHT_DOWN:
						sprite->changeAnimation(STAND_RIGHT_UP);
						break;
					case STAND_LEFT_UP:
						sprite->changeAnimation(STAND_LEFT_DOWN);
						break;
					case STAND_RIGHT_UP:
						sprite->changeAnimation(STAND_RIGHT_DOWN);
						break;
					case MOVE_LEFT_DOWN:
						sprite->changeAnimation(STAND_LEFT_UP);
						break;
					case MOVE_LEFT_UP:
						sprite->changeAnimation(STAND_LEFT_DOWN);
						break;
					case MOVE_RIGHT_DOWN:
						sprite->changeAnimation(STAND_RIGHT_UP);
						break;
					case MOVE_RIGHT_UP:
						sprite->changeAnimation(STAND_RIGHT_DOWN);
						break;
					default:
						break;
					}
					falling = true;
					startY = posPlayer.y;
				}
			}
		}
		else {
			falling = true;
		}
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}
	glm::ivec2 result = glm::ivec2(-1, -1);
	if (level_x != floor(posPlayer.x / LEVEL_SIZE_X) || level_y != floor(posPlayer.y / LEVEL_SIZE_Y)) {
		result = glm::ivec2(posPlayer.x / LEVEL_SIZE_X, posPlayer.y / LEVEL_SIZE_Y);
	}
	on_moving_platform = false;
	if (!death_in_progress && za_warudo && Game::instance().getKey(' ')) {
		timeStop();
	}
	if (Game::instance().getKey('q')) inmortal = true;
	return result;
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Player::getUpperPosition() {
	return posPlayer;
}

glm::vec2 Player::getLowerPosition() {
	return posPlayer + character_size;
}

int Player::getDir() {
	return dirY;
}

bool Player::getZaWarudo() {
	return SoundEngineZaWarudo->isCurrentlyPlaying("sounds/Za_Warudo.mp3");
}

bool Player::isZaWarudoAvailable() {
	return za_warudo;
}

void Player::playerDies() {
	if (!inmortal && !death_in_progress) {
		SoundEngineDeath->play2D("sounds/Death.mp3", GL_FALSE);
		death_in_progress = true;
		switch (sprite->animation()) {
		case STAND_LEFT_DOWN:
			sprite->changeAnimation(DEAD_LEFT_DOWN);
			break;
		case STAND_RIGHT_DOWN:
			sprite->changeAnimation(DEAD_RIGHT_DOWN);
			break;
		case STAND_LEFT_UP:
			sprite->changeAnimation(DEAD_LEFT_UP);
			break;
		case STAND_RIGHT_UP:
			sprite->changeAnimation(DEAD_RIGHT_UP);
			break;
		case MOVE_LEFT_DOWN:
			sprite->changeAnimation(DEAD_LEFT_DOWN);
			break;
		case MOVE_LEFT_UP:
			sprite->changeAnimation(DEAD_LEFT_UP);
			break;
		case MOVE_RIGHT_DOWN:
			sprite->changeAnimation(DEAD_RIGHT_DOWN);
			break;
		case MOVE_RIGHT_UP:
			sprite->changeAnimation(DEAD_RIGHT_UP);
			break;
		default:
			break;
		}
	}
}

void Player::executeDeath() {
	if (!SoundEngineZaWarudo->isCurrentlyPlaying("sounds/Za_Warudo.mp3")) za_warudo = true;
	setPosition(posCheckpoint*map->getTileSize());
	death_in_progress = false;
	dirY = 1;
	sprite->changeAnimation(STAND_RIGHT_DOWN);
	falling = false;
}

bool Player::tileBloqueante(int id) {
	if (id != 0 && id != 4 && id != 8 && id != 12 && id != 16 && id != 20) return true;
	return false;
}

void Player::setCheckpoint(glm::ivec2 checkpoint) {
	inmortal = false;
	if (!SoundEngineZaWarudo->isCurrentlyPlaying("sounds/Za_Warudo.mp3")) za_warudo = true;
	if (!(checkpoint.x == posCheckpoint.x && checkpoint.y == posCheckpoint.y)){
		SoundEngineSave->play2D("sounds/save.wav", GL_FALSE);
	}
	posCheckpoint = checkpoint;
	Checkpoint::instance().setPosition(glm::ivec2(checkpoint.x,checkpoint.y+1)*map->getTileSize());
}

void Player::movingPlatform(int id, int speed) {
	// id==2 horizontal derecha (-2 hor izq)
	// id==3 vertical arriba (-3 ver abajo)
	on_moving_platform = true;
	if (falling) {
		falling = false;
		posPlayer.y -= 4 * dirY;
	}
	posPlayer.y -= FALL_STEP * dirY;
	if (id == 2) {
		int bloque_colision = map->collisionMoveRight(posPlayer+speed, character_size);
		if (!tileBloqueante(bloque_colision)) posPlayer.x += speed;
	}
	else if (id == -2) {
		int bloque_colision = map->collisionMoveLeft(posPlayer-speed, character_size);
		if (!tileBloqueante(bloque_colision)) posPlayer.x -= speed;
	}
	else if (id == 3) posPlayer.y -= speed;
	else if (id == -3) posPlayer.y += speed;
	if (map->collisionDeadly(posPlayer, character_size, &posPlayer.y)) playerDies();
}

void Player::timeStop() {
	if (!SoundEngineZaWarudo->isCurrentlyPlaying("sounds/Za_Warudo.mp3")) {
		za_warudo = false;
		SoundEngineZaWarudo->play2D("sounds/Za_Warudo.mp3", GL_FALSE);
	}
}

void Player::win() {
	Game::instance().win();
}