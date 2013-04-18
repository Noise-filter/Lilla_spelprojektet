#pragma once
#include "stdafx.h"
#include "Enemy.h"
#include <vector>
using namespace std;

class EnemyHandler
{
public:
	EnemyHandler(void);
	~EnemyHandler(void);
	
	bool init();

	void getRenderData(vector<vector<RenderData*>>& rData);
	int update(float dt);

	vector<Enemy*>& getEnemies();

private:
	vector<Enemy*> enemies;

};

