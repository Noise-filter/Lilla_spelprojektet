#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy();
	Enemy(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, float speed, float damage);
	~Enemy();

	virtual int update(float dt);

private:
	//Waypoints
	//vector<Waypoint> waypoints;
	int currentWP;
	float speed;
	float damage;

};

#endif