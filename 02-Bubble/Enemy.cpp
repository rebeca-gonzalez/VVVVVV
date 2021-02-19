#include "Enemy.h"



Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

void Enemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {

	Character::init(tileMapPos,shaderProgram);
}

float Enemy::sign(float a) {
	if (a > 0) return 1;
	return -1;
}

int Enemy::update(int deltaTime, glm::vec2 upPos, glm::vec2 lowPos, bool za_warudo) {
	// Retorna true si se choca con un enemigo
	Character::update(deltaTime);
	if (!za_warudo) {
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
	}
	bool collision = checkCollision(upPos, lowPos,posCharacter,posCharacter+character_size);
	if (collision && collision_value != 0 && collision_value != 1 && sign(speedX) != sign(collision_value)) collision_value *= -1;
	if (collision) return collision_value;
	else return 0;
}

int Enemy::update(int deltaTime, glm::vec2 upPos, glm::vec2 lowPos, int dir) {
	return -1;
}

bool Enemy::checkCollision(glm::vec2 upPosPlayer, glm::vec2 lowPosPlayer, glm::vec2 upPosCollider, glm::vec2 lowPosCollider) {
	if (
		(upPosCollider.x <= upPosPlayer.x && upPosPlayer.x <= lowPosCollider.x) && ((lowPosCollider.y>=upPosPlayer.y && upPosPlayer.y>=upPosCollider.y) || (lowPosCollider.y>=lowPosPlayer.y && lowPosPlayer.y>=upPosCollider.y)) ||
		(upPosCollider.x <= lowPosPlayer.x && lowPosPlayer.x <= lowPosCollider.x) && ((lowPosCollider.y >= upPosPlayer.y && upPosPlayer.y >= upPosCollider.y) || (lowPosCollider.y >= lowPosPlayer.y && lowPosPlayer.y >= upPosCollider.y))
		)
	{
		return true;
	}
	return false;
}