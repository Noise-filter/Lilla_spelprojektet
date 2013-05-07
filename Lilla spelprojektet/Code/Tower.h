#ifndef TOWER_H
#define TOWER_H

#include "Structure.h"
#include "Enemy.h"
#include "Projectile.h"
#include "SoundSystem.h"
#include <vector>
using namespace std;

class Tower : public Structure
{
public:
	Tower();
	Tower(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, float damage, float attackSpeed, float range, float projectileSpeed);
	virtual ~Tower();

	virtual int update(float dt);

	void aquireTarget(vector<Enemy*>* enemies);

	vector<RenderData*> getRenderData();
	void giveUpgrade(UpgradeStats &stats);
	void removeUpgrade(UpgradeStats &stats);

	void rotateTop();

private:
	float damage;
	float attackSpeed;
	float range;
	float projectileSpeed;
	float cooldown;
	Enemy* target;

	vector<Projectile*> projectiles;
	
	Sound* sound;

	//Top part of tower
	RenderData* topTower;
	D3DXMATRIX topPointTrans, topRotation, topTrans;

};

#endif