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
	~Tower();

	int update(float dt);

	void aquireTarget(vector<Enemy*>* enemies);
	void giveUpgrade(UpgradeStats &stats);
	void removeUpgrade(UpgradeStats &stats);
	bool rotateTop();
	
	vector<RenderData*> getRenderData();

private:
	void giveXp(int xp);
	void lvlUp();

private:
	float damage;
	float attackSpeed;
	float range;
	float projectileSpeed;
	float cooldown;
	int experience;
	int level;
	int xpToNextLvl;

	Enemy* target;
	vector<Projectile*> projectiles;
	Sound* sound;

	//Top part of tower
	RenderData* topTower;
	D3DXMATRIX topScale, topPointTrans, topRotation, topTrans;
	float rotationSpeed;
	float rotY;
	D3DXVECTOR3 look;


};

#endif