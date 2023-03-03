#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Key.h"
#include "Game.h"

enum KeyAnims
{
	IDLE
};


void Key::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/key.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram);
	
	//Animation
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(IDLE, 8);

	// Add frames to animation 0
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(IDLE, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.5f));

	// Change to animation IDLE
	sprite->changeAnimation(IDLE);

	itemSize = glm::ivec2(8, 8);
}

void Key::update(int deltaTime)
{
	if (showing && collisionPlayer())
	{
		taken = true;
		showing = false;

		map->setKeyTaken(true);
	}
	
	sprite->update(deltaTime);
}

void Key::render()
{
	if (map->allPlattformsStepped() && !taken)
	{
		if (!showing)
		{
			glm::ivec2 pos = map->getRandomPosition();
			pos = pos * 16;
			posItem = pos - glm::ivec2(0, 1);
			//DEBUG
			cout << "Position given: " << pos.x << " " << pos.y << endl;

			sprite->setPosition(glm::vec2(pos.x + 32, pos.y - 8));

			//DEBUG
			cout << "position assinged is: " << pos.x << " " << pos.y - 1 << endl;

			showing = true;
		}
		sprite->render();
	}
}