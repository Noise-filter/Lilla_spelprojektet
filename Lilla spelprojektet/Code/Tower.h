#ifndef TOWER_H
#define TOWER_H

#include "Structure.h"
#include "Enemy.h"
#include "Projectile.h"
#include <vector>
using namespace std;

class Tower : public Structure
{
public:
	Tower();
	Tower(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, float damage, float attackSpeed, float range, float projectileSpeed);
	virtual ~Tower();

	virtual int update(float dt);

	void aquireTarget(vector<Enemy*>& enemies);

private:
	float damage;
	float attackSpeed;
	float range;
	float projectileSpeed;
	float cooldown;
	Enemy* target;

	vector<Projectile*> projectiles;


};

#endif