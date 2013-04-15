#ifndef ENTITY_H
#define ENTITY_H

#include "stdafx.h"

class Entity
{
public:
	Entity();
	Entity(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID);
	virtual ~Entity();

	virtual int update(float dt) = 0;

	RenderData& getRenderData();


protected:
	RenderData renderData;
	float hp, maxHp;

};

#endif