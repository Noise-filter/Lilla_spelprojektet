#include "Tower.h"

Tower::Tower() : Structure()
{
	target = NULL;
	this->damage = 0;
	this->attackSpeed = 0;
	this->range = 0;
	this->projectileSpeed = 0;
	this->cooldown = 0;

	D3DXMATRIX world;
	D3DXMatrixTranslation(&world, 0, 70, 0);
	topTower = new RenderData(ENTITY_TOWERTOP, 0, world, 0);
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

	D3DXMATRIX world;
	D3DXMatrixTranslation(&world, pos.x, 0.0, pos.z);
	topTower = new RenderData(ENTITY_TOWERTOP, 0, world, 0);
	temp = world;
}

void Tower::giveUpgrade(UpgradeStats &stats)
{
	this->damage += stats.dmg;
	this->attackSpeed += stats.atkSpeed;
	this->maxHp += stats.hp;
	this->hp += stats.hp;
	this->projectileSpeed += stats.prjSpeed;
	this->range += stats.range;
}

void Tower::removeUpgrade(UpgradeStats &stats)
{
	this->damage -= stats.dmg;
	this->attackSpeed -= stats.atkSpeed;
	this->maxHp -= stats.hp;
	this->hp -= stats.hp;
	this->projectileSpeed -= stats.prjSpeed;
	this->range -= stats.range;	
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

	if(target != NULL && target->isDead())
		target = NULL;

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
	if(target != NULL)
	{
		rotateTop();
		if(cooldown <= 0)
		{
			if(range > D3DXVec3Length(&(target->getPosition() - getPosition())))
			{
				projectiles.push_back(new Projectile(getPosition(), ENTITY_PROJECTILE, 0, 0, 0, target, projectileSpeed, damage));
				cooldown = attackSpeed;
				SoundSystem::Getinstance()->playSound(sound);
			}
			else	//target har gått bortanför tornets range.
			{
				target = NULL;
				return 2;
			}
		}
	}
	else	//Tornet har inget target
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
	renderData.reserve(projectiles.size()+2);

	renderData.push_back(&this->renderData);
	renderData.push_back(topTower);
	for(int i = 0; i < (int)projectiles.size(); i++)
		renderData.push_back(&projectiles.at(i)->getRenderData());

	return renderData;
}

void Tower::rotateTop()
{
	D3DXMATRIX rotation;/*
	D3DXMatrixIdentity(&rotation);
	D3DXMatrixRotationY(&rotation, PI/2);
	topTower->worldMat = temp;
	topTower->worldMat = rotation * topTower->worldMat;
	*/

	D3DXVECTOR3 pos = getPosition();
	look = target->getPosition() - pos;

	up = D3DXVECTOR3(0, 1, 0);
	right = D3DXVECTOR3(0, 0, 1);

	D3DXVec3Normalize(&look, &look);

	float dot = D3DXVec3Dot(&look, &D3DXVECTOR3(-1, 0, 0));
	float yaw = acos(dot);
	topTower->worldMat = temp;

	if(dot > 0)
		D3DXMatrixRotationY(&rotation, yaw);
	else
		D3DXMatrixRotationY(&rotation, -yaw);


	topTower->worldMat = rotation * topTower->worldMat;


	/*

	D3DXVECTOR3 direction = target->getPosition() - getPosition();
	D3DXVec3Normalize(&direction, &direction);
	D3DXMatrixRotationAxis(&rotation, &direction, 1);
	*/

	/*
	D3DXVec3Normalize(&this->look, &this->look);
	D3DXVec3Normalize(&this->up, &this->up);
	D3DXVec3Cross(&this->right, &this->up, &this->look);
	D3DXVec3Cross(&this->up, &this->look, &this->right);
	

	float x = -D3DXVec3Dot(&pos, &right);
	float y = -D3DXVec3Dot(&pos, &up);
	float z = -D3DXVec3Dot(&pos, &look);

	rotation(0, 0) = this->right.x;
	rotation(1, 0) = this->right.y;
	rotation(2, 0) = this->right.z;
	rotation(3, 0) = pos.x;

	rotation(0, 1) = this->up.x;
	rotation(1, 1) = this->up.y;
	rotation(2, 1) = this->up.z;
	rotation(3, 1) = pos.y;

	rotation(0, 2) = this->look.x;
	rotation(1, 2) = this->look.y;
	rotation(2, 2) = this->look.z;
	rotation(3, 2) = pos.z;

	rotation(0, 3) = 0.0f;
	rotation(1, 3) = 0.0f;
	rotation(2, 3) = 0.0f;
	rotation(3, 3) = 1.0f;
	
	D3DXMatrixTranslation(&topTower->worldMat, pos.x, pos.y, pos.z);

	//topTower->worldMat = temp;
	topTower->worldMat *= rotation;*/
}