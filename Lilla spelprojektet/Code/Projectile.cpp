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

	trail = ParticleSystem::Getinstance()->addTrail(D3DXVECTOR3(1, 1, 1),this->getPosition(), 10, 0.05f, 0, 1, 1, 1);
}

Projectile::~Projectile()
{
	ParticleSystem::Getinstance()->removePolicy(trail);
}

int Projectile::update(float dt)
{
	if(target->isDead())
		return 0;

	float length;
	D3DXVECTOR3 dir;
	D3DXVECTOR3 pos = getPosition();
	dir = target->getPosition() - pos;
	length = D3DXVec3Length(&dir);
	D3DXVec3Normalize(&dir, &dir);

	pos += (dir * speed * dt);
	this->setPosition(pos);

	//returnerna ett annat värde om projektilen har träffat sitt target.
	if(length <= 1)
	{
		target->doDamage(damage);
		return 0;
	}

	renderData.worldMat = scale * pointTranslate * rotation * translate;

	trail->updatePosition(pos);

	return 1;
}