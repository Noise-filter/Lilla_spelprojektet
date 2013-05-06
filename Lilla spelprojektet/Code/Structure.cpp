#include "Structure.h"

Structure::Structure() : Entity()
{}

Structure::Structure(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID)
	: Entity(pos, meshID, textureID, hp, lightID)
{
	renderData.worldMat._11 = 2;
	renderData.worldMat._22 = 2;
	renderData.worldMat._33 = 2;
}

Structure::~Structure()
{
	ParticleSystem::Getinstance()->addDeathExplosion(D3DXVECTOR3(1, 1, 1), getPosition(), 100, 1, 10);
}

int Structure::update(float dt)
{
	return Entity::update(dt);
}