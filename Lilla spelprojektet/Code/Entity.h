#ifndef ENTITY_H
#define ENTITY_H

#include "D3DMathUtility.h"
#include "SoundSystem.h"

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
	void setDead(bool dead);

	float getHp();
	bool isDead();
	RenderData& getRenderData();
	D3DXVECTOR3 getPosition();

protected:
	D3DXMATRIX scale, pointTranslate, rotation, translate;

	RenderData renderData;

	float hp, maxHp;
	bool dead;
	float scaleFactor;

	Sound* death;
};

#endif