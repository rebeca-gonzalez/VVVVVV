#include "Enemy.h"

class SkeletonHor : public Enemy
{
public:
	SkeletonHor();
	~SkeletonHor();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
};

