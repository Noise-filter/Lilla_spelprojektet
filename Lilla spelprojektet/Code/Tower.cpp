#include "Tower.h"

Tower::Tower() : Structure()
{
	target = NULL;
	this->damage = 0;
	this->attackSpeed = 0;
	this->range = 0;
	this->projectileSpeed = 0;
	this->cooldown = 0;
}

Tower::Tower(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, float damage, float attackSpeed, float range, float projectileSpeed)
	: Structure(pos, meshID, textureID, hp, lightID)
{
	target = NULL;
	this->damage = damage;
	this->attackSpeed = attackSpeed;
	this->range = range;
	this->projectileSpeed = projectileSpeed;
	this->cooldown = 0;
}

Tower::~Tower()
{
	for(int i = 0; i < projectiles.size(); i++)
		delete projectiles.at(i);
}

int Tower::update(float dt)
{
	cooldown -= dt;
	if(target != NULL)
	{
		//Skjuter ett skott
		if(cooldown <= 0)
		{
			projectiles.push_back(new Projectile(getPosition(), 2, 0, 0, 0, target, projectileSpeed, damage));
			cooldown = attackSpeed;
		}
	}

	//Uppdatera projektilerna
	for(int i = 0; i < projectiles.size(); i++)
	{
		if(projectiles.at(i)->update(dt) == 2)
		{
			Projectile* temp = projectiles.at(i);
			projectiles.erase(projectiles.begin() + i);
			delete temp;
		}
	}

	return 1;
}