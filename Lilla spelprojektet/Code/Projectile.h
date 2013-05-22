#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Entity.h"
#include "Enemy.h"
#include "ParticleSystem.h"

class Projectile : public Entity
{
public:
	Projectile();
	Projectile(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, Enemy* target, float speed, float damage);
	~Projectile();

	virtual int update(float dt);

	Enemy* getTarget();

private:
	Enemy* target;
	Vec3 targetPos;
	float speed;
	float damage;

	Trail* trail;

};

#endif