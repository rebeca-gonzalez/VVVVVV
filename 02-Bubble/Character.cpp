#include "Character.h"



Character::Character()
{
}


Character::~Character()
{
}

void Character::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	// Parte comuna a llamar al final de la implementacion de la subclase
	tileMapDispl = tileMapPos;
	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
}

void Character::update(int deltaTime) {
	sprite->update(deltaTime);
}

void Character::render() {
	sprite->render();
}

void Character::setPosition(const glm::vec2 &pos) {
	posCharacter = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCharacter.x), float(tileMapDispl.y + posCharacter.y)));
}

void Character::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}