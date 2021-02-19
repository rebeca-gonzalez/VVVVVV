#ifndef _CHECKPOINT_INCLUDE
#define _CHECKPOINT_INCLUDE


#include "ShaderProgram.h"
#include "Sprite.h"

class Checkpoint
{
public:
	Checkpoint();
	~Checkpoint();
	static Checkpoint &instance()
	{
		static Checkpoint C;

		return C;
	}

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void setPosition(const glm::vec2 &pos);

private:
	Sprite * sprite;
	Texture spritesheet;
	glm::ivec2 size;
	glm::ivec2 tileMapDispl;
};

#endif