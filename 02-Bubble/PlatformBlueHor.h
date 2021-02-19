#include "Enemy.h"
class PlatformBlueHor:public Enemy
{
public:
	PlatformBlueHor(int speed);
	~PlatformBlueHor();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	int update(int deltaTime, glm::vec2 upPos, glm::vec2 lowPos, int dir);
	bool checkCollision(glm::vec2 upPosPlayer, glm::vec2 lowPosPlayer, glm::vec2 upPosCollider, glm::vec2 lowPosCollider, int dir);
	int abs(int a);
	int min(int a, int b);
};

