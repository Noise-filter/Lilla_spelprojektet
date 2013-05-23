#include "Projectile.h"

Projectile::Projectile() : Entity()
{
	this->target = NULL;
	this->speed = 0;
	this->damage = 0;
	targetPos = Vec3(0,0,0);
}

Projectile::Projectile(Vec3 pos, int meshID, int textureID, float hp, int lightID, Enemy* target, float speed, float damage)
	: Entity(pos, meshID, textureID, hp, lightID)
{
	this->target = target;
	this->speed = speed;
	this->damage = damage;

	trail = ParticleSystem::Getinstance()->addTrail(Vec3(1, 1, 1),this->getPosition(), 10, 0.05f, 0, 1, 1, 1);
}

Projectile::~Projectile()
{
	ParticleSystem::Getinstance()->removePolicy(trail);
}

int Projectile::update(float dt)
{
	if(target && target->isDead())
		target = NULL;

	float length;
	Vec3 dir;
	Vec3 pos = getPosition();

	if(target && !target->isDead())
		targetPos = target->getPosition();

	dir = targetPos - pos;
	length = D3DXVec3Length(&dir);
	D3DXVec3Normalize(&dir, &dir);

	pos += (dir * speed * dt);
	this->setPosition(pos);

	//returnerna ett annat värde om projektilen har träffat sitt target.
	if(length <= dt * speed * 1.1)
	{
		if(target && !target->isDead())
		{
			if(target->getHp() > 0)
			{
				target->doDamage(damage);

				if(target->getHp() <= 0)
				{
					return 2; // död, ge xp till tornet
				}
			}
				
		}
		
		return 0;
	}

	renderData.worldMat = scale * pointTranslate * rotation * translate;

	trail->updatePosition(pos);

	return 1;
}
Enemy* Projectile::getTarget()
{
	return this->target;
}