#include "Structure.h"

Structure::Structure() : Entity()
{
	fake = false;
}

Structure::Structure(Vec3 pos, int meshID, int textureID, float hp, int lightID, bool fakeBuilding)
	: Entity(pos, meshID, textureID, hp, lightID)
{
	scaleFactor = 2.3f;
	fake = fakeBuilding;

	D3DXMatrixScaling(&scale, scaleFactor, scaleFactor, scaleFactor);
}

Structure::~Structure()
{
	if(!fake)
		ParticleSystem::Getinstance()->addDeathExplosion(Vec3(1, 1, 1), getPosition(), 100, 1, 10);
}

int Structure::update(float dt)
{
	return Entity::update(dt);
}

HPBarInfo Structure::getHPBarInfo()
{
	HPBarInfo info(translate, hp/maxHp);

	info.translate._41 -= 3;
	info.translate._42 += 5;


	return info;
}