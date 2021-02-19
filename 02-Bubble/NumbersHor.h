#include "Enemy.h"

class NumbersHor : public Enemy
{
public:
	NumbersHor();
	~NumbersHor();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
};

