#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"

#include <Windows.h>
#include <thread>

using namespace std;

enum Items
{
	KEY, DOOR
};


TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	TileMap* map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
	
	steppedFloorSprite = Sprite::createSprite(glm::ivec2(tileSize, tileSize), glm::vec2(0.5, 0.5), &tilesheet, &program);
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);

	for (auto p : positionsStepped)
	{
		steppedFloorSprite->setPosition(glm::vec2(p[0]* tileSize, p[1] * tileSize));
		steppedFloorSprite->render();
	}
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string& levelFile)
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
	tileTexSize = glm::vec2(0.5f, 0.5f);

	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin.get(tile);
			if (tile == ' ')
				map[j * mapSize.x + i] = 0;
			//Skeleton
			else if (tile == 'S') {
				enemies.push_back(make_pair('S', glm::ivec2(i,j)));
				map[j * mapSize.x + i] = 0;
			}
			//Player
			else if (tile == 'P') {
				posPlayer = glm::ivec2(i, j);
				map[j * mapSize.x + i] = 0;
			}
			//Door
			else if (tile == 'D') {

				posDoor = glm::ivec2(i, j);

				map[j * mapSize.x + i] = 0;
			}
			else {
				//Steppable platform
				if (tile == '2')
				{
					++nStepTiles;
					plattforms.push_back(glm::ivec2(i, j));
				}
				map[j * mapSize.x + i] = tile - int('0');
			}
		}
		fin.get(tile);

#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	//DEBUG
	//cout << "Stepping Tiles: " << nStepTiles << endl;
	//for (auto p : plattforms) cout << "platform in: " << p.x << " " << p.y << endl;

	return true;
}

void TileMap::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				//texCoordTile[1] -= halfTexel;
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
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	std::cout << "tileTexSize: " << tileTexSize[0] << " " << tileTexSize[1] << endl;
	std::cout << "nTiles: " << nTiles << endl;
	std::cout << "tileSize: " << tileSize << endl;
	std::cout << "blockSize: " << blockSize << endl;
	std::cout << "mapSize: " << mapSize[0] << " " << mapSize[1] << endl;
	std::cout << "tileSheetSize: " << tilesheetSize[0] << " " << tilesheetSize[1] << endl;
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] == 3)
			return true;
	}
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] == 3)
			return true;
	}
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] == 3)
			if (*posY - tileSize * y < tileSize)
				return true;
	}
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0)
		{
			if (*posY - tileSize * y + size.y <= 4)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}
	return false;
}

void TileMap::positionStepped(const glm::ivec2& pos, const glm::ivec2& size, int* posY)
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (*posY - tileSize * y + size.y <= 4)
		{
			if (map[y * mapSize.x + x] == 2)
			{
				map[y * mapSize.x + x] = 1;
				positionsStepped.push_back(glm::ivec2(x + 2, y + 1));
				--nStepTiles;
			}
			*posY = tileSize * y - size.y;
		}
	}

}

glm::ivec2 TileMap::getPosPlayer() const
{
	return posPlayer;
}

vector<pair<char, glm::ivec2>> TileMap::getEnemies() const
{
	return enemies;
}

bool TileMap::doorOpen()
{
	return false;
}

glm::vec2 TileMap::getRandomPosition()
{
	srand(time(NULL));
	return plattforms[rand() % plattforms.size()];
}

glm::ivec2 TileMap::getDoorPosition()
{
	return posDoor;
}

bool TileMap::allPlattformsStepped() const
{
	return nStepTiles == 0;
}

void TileMap::setPosPlayer(glm::ivec2 pos)
{
	posPlayer = pos;
}

void TileMap::setKeyTaken(bool b)
{
	keyTaken = b;
}

bool TileMap::getKeyTaken() const
{
	return keyTaken;
}