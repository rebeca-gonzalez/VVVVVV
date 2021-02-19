#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Belt.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, vector<Belt *> &belts);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, vector<Belt *> &belts);
	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }

	int collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	int collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	int collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, bool enemy) const;
	int collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, bool enemy) const;
	bool collisionDeadly(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	glm::ivec2 collisionCheckpoint(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	
private:
	bool loadLevel(const string &levelFile, ShaderProgram &program, vector<Belt *> &belts);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
	bool blockIsDeadly(const int id) const;

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;

	std::vector<int> bloquesDeadly;
};


#endif // _TILE_MAP_INCLUDE


