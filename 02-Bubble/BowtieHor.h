#include "Enemy.h"

class BowtieHor : public Enemy
{
public:
	BowtieHor();
	~BowtieHor();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
};

