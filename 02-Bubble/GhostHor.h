#include "Enemy.h"

class GhostHor : public Enemy
{
public:
	GhostHor();
	~GhostHor();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
};

