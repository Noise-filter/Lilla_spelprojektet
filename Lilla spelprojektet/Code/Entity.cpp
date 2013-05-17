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
	D3DXMatrixIdentity(&renderData.worldMat);
	renderData.meshID = meshID;
	renderData.textureID = textureID;
	renderData.lightID = lightID;

	maxHp = hp;
	this->hp = hp;
	dead = false;

	scaleFactor = 1;
	
	D3DXMatrixIdentity(&scale);
	D3DXMatrixIdentity(&pointTranslate);
	D3DXMatrixIdentity(&rotation);
	D3DXMatrixTranslation(&translate, pos.x-0.3f, pos.y, pos.z);
	renderData.worldMat = scale * pointTranslate * rotation * translate;
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

	renderData.worldMat = scale * pointTranslate * rotation * translate;

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
	D3DXMatrixTranslation(&translate, pos.x, pos.y, pos.z);
}

void Entity::setScale(float scale)
{
	scaleFactor = scale;
	D3DXMatrixScaling(&this->scale, scaleFactor, scaleFactor, scaleFactor);
	renderData.worldMat = this->scale * pointTranslate * rotation * translate;
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
	return D3DXVECTOR3(translate._41, translate._42, translate._43);
}