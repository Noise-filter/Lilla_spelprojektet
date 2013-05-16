#include "Structure.h"

Structure::Structure() : Entity()
{}

Structure::Structure(Vec3 pos, int meshID, int textureID, float hp, int lightID)
	: Entity(pos, meshID, textureID, hp, lightID)
{

	scaleFactor = 2.8f;

	D3DXMatrixScaling(&scale, scaleFactor, scaleFactor, scaleFactor);
}

Structure::~Structure()
{
	ParticleSystem::Getinstance()->addDeathExplosion(Vec3(1, 1, 1), getPosition(), 100, 1, 10);
}

int Structure::update(float dt)
{
	return Entity::update(dt);
}

HPBarInfo Structure::getHPBarInfo()
{
	HPBarInfo info(translate, hp/maxHp);
	info.translate._41 -= scale._11*3;
	return info;
}