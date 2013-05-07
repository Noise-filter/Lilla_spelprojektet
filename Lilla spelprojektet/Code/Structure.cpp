#include "Structure.h"

Structure::Structure() : Entity()
{}

Structure::Structure(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID)
	: Entity(pos, meshID, textureID, hp, lightID)
{
	scaleFactor = 2;
	D3DXMatrixScaling(&scale, scaleFactor, scaleFactor, scaleFactor);
}

Structure::~Structure()
{
	ParticleSystem::Getinstance()->addDeathExplosion(D3DXVECTOR3(1, 1, 1), getPosition(), 100, 1, 10);
}

int Structure::update(float dt)
{
	return Entity::update(dt);
}