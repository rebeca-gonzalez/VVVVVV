#include "YesHor.h"


enum YesAnims
{
	MOVE,
};

YesHor::YesHor()
{
}


YesHor::~YesHor()
{
}

void YesHor::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	character_size = glm::ivec2(30, 30);
	speedX = 2.f;
	spritesheet.loadFromFile("images/yes.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(character_size, glm::vec2(0.25f, 1.f), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(MOVE, 10);
	sprite->addKeyframe(MOVE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(0.75f, 0.f));
	Enemy::init(tileMapPos, shaderProgram);
}
