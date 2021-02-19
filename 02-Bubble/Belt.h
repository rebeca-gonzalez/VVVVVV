#ifndef _BELT_INCLUDE
#define _BELT_INCLUDE

#include "ShaderProgram.h"
#include "Sprite.h"

class Belt
{
public:
	Belt();
	~Belt();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool right, float x, float y, bool blue);
	void update(int deltaTime);
	void render();

private:
	Sprite * sprite;
	Texture spritesheet;
	int speed;
	glm::ivec2 size;
	glm::ivec2 tileMapDispl;
};

#endif