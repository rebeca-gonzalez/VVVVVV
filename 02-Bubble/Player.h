#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include <irrKlang.h>
using namespace irrklang;


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, glm::vec2 playerIniPos, ShaderProgram &shaderProgram);
	glm::ivec2 update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::vec2 getUpperPosition();
	glm::vec2 getLowerPosition();
	int getDir();
	
	void playerDies();
	void executeDeath();
	void movingPlatform(int id, int speed);
	void timeStop();
	bool getZaWarudo();
	bool isZaWarudoAvailable();

private:
	bool falling, death_in_progress, on_moving_platform, za_warudo;
	int deathTime;
	glm::ivec2 tileMapDispl, posPlayer, posCheckpoint;
	int jumpAngle, startY, dirY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	ISoundEngine *SoundEngineZaWarudo;
	ISoundEngine *SoundEngineSave;
	ISoundEngine *SoundEngineJump;
	ISoundEngine *SoundEngineDeath;

	bool tileBloqueante(int id);
	bool inmortal;
	void setCheckpoint(glm::ivec2 checkpoint);
	void win();
	// Original: 12,21
	// Esto es el size del quad donde se textura el sprite
	glm::ivec2 character_size = glm::ivec2(18, 31);
};


#endif // _PLAYER_INCLUDE


