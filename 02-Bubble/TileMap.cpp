#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include "Belt.h"

using namespace std;

// Bloque 12 -> Bloque de colision de enemigos para delimitar rutas

TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, vector<Belt *> &belts)
{
	TileMap *map = new TileMap(levelFile, minCoords, program, belts);

	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, vector<Belt *> &belts)
{
	loadLevel(levelFile, program, belts);
	prepareArrays(minCoords, program);
	bloquesDeadly.push_back(4);
	bloquesDeadly.push_back(8);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile, ShaderProgram &program, vector<Belt *> &belts)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j<mapSize.y; j++)
	{
		for (int i = 0; i<mapSize.x; i++)
		{
			fin.get(tile);
			while (tile == ',')
				fin.get(tile);
			string sTile = "";
			while (tile != ',' && tile != '\n') {
				sTile += tile;
				fin.get(tile);
			}
			if (sTile == " " || sTile == "-1")
				map[j*mapSize.x + i] = 0;
			else {
				int id = 1 + stoi(sTile);
				map[j*mapSize.x + i] = id;
				if (id == 21) {
					Belt* belt = new Belt();
					belt->init(glm::ivec2(32.f, 16.f), program, false, (float)i, (float)j,true);
					belts.push_back(belt);
				}
				else if (id == 22) {
					Belt* belt = new Belt();
					belt->init(glm::ivec2(32.f, 16.f), program, false, (float)i, (float)j,false);
					belts.push_back(belt);
				}
				else if (id == 25) {
					Belt* belt = new Belt();
					belt->init(glm::ivec2(32.f, 16.f), program, true, (float)i, (float)j,true);
					belts.push_back(belt);
				}
				else if (id == 26) {
					Belt* belt = new Belt();
					belt->init(glm::ivec2(32.f, 16.f), program, true, (float)i, (float)j,false);
					belts.push_back(belt);
				}
			}
		}
		//fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%tilesheetSize.x) / tilesheetSize.x, float((tile-1)/tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
			
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

bool TileMap::blockIsDeadly(const int id) const {
	for (int i = 0; i < bloquesDeadly.size(); ++i) {
		if (bloquesDeadly[i] == id) return true;
	}
	return false;
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.
int TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	int result = 0;
	for(int y=y0; y<=y1; y++)
	{
		int id = map[y*mapSize.x + x];
		if(id != 0)
			if (result == 0 || !blockIsDeadly(id)) result = id;
	}
	
	return result;
}

int TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	int result = 0;
	for(int y=y0; y<=y1; y++)
	{
		int id = map[y*mapSize.x + x];
		if (id != 0) {
			if (result == 0 || !blockIsDeadly(id)) result = id;
		}
	}
	
	return result;
}

int TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, bool enemy) const
{
	int x0, x1, y;
	
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	int priorityId = 0;
	for(int x=x0; x<=x1; x++)
	{
		int id = map[y*mapSize.x + x];
		if(id != 0 && id != 16 && id != 20 && (enemy || id!=12))
		{
			if(*posY - tileSize * y + size.y <= 4)
			{
				*posY = tileSize * y - size.y;
				if (id>priorityId) priorityId = id;
			}
		}
	}
	return priorityId;
}

int TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, bool enemy) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		int id = map[y*mapSize.x + x];
		if (id != 0 && id != 16 && id != 20 && (enemy || id!=12))
		{
			*posY += 4;
			return id;
		}
	}

	return 0;
}

bool TileMap::collisionDeadly(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	if (blockIsDeadly(collisionMoveLeft(pos, size)) || blockIsDeadly(collisionMoveRight(pos, size)) /*|| blockIsDeadly(collisionMoveDown(pos, size, posY)) || blockIsDeadly(collisionMoveUp(pos, size, posY))*/ )
	{
		return true;
	}
	return false;
}

glm::ivec2 TileMap::collisionCheckpoint(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		int id = map[y*mapSize.x + x];
		if (id == 16) return glm::ivec2(x, y - 1);
	}

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	int result = 0;
	for (int y = y0; y <= y1; y++)
	{
		int id = map[y*mapSize.x + x];
		if (id == 16) return glm::ivec2(x, y - 1);
	}

	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;

	for (x = x0; x <= x1; x++)
	{
		int id = map[y*mapSize.x + x];
		if (id == 16) return glm::ivec2(x, y - 1);
	}

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y) / tileSize;
	for (x = x0; x <= x1; x++)
	{
		int id = map[y*mapSize.x + x];
		if (id == 16) return glm::ivec2(x, y - 1);
	}

	return glm::ivec2(-1, -1);
}
