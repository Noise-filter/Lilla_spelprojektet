#include "Structure.h"

Structure::Structure() : Entity()
{}

Structure::Structure(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID)
	: Entity(pos, meshID, textureID, hp, lightID)
{}

Structure::~Structure()
{}

int Structure::update(float dt)
{
	return Entity::update(dt);
}