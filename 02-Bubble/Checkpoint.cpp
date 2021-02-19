#include "Checkpoint.h"



Checkpoint::Checkpoint()
{
}


Checkpoint::~Checkpoint()
{
}

void Checkpoint::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram){
	size = glm::ivec2(16, 16);
	spritesheet.loadFromFile("images/checkpoint.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(size, glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(-16.f, -16.f));
}


void Checkpoint::update(int deltaTime) {
	sprite->update(deltaTime);
}

void Checkpoint::render() {
	sprite->render();
}

void Checkpoint::setPosition(const glm::vec2 &pos)
{
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}
