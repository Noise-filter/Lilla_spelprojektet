#ifndef ENTITY_H
#define ENTITY_H

#include "D3DMathUtility.h"

class Entity
{
public:
	Entity();
	Entity(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID);
	virtual ~Entity();

	int update(float dt);

	void doDamage(float damage);	//Does damage on the entity

	void setPosition(D3DXVECTOR3& pos);
	void setRotation(D3DXVECTOR3& dir);

	float getHp();

	bool isDead();

	RenderData& getRenderData();

	D3DXVECTOR3 getPosition();

protected:
	RenderData renderData;
	float hp, maxHp;
	bool dead;

};

#endif