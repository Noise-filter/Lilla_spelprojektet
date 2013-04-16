#ifndef TOWER_H
#define TOWER_H

#include "Structure.h"
#include "Enemy.h"

class Tower : public Structure
{
public:
	Tower();
	Tower(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, float damage, float attackSpeed, float range, float projectileSpeed);
	virtual ~Tower();

	virtual int update(float dt);

private:
	float damage;
	float attackSpeed;
	float range;
	float projectileSpeed;
	Enemy* target;

};

#endif