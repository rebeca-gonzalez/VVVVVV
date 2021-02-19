#include "Enemy.h"

class StopHor : public Enemy
{
public:
	StopHor();
	~StopHor();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
};

