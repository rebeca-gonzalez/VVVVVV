#include "Enemy.h"

class WheelHor : public Enemy
{
public:
	WheelHor();
	~WheelHor();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
};

