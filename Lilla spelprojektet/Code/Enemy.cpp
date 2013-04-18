#include "Enemy.h"

Enemy::Enemy() : Entity()
{
	this->speed = 0;
	this->damage = 0;
	this->currentWP = 0;
	dead = false;
}

Enemy::Enemy(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, float speed, float damage)
	: Entity(pos, meshID, textureID, hp, lightID)
{
	this->speed = speed;
	this->damage = damage;
	this->currentWP = 0;
	dead = false;
}

Enemy::~Enemy()
{}

int Enemy::update(float dt)
{
	if(hp <= 0)
		dead = true;

	return 1;
}

bool Enemy::isDead()
{
	return dead;
}