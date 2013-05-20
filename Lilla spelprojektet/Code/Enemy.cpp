#include "Enemy.h"

Enemy::Enemy() : Entity()
{
	this->speed = 0;
	this->damage = 0;
	this->currentWP = 0;

	waypoints.push_back(Waypoint(0, 0));

	target = NULL;
}

Enemy::Enemy(Vec3 pos, int meshID, int textureID, float hp, int lightID, float speed, float damage, int xp)
	: Entity(pos, meshID, textureID, hp, lightID)
{
	this->speed = speed;
	this->damage = damage;
	this->currentWP = 0;
	this->xp = xp;

	waypoints.push_back(Waypoint((int)pos.x, (int)pos.z));
	trail = ParticleSystem::Getinstance()->addTrail(Vec3(1, 1, 1),this->getPosition(), 1, 0.1f, 0, 1, 1, 1);

	target = NULL;
	attackSpeed = 1;
	cooldown = attackSpeed;
	targetUpdateTime = 3;

	scaleFactor = 0.3f;
	D3DXMatrixScaling(&scale, scaleFactor, scaleFactor, scaleFactor);
}

Enemy::~Enemy()
{
	ParticleSystem::Getinstance()->removePolicy(trail);
	ParticleSystem::Getinstance()->addDeathExplosion(Vec3(1,0,0), getPosition(), 100, 0.5, 30);
}

int Enemy::update(float dt)
{
	targetUpdateTime -= dt;
	trail->updatePosition(this->getPosition());
	int id = Entity::update(dt);
	if(id == 0)
		return 0;

	id = move(dt);
	if(id == 2)		// kommit fram till målet/vill köra pathFind igen
		return 2;

	if(targetUpdateTime <= 0 && (int)waypoints.size() > currentWP)
	{
		targetUpdateTime = 3;
		return 2;
	}

	return 1;
}

int Enemy::move(float dt)
{
	if(target != NULL && target->isDead())
		target = NULL;

	if((int)waypoints.size() > currentWP && target != NULL)
	{
		Vec3 pos = this->getPosition();
		Vec3 target((float)waypoints.at(currentWP).x, 0, (float)waypoints.at(currentWP).y);
		Vec3 dir = target - pos;

		if(D3DXVec3Length(&dir) < 0.5)
		{
			//this->setPosition(target);	// Gör så att fienderna hoppar till mitten av noderna när de är en viss längd ifrån dem.
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
		cooldown -= dt;
		if(target != NULL)
		{
			if(cooldown <= 0)	//Fienden slår ett slag 
			{
				target->doDamage(damage);
				cooldown = attackSpeed;
				Vec3 dir = target->getPosition() - getPosition();
				D3DXVec3Normalize(&dir, &dir);
				ParticleSystem::Getinstance()->addAttackParticlePolicy(Vec3(1, 1, 1), getPosition(), 60, 0.8f, 10, dir);
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
int Enemy::getXp()
{
	return this->xp;
}

Waypoint Enemy::getCurrentWaypoint()
{
	if(currentWP > (int)waypoints.size()-1)
	{
		return waypoints.at(currentWP-1);
	}
	return waypoints.at(currentWP);
}

void Enemy::setTarget(Structure* t)
{
	target = t;
}

HPBarInfo Enemy::getHPBarInfo()
{
	HPBarInfo info(translate, hp/maxHp);
	info.translate._41 -= scale._11*10;
	return info;
}