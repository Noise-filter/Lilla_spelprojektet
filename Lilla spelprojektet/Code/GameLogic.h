#pragma once
#include "Level.h"
#include "EnemyHandler.h"
class GameLogic
{
public:
	GameLogic(void);
	~GameLogic(void);

	RenderData* getRenderData();
	bool init(int mapSize);
	int update(float dt);
private:
	Level* level;
	EnemyHandler* eHandler;
};

