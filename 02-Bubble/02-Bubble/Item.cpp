#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Item.h"
#include "Game.h"


enum ItemAnims
{
	IDLE
};


void Item::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {}

void Item::update(int deltaTime) {}

void Item::render()
{
	//if(checkIfShowStateChanges())
	//	sprite->render();
}

void Item::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Item::setPosition(const glm::vec2& pos)
{
	posItem = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
}

glm::ivec2 Item::getPosition()
{
	return posItem;
}

bool Item::collisionPlayer()
{
	glm::ivec2 posPlayer = map->getPosPlayer();
	int tileSize = map->getTileSize();
	
	bool collisionX = posPlayer.x + 32 >= posItem.x && posItem.x + itemSize.x >= posPlayer.x;
	bool collisionY = posPlayer.y + 32 >= posItem.y && posItem.y + itemSize.y >= posPlayer.y;

	return collisionX && collisionY;
}