#include "Enemy.h"

class YesHor : public Enemy
{
public:
	YesHor();
	~YesHor();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
};

