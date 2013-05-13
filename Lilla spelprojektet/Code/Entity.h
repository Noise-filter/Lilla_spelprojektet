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
	void setScale(float scale);
	void setRotation(D3DXVECTOR3& dir);

	float getHp();

	bool isDead();

	void setDead(bool dead);

	RenderData& getRenderData();

	D3DXVECTOR3 getPosition();

protected:
	RenderData renderData;
	float hp, maxHp;
	bool dead;

	D3DXMATRIX scale, pointTranslate, rotation, translate;
	float scaleFactor;
};

#endif