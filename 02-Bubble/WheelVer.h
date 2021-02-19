#include "Enemy.h"

class WheelVer : public Enemy
{
public:
	WheelVer();
	~WheelVer();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
};

