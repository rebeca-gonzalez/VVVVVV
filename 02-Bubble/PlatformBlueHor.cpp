#include "PlatformBlueHor.h"



PlatformBlueHor::PlatformBlueHor(int speed)
{
	speedX = speed;
}


PlatformBlueHor::~PlatformBlueHor()
{
}

void PlatformBlueHor::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	character_size = glm::ivec2(64, 16);
	collision_value = 2;
	spritesheet.loadFromFile("images/blue_platform.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(character_size, glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	Enemy::init(tileMapPos, shaderProgram);
}

int PlatformBlueHor::abs(int a) {
	if (a > 0) return a;
	return a * -1;
}

int PlatformBlueHor::min(int a, int b) {
	if (a < b) return a;
	else return b;
}

int PlatformBlueHor::update(int deltaTime, glm::vec2 upPos, glm::vec2 lowPos, int dir) {
	// Retorna true si se choca con un enemigo
	Character::update(deltaTime);
	posCharacter.x += speedX;
	posCharacter.y += speedY;

	int bloque_colision_right = map->collisionMoveRight(posCharacter, character_size);
	int bloque_colision_left = map->collisionMoveLeft(posCharacter, character_size);
	int up = upPos.y;
	int down = lowPos.y;
	int bloque_colision_down = map->collisionMoveDown(posCharacter, character_size, &down, true);
	int bloque_colision_up = map->collisionMoveUp(posCharacter, character_size, &up, true);
	if (bloque_colision_right != 0 || bloque_colision_left != 0 || bloque_colision_down != 0 || bloque_colision_up != 0) {
		speedX *= -1;
		speedY *= -1;
	}

	Character::setPosition(posCharacter);
	bool collision = checkCollision(upPos, lowPos, posCharacter, posCharacter + character_size, dir);
	if (collision && collision_value != 0 && collision_value != 1 && sign(speedX) != sign(collision_value)) collision_value *= -1;
	if (collision) return collision_value;
	else return 0;
}

bool PlatformBlueHor::checkCollision(glm::vec2 upPosPlayer, glm::vec2 lowPosPlayer, glm::vec2 upPosCollider, glm::vec2 lowPosCollider, int dir) {
	if (
		(upPosCollider.x <= upPosPlayer.x && upPosPlayer.x <= lowPosCollider.x) && ((lowPosCollider.y >= upPosPlayer.y && upPosPlayer.y >= upPosCollider.y) || (lowPosCollider.y >= lowPosPlayer.y && lowPosPlayer.y >= upPosCollider.y)) ||
		(upPosCollider.x <= lowPosPlayer.x && lowPosPlayer.x <= lowPosCollider.x) && ((lowPosCollider.y >= upPosPlayer.y && upPosPlayer.y >= upPosCollider.y) || (lowPosCollider.y >= lowPosPlayer.y && lowPosPlayer.y >= upPosCollider.y))
		)
	{
		int dif_clipping;
		if ((lowPosCollider.y >= upPosPlayer.y && upPosPlayer.y >= upPosCollider.y)) {
			if (dir == 1) return false;
			else dif_clipping = abs(upPosPlayer.y - lowPosCollider.y);
		}
		else if (((lowPosCollider.y >= lowPosPlayer.y && lowPosPlayer.y >= upPosCollider.y))) {
			if (dir == 1) dif_clipping = abs(lowPosPlayer.y - upPosCollider.y);
			else return false;
		}
		if (dif_clipping > 3) return false;
		return true;
	}
	return false;
}