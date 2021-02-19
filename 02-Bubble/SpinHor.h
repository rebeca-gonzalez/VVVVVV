#include "Enemy.h"

class SpinHor : public Enemy
{
public:
	SpinHor();
	~SpinHor();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
};

