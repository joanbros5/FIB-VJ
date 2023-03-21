#ifndef _LEVEL_INTERFACE_INCLUDE
#define _LEVEL_INTERFACE_INCLUDE

#include <map>
#include "Sprite.h"

#include "StageState.h"

// SceneInterface contains all the entities of our game.
// It is responsible for updating and render them.


class SceneInterface
{

public:
	SceneInterface();
	~SceneInterface();

	void init(ShaderProgram &shaderProgram);
	void updateLives(int l);
	void updateScore(int s);
	void updateRemainingTime(int t);
	void updateStageNumber(int s);
	void render();

	// Gettters & Setters
	void setState(StageState s) { state = s; };

private:
	void initNumberSprite(ShaderProgram& shaderProgram);
	void initCharacterSprite(ShaderProgram& shaderProgram);
	void initHeartSprite(ShaderProgram& shaderProgram);
	void renderMessages();
	void renderNumber(int n, int x, int y);
	void renderCharacter(char c, int x, int y);


private:
	Texture numbersSpritesheet, charactersSpritesheet, heartSpritesheet;

		// TENEMOS UN SOLO SPRITE PARA CADA COSA Y LO VAMOS COPIANDO A LOS LADOS E IMPREMINEDO VARIAS VECES EN CADA UPDATE

	//Sprite* livesSprite, scoreSprite, remainingTimeSprite, stageSprite;
	Sprite *numberSprite;
	Sprite *characterSprite;
	Sprite *heartSprite;

	int lives, score, remainingTime, stageNumber;
	map<char, int> char2id;

	StageState state;
};


#endif // _LEVEL_INTERFACE_INCLUDE

