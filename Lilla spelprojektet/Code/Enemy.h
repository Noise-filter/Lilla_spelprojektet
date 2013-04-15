#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy();
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