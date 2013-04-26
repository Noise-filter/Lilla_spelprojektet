#ifndef ENEMY_H
#define ENEMY_H

#include "Structure.h"
#include "Entity.h"
#include <vector>

struct Waypoint
{
	Waypoint(int X, int Y)
	{
		x = X;
		y = Y;
	}

	int x, y;
};

class Enemy : public Entity
{
public:
	Enemy();
	Enemy(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, float speed, float damage);
	~Enemy();

	virtual int update(float dt);

	void setPath(std::vector<Waypoint>& wp);

	int move(float dt);

	Waypoint getCurrentWaypoint();
	int getCurrentWaypoint1D();


private:
	//Waypoints
	std::vector<Waypoint> waypoints;
	int currentWP;

	Structure* target;

	float speed;
	float damage;

};

#endif