#include "Enemy.h"

class SoldierVer : public Enemy
{
public:
	SoldierVer();
	~SoldierVer();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
};

