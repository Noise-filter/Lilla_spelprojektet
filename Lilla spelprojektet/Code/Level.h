#pragma once
#include "stdafx.h"
#include "Node.h"
#include "Structure.h"
class Level
{
public:
	Level(void);
	~Level(void);

	bool init(int mapSize,int quadSize);

	RenderData* getRenderData();
	int update(float dt);
private:
	Node** nodes;
	Structure** structures;

	int quadSize;
	int mapSize;
};

