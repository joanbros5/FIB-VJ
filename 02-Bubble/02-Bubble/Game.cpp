#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	windowSize = glm::vec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	scene.setWindowSize(windowSize);
	
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();

}

#include <iostream>
bool Game::update(int deltaTime)
{
	glm::vec2 currentWindowSize = glm::vec2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	if (currentWindowSize != windowSize)
	{
		windowSize = currentWindowSize;
		scene.setWindowSize(currentWindowSize);
		cout << "camera: " << windowSize.x << " " << windowSize.y << endl;
	}
	scene.update(deltaTime);
	
	return bPlay;
}

#include <iostream>
void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





