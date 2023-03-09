#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


//#define SCREEN_X 144
//#define SCREEN_Y 99
#define SCREEN_X 0
#define SCREEN_Y 48

Scene::Scene()
{
	map = NULL;
	player = NULL;

}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}

// Public functions

void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level28.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	initPlayer();
	initEnemies();
	initItems();

	//projection = glm::ortho(0.f, float(windowSize.x - 1), float(windowSize.y - 1), 0.f);
	//projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0;
	remainingSeconds = 60;
}

void Scene::update(int deltaTime)
{
	updateTime(deltaTime);

	player->update(deltaTime);
	map->setPosPlayer(player->getPosition());
	for (auto e : enemies)
	{
		e->update(deltaTime);
		if (e->collisionPlayer())
			player->loseLive();
	}
	for (auto i : items)
		i->update(deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	map->render();
	for (auto i : items)
		i->render();
	for (auto e : enemies)
		e->render();
	player->render();
}

void Scene::updateRatioWindowSize(int width, int height)
{
	float gameRatio = 1.28f;
	float windowRatio = width / float(height);
	float scale = 1.0f;

	if (windowRatio > gameRatio)
	{
		scale = height / 400.0f;  // escalate Y axis
		float offset = (width - 512*scale) / 2;
		projection = glm::ortho(-offset, float(width)-offset, float(height), 0.f);
	}
	else
	{
		scale = width / 512.0f;  // escalate X axis
		float offset = (height - 400*scale) / 2;
		projection = glm::ortho(0.f, float(width), float(height)-offset, -offset);
		//projection = glm::ortho(0.f, float(width), float(height), 0.f);
	}
	projection = glm::scale(projection, glm::vec3(scale));

	//projection = glm::scale(projection, glm::vec3(1.0f / windowRatio, 1.0f, 1.0f));

	//glViewport(0, 0, width, height);
}

// Private functions

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::initPlayer()
{
	player = new Player();
	glm::ivec2 posPlayer = map->getPosPlayer();

	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(posPlayer[0] * map->getTileSize(), posPlayer[1] * map->getTileSize()));
	player->setTileMap(map);
}

void Scene::initEnemies()
{
	vector<std::pair<char, glm::ivec2>> tileMapEnemies = map->getEnemies();

	for (unsigned int i = 0; i < tileMapEnemies.size(); ++i) {
		if (tileMapEnemies[i].first == 'S') {
			enemies.push_back(new Skeleton());
		}
		else if(tileMapEnemies[i].first == 'V')
		{
			enemies.push_back(new Vampire());
		}
		auto e = enemies[i];
		e->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		e->setPosition(glm::vec2(tileMapEnemies[i].second[0] * map->getTileSize(), tileMapEnemies[i].second[1] * map->getTileSize()));
		e->setTileMap(map);
	}
}

void Scene::initItems()
{
	items.push_back(new Key());
	items.push_back(new Door());

	for (auto i : items)
	{
		i->setTileMap(map);
		i->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	}
}

void Scene::updateTime(int deltatime)
{
	currentTime += deltatime;
	if (60 - (currentTime / 1000) < remainingSeconds)
	{
		--remainingSeconds;
		cout << remainingSeconds << endl;
	}
}
