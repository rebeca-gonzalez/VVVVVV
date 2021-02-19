#include "Enemy.h"

class SoldierHor : public Enemy
{
public:
	SoldierHor();
	~SoldierHor();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
};

