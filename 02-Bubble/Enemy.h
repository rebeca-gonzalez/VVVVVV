#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE

#include "Character.h"

class Enemy : public Character
{
public:
	Enemy();
	~Enemy();

	virtual void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	virtual int update(int deltaTime, glm::vec2 upPos, glm::vec2 lowPos, bool za_warudo);
	virtual int update(int deltaTime, glm::vec2 upPos, glm::vec2 lowPos, int dir);
	float speedX=0;
	float speedY=0;
	int collision_value = 1;
	float sign(float a);

protected:
	virtual bool checkCollision(glm::vec2 upPosPlayer, glm::vec2 lowPosPlayer, glm::vec2 upPosCollider, glm::vec2 lowPosCollider);
};

#endif