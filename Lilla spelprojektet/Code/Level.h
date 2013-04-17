#pragma once
#include "stdafx.h"
#include "Node.h"
#include "Structure.h"
#include "Enemy.h"
#include "Tower.h"
#include <vector>
using namespace std;

class Level
{
public:
	Level(void);
	~Level(void);

	bool init(int mapSize,int quadSize);

	vector<RenderData*> getRenderData();
	int update(float dt, vector<Enemy*>& enemies);

private:
	Node** nodes;
	Structure*** structures;
	vector<RenderData*> renderData;  

	int quadSize;
	int mapSize;
};

