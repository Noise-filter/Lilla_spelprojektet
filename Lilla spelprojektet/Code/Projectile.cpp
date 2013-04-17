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
	float length;
	D3DXVECTOR3 dir;
	if(target != NULL)
	{
		D3DXVECTOR3 pos = getPosition();
		dir = target->getPosition() - pos;
		length = D3DXVec3Length(&dir);
		D3DXVec3Normalize(&dir, &dir);

		pos += (dir * speed * dt);
		this->setPosition(pos);
	}
	
	//returnerna ett annat värde om projektilen har träffat sitt target. Eller om targetet har försvunnit.
	if(length <= 1)
	{
		target->doDamage(damage);
		return 0;
	}

	if(target == NULL)
		return 0;

	return 1;
}