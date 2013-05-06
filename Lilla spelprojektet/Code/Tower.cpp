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
	D3DXMatrixTranslation(&world, getPosition().x, getPosition().y, getPosition().z);
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

	D3DXMATRIX world = this->renderData.worldMat;
	topTower = new RenderData(ENTITY_TOWERTOP, 0, world, 0);

	temp = topTower->worldMat;
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
	SAFE_DELETE(topTower);
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
	D3DXMATRIX rotation;
	D3DXVECTOR3 pos = getPosition();
	D3DXMATRIX translation;
	D3DXMATRIX scale;
	D3DXMATRIX translation2;
	D3DXMatrixTranslation(&translation, 3, 0, 0);
	D3DXMatrixTranslation(&translation2, pos.x-2, pos.y, pos.z);
	D3DXMatrixScaling(&scale, 2, 2, 2);
	D3DXMatrixTranslation(&rotation, 0, 0, 0);

	pos.y = 0;
	look = D3DXVECTOR3(target->getPosition().x, 0, target->getPosition().z) - pos;

	up = D3DXVECTOR3(0, 1, 0);
	right = D3DXVECTOR3(0, 0, 1);
	D3DXVec3Normalize(&look, &look);

	float dot = D3DXVec3Dot(&look, &D3DXVECTOR3(-1, 0, 0));
	float yaw = acos(dot);
	topTower->worldMat = temp;

	if(look.z > 0)
		D3DXMatrixRotationY(&rotation, yaw);
	else
		D3DXMatrixRotationY(&rotation, -yaw);

	topTower->worldMat = scale * translation * rotation * translation2;

	//topTower->worldMat = rotation * topTower->worldMat;
}