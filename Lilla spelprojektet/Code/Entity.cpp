#include "Entity.h"

Entity::Entity()
{
	D3DXMatrixIdentity(&renderData.worldMat);
	renderData.meshID = 0;
	renderData.textureID = 0;
	renderData.lightID = 0;

	maxHp = hp = 0;
}

Entity::Entity(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID)
{
	D3DXMatrixTranslation(&renderData.worldMat, pos.x, pos.y, pos.z);
	renderData.meshID = meshID;
	renderData.textureID = textureID;
	renderData.lightID = lightID;

	maxHp = hp;
	this->hp = hp;
}

Entity::~Entity()
{}

RenderData& Entity::getRenderData()
{
	return renderData;
}