#ifndef ENTITY_H
#define ENTITY_H

#include "stdafx.h"

class Entity
{
public:
	Entity();
	virtual ~Entity();

	virtual int update(float dt) = 0;

	RenderData getRenderData();


private:
	RenderData renderData;
	float hp, maxHp;
	int lightID;



};

#endif