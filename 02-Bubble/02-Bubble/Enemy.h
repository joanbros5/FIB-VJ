#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Enemy is basically a Sprite that represents the wnwmy. As such it has
// all properties it needs to track its movement, and collisions.


class Enemy
{

public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual void render();

	// Getters & Setters
	void setPosition(const glm::vec2& pos);
	void setHitbox(glm::ivec2 h) { hitbox = h; };
	void setTileMap(TileMap* tileMap) { map = tileMap; };

public:
	bool collisionPlayer();

protected:
	Texture spritesheet;
	Sprite* sprite;

	glm::ivec2 tileMapDispl, posEnemy, hitbox;
	TileMap* map;

};


#endif // _ENEMY_INCLUDE


