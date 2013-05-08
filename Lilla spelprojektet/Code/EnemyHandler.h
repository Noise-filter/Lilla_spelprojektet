#pragma once
#include "Enemy.h"
#include "AI.h"
#include "Node.h"
#include <vector>
using namespace std;

class EnemyHandler
{
public:
	EnemyHandler(void);
	~EnemyHandler(void);
	
	bool init(Structure*** structures, Node** nodes, int mapSize, int quadSize, int enemiesPerMin, int difficulty);

	void getRenderData(vector<vector<RenderData*>>& rData);
	void getHPBarInfo(vector<HPBarInfo>& hpBars);
	int update(float dt);

	vector<Enemy*>& getEnemies();

private:
	vector<Enemy*> enemies;
	AI* ai;

};

