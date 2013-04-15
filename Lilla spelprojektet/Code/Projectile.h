#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Entity.h"
#include "Enemy.h"

class Projectile : public Entity
{
public:
	Projectile();
	~Projectile();

	virtual int update(float dt);




private:
	Enemy* target;
	float speed;
	float damage;

};

#endif