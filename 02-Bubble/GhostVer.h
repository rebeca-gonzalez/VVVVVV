#include "Enemy.h"

class GhostVer : public Enemy
{
public:
	GhostVer();
	~GhostVer();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
};

