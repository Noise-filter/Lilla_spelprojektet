#include "Entity.h"

Entity::Entity()
{
	D3DXMatrixIdentity(&renderData.worldMat);
	renderData.meshID = 0;
	renderData.textureID = 0;
	renderData.lightID = 0;

	maxHp = hp = 0;
	dead = false;
}

Entity::Entity(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID)
{
	D3DXMatrixTranslation(&renderData.worldMat, pos.x, pos.y, pos.z);
	renderData.meshID = meshID;
	renderData.textureID = textureID;
	renderData.lightID = lightID;

	maxHp = hp;
	this->hp = hp;
	dead = false;

	scaleFactor = 3;
/*
	renderData.worldMat._11 = scaleFactor;
	renderData.worldMat._22 = scaleFactor;
	renderData.worldMat._33 = scaleFactor;*/
}

Entity::~Entity()
{}

int Entity::update(float dt)
{
	if(hp <= 0)
	{
		dead = true;
		return 0;
	}
	return 1;
}

void Entity::doDamage(float damage)
{
	hp -= damage;

	if(hp > maxHp)
		hp = maxHp;
	else if(hp < 0)
		hp = 0;
}

void Entity::setPosition(D3DXVECTOR3& pos)
{
	D3DXMatrixTranslation(&renderData.worldMat, pos.x, pos.y, pos.z);
}

void Entity::setRotation(D3DXVECTOR3& dir)
{

}

float Entity::getHp()
{
	return hp;
}

bool Entity::isDead()
{
	return dead;
}

void Entity::setDead(bool dead)
{
	this->dead = dead;
}

RenderData& Entity::getRenderData()
{
	return renderData;
}

D3DXVECTOR3 Entity::getPosition()
{
	return D3DXVECTOR3(renderData.worldMat._41, renderData.worldMat._42, renderData.worldMat._43);
}