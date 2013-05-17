#ifndef ENEMY_H
#define ENEMY_H

#include "Structure.h"
#include "Entity.h"
#include <vector>
#include "ParticleSystem.h"

struct Waypoint
{
	Waypoint()
	{
		x = y = 0;
	}

	Waypoint(int X, int Y)
	{
		x = X;
		y = Y;
	}

	short x, y;
};

class Enemy : public Entity
{
public:
	Enemy();
	Enemy(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, float speed, float damage, int xp);
	~Enemy();

	int update(float dt);
	int move(float dt);

	void setPath(std::vector<Waypoint>& wp);
	void setTarget(Structure* t);

	Waypoint getCurrentWaypoint();
	int getXp();
	HPBarInfo getHPBarInfo();

private:
	//Waypoints
	Trail* trail;
	std::vector<Waypoint> waypoints;
	int currentWP;

	Structure* target;
	float cooldown;
	float speed;
	float damage;
	float attackSpeed;
	float targetUpdateTime;
	int xp;
};

#endif