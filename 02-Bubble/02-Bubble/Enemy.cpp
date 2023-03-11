#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Enemy.h"
#include "Game.h"


//#define JUMP_ANGLE_STEP 4
//#define JUMP_HEIGHT 96
//#define FALL_STEP 4


enum EnemyAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

// Public functions

void Enemy::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {}

void Enemy::update(int deltaTime) {}

void Enemy::render()
{
	sprite->render();
}

void Enemy::vibrationMovement(int deltaTime)
{
	//sprite->update(deltaTime);
	if (mov < 5)
	{
		if (mov == 0)
			posEnemy.x += 5;
	}
	else
	{
		if (mov==5)
			posEnemy.x -= 10;
		if (mov == 9)
			mov = 0;
	}
}

// Getters & Setters

void Enemy::setPosition(const glm::vec2& pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

// Protected functions

bool Enemy::collisionPlayer()
{
	glm::ivec2 posPlayer = map->getPosPlayer();
	int tileSize = map->getTileSize();

	bool collisionX = posPlayer.x + 32 >= posEnemy.x && posEnemy.x + 32 >= posPlayer.x;
	bool collisionY = posPlayer.y + 32 >= posEnemy.y && posEnemy.y + 32 >= posPlayer.y;

	return collisionX && collisionY;
}




