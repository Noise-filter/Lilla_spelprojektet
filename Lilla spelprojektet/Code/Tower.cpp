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

	sound = SoundSystem::Getinstance()->createSound("plop.mp3");
}

Tower::~Tower()
{
	for(int i = 0; i < (int)projectiles.size(); i++)
		delete projectiles.at(i);
}

int Tower::update(float dt)
{
	int id = Structure::update(dt);
	if(id == 0)
		return 0;

	//Uppdatera projektilerna
	for(int i = 0; i < (int)projectiles.size(); i++)
	{
		if(projectiles.at(i)->update(dt) == 0)
		{
			Projectile* temp = projectiles.at(i);
			projectiles.erase(projectiles.begin() + i);
			delete temp;
		}
	}

	cooldown -= dt;
	if(target != NULL && !target->isDead())
	{
		//Skjuter ett skott
		if(cooldown <= 0)
		{
			projectiles.push_back(new Projectile(getPosition(), 0, 0, 0, 0, target, projectileSpeed, damage));
			cooldown = attackSpeed;
			SoundSystem::Getinstance()->playSound(sound);
		}
	}
	else	//Vill ha ett nytt target
	{
		return 2;
	}

	return 1;
}

void Tower::aquireTarget(vector<Enemy*>* enemies)
{
	D3DXVECTOR3 vec;
	Enemy* t = NULL;
	float closestLength = 1000;
	float length;

	for(int i = 0; i < (int)enemies->size(); i++)
	{
		vec = enemies->at(i)->getPosition() - getPosition();
		length = D3DXVec3Length(&vec);

		if(length < closestLength)
		{
			t = enemies->at(i);
			closestLength = length;
		}
	}

	if(closestLength < range)
	{
		target = t;
	}
}

vector<RenderData*> Tower::getRenderData()
{
	vector<RenderData*> renderData;

	renderData.push_back(&this->renderData);
	for(int i = 0; i < (int)projectiles.size(); i++)
		renderData.push_back(&projectiles.at(i)->getRenderData());

	return renderData;
}