#pragma once
#include "stdafx.h"
#include "Node.h"
class Level
{
public:
	Level(void);
	~Level(void);

	bool init(int mapSize);

	RenderData* getRenderData();
	int update(float dt);
private:
	Node** nodes;
	int mapSize;
};

