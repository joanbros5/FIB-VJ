#ifndef _SOUND_FACTORY_INCLUDE
#define _SOUND_FACTORY_INCLUDE

#include <irrKlang.h>

using namespace irrklang;

class SoundFactory
{
private:
	SoundFactory() {
		engine = createIrrKlangDevice();
		immuneSound = engine->addSoundSourceFromFile("sounds/immune.wav");
		tickSound = engine->addSoundSourceFromFile("sounds/time_tick.wav");
		timeTicks = 0;
	}

public:
	static SoundFactory& instance()
	{
		static SoundFactory SF;
		return SF;
	}

	void setAllSoundsPaused(bool b);
	//void stopAllSounds(bool b);

	void playChangeOption();
	void playSelectOption();
	void playTimeTick();
	void playTimeStop();
	void playTimeResume();
	void playPlayerDie();
	void playKeyTaken();
	void playKeySpawn();
	void playVampireTransform();
	void playVampireUntransform();
	void playReaperCharge();
	void playReaperAttack();
	void playPlayerSpawn();
	void playJump();
	void playPointsObtained();
	void playLifeUp();

	void playImmune();
	void stopImmune();

	void setTimeTicks(int n) { timeTicks = n; }

private:
	ISoundEngine* engine;

	ISoundSource* immuneSound;
	ISoundSource* tickSound;

	int timeTicks;
};


#endif

