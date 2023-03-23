#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "SceneInterface.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "Skeleton.h"
#include "Vampire.h"
#include "Reaper.h"
#include "PlayerKiller.h"
#include "Item.h"
#include "Key.h"
#include "Door.h"
#include "Hourglass.h"
#include "Treasure.h"
#include "Bible.h"

#include "StageState.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(ShaderProgram &shaderProgram, string scene);
	void update(int deltaTime);
	void render();

	// Gettters & Setters
	int getCurrentTime() { return currentTime; };
	int getPlayerLives() { return player->getLives(); };
	int getPlayerScore() { return player->getScore(); };
	bool getPlayerInvencible() { return bPlayerInvencible; };

	void setPlayerLives(int lives) { player->setLives(lives); };
	void setPlayerScore(int score) { player->addScore(score); };
	void setStageNumber(int stageNumber) { sceneInterface->updateStageNumber(stageNumber); };
	void setPlayerInvencible(bool b) { bPlayerInvencible = b; player->setInvincible(b); };
	void makeKeyAppear();

private:
	void initPlayer();
	void initEnemies();
	void initItems();
	void stageClearMessage();
	void gameOverMessage();
	void updateTime(int deltaTime);
	void updatePlayer(int deltaTime);
	void updateEnemies(int deltaTime);
	void updateItems(int deltaTime);
	void updateSceneInterface(int deltaTime);

public:
	StageState state;

private:
	SceneInterface* sceneInterface;
	TileMap *map;
	Player *player;
	vector<Enemy*> enemies;
	vector<Item*> items;
	ShaderProgram texProgram;

	Texture spritesheet;
	Sprite* sprite;

	glm::ivec2 initPosPlayer;

	int currentTime = 60;
	int remainingSeconds = 60;
	int timer = 0;
	int hourglassTimer = 0;
	int messageTimer = 0;

	int itemCountDown = 0;
	int itemSpawnCounter = 7 - (rand() % 5);
	bool itemSpawned = false;

	int itemRNG = rand() % 100;

	bool bDoorTaken, bPlayerDead;
	bool bHourglassEnding;

	bool bPlayerInvencible = false;
};


#endif // _SCENE_INCLUDE

