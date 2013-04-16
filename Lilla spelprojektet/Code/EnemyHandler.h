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

	vector<RenderData*> getRenderData();
	int update(float dt);

private:
	vector<Enemy*> enemies;
	vector<RenderData*> renderData;

};

