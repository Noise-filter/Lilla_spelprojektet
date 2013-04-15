#pragma once
#include "stdafx.h"
class EnemyHandler
{
public:
	EnemyHandler(void);
	~EnemyHandler(void);
	
	RenderData* getRenderData();
	int update(float dt);
private:
	//Enemy* enemies;
};

