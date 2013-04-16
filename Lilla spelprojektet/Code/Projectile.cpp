#include "Projectile.h"

Projectile::Projectile() : Entity()
{
	this->target = NULL;
	this->speed = 0;
	this->damage = 0;
}

Projectile::Projectile(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, Enemy* target, float speed, float damage)
	: Entity(pos, meshID, textureID, hp, lightID)
{
	this->target = target;
	this->speed = speed;
	this->damage = damage;
}

Projectile::~Projectile()
{
}

int Projectile::update(float dt)
{

	return 1;
}