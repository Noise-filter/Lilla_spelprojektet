#pragma once
#include "stdafx.h"
#include "Node.h"
#include "Structure.h"
#include "Enemy.h"
#include "Tower.h"
#include "Supply.h"
#include <vector>
using namespace std;

class Level
{
public:
	Level(void);
	~Level(void);

	bool init(int mapSize,int quadSize);

	void getRenderData(vector<vector<RenderData*>>& rData);
	int update(float dt, vector<Enemy*>& enemies);

	bool buildStructure(D3DXVECTOR3 mouseClickPos, int selectedStructure);
	bool isAdjecent(int xPos, int yPos);
	bool isLocationBuildable(int xPos, int yPos);

private:
	Node** nodes;
	Structure*** structures;

	int quadSize;
	int mapSize;
};

