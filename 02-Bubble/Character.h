#ifndef _CHARACTER_INCLUDE
#define _CHARACTER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Character
{
public:
	Character();
	~Character();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void setPosition(const glm::vec2 &pos);
	void setTileMap(TileMap *tileMap);

protected:
	Texture spritesheet;
	glm::ivec2 tileMapDispl, posCharacter;
	Sprite * sprite;
	glm::ivec2 character_size;
	TileMap *map;
};

#endif