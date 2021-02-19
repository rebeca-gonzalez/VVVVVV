#include "Belt.h"

enum BeltAnims
{
	RIGHT, LEFT,
};

Belt::Belt()
{
}


Belt::~Belt()
{
}

void Belt::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, bool right, float x, float y,bool blue) {
	size = glm::ivec2(16, 16);
	speed = 10;
	if (blue) spritesheet.loadFromFile("images/belt.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else spritesheet.loadFromFile("images/green_belt.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(size, glm::vec2(0.25f, 0.5f), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(RIGHT, speed);
	sprite->addKeyframe(RIGHT, glm::vec2(0.75f, 0.5f));
	sprite->addKeyframe(RIGHT, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(RIGHT, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(RIGHT, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(LEFT, speed);
	sprite->addKeyframe(LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(LEFT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(LEFT, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(LEFT, glm::vec2(0.75f, 0.f));

	tileMapDispl = tileMapPos;
	if (right) sprite->changeAnimation(0);
	else sprite->changeAnimation(1);
	sprite->setPosition(glm::vec2(/*32.f + */x * float(tileMapDispl.x)/2, /*16.f +*/ y * float(tileMapDispl.y)));
}

void Belt::update(int deltaTime) {
	sprite->update(deltaTime);
}

void Belt::render() {
	sprite->render();
}