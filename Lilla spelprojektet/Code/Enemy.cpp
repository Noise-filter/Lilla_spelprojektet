#include "Enemy.h"

Enemy::Enemy() : Entity()
{
	this->speed = 0;
	this->damage = 0;
	this->currentWP = 0;

	waypoints.push_back(Waypoint(0, 0));

	target = NULL;
}

Enemy::Enemy(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, float speed, float damage, int quadSize)
	: Entity(pos, meshID, textureID, hp, lightID)
{
	this->speed = speed;
	this->damage = damage;
	this->currentWP = 0;

	waypoints.push_back(Waypoint((int)pos.x/quadSize, (int)pos.z/quadSize));
	trail = ParticleSystem::Getinstance()->addTrail(D3DXVECTOR3(1, 1, 1),this->getPosition(), 1, 0.1f, 0, 1, 1, 1);

	target = NULL;
	attackSpeed = 1;
	cooldown = attackSpeed;
}

Enemy::~Enemy()
{
	ParticleSystem::Getinstance()->removePolicy(trail);
	ParticleSystem::Getinstance()->addDeathExplosion(D3DXVECTOR3(1,0,0), getPosition(), 100, 0.5, 30);
}

int Enemy::update(float dt)
{
	trail->updatePosition(this->getPosition());
	int id = Entity::update(dt);
	if(id == 0)
		return 0;

	id = move(dt);
	if(id == 2)		// kommit fram till m�let/vill k�ra pathFind igen
		return 2;
	
	return 1;
}

int Enemy::move(float dt)
{
	if((int)waypoints.size()-1 > currentWP)
	{
		D3DXVECTOR3 pos = this->getPosition();
		D3DXVECTOR3 target((float)waypoints.at(currentWP+1).x, 0, (float)waypoints.at(currentWP+1).y);
		D3DXVECTOR3 dir = target - pos;

		if(D3DXVec3Length(&dir) < 0.8)
		{
			this->setPosition(target);
			currentWP++;
		}
		else
		{
			D3DXVec3Normalize(&dir, &dir);
			this->setPosition(pos + (dir * dt * speed));
		}
	}
	else
	{
		if(target != NULL && target->isDead())
			target = NULL;

		cooldown -= dt;
		if(target != NULL)
		{
			if(cooldown <= 0)	//Fienden sl�r ett slag 
			{
				target->doDamage(damage);
				cooldown = attackSpeed;
			}
		}
		else	//Fienden har inget target
		{
			return 2;
		}
	}
	return 1;
}

void Enemy::setPath(std::vector<Waypoint>& wp)
{
	waypoints = wp;
	currentWP = 0;
}

Waypoint Enemy::getCurrentWaypoint()
{
	return waypoints.at(currentWP);
}