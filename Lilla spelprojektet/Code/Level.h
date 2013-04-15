#pragma once
#include "stdafx.h"
class Level
{
public:
	Level(void);
	~Level(void);

	RenderData* getRenderData();
	int update(float dt);
private:

};

