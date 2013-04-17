#pragma once
#include "stdafx.h"
#include "Node.h"
#include "Structure.h"
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

	bool buildStructure(D3DXVECTOR3 mouseClickPos, int selectedStructure);

	vector<RenderData*> getRenderData();
	int update(float dt);
private:
	Node** nodes;
	Structure** structures;
	vector<RenderData*> renderData;  

	int quadSize;
	int mapSize;
};

