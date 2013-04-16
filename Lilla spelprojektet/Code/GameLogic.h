#pragma once
#include "Level.h"
#include "EnemyHandler.h"
class GameLogic
{
public:
	GameLogic(void);
	~GameLogic(void);

	vector<vector<RenderData*>> getRenderData();
	bool init(int mapSize, int quadSize);
	int update(float dt);
private:
	vector<vector<RenderData*>> rDataList;
	Level* level;
	EnemyHandler* eHandler;
};

