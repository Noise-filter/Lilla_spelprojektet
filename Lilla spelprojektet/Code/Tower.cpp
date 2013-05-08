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
	this->xpToNextLvl = 100;
	this->level = 1;
	this->experience = 0;

	sound = SoundSystem::Getinstance()->createSound("plop.mp3");

	topTower = new RenderData(ENTITY_TOWERTOP, 0, this->renderData.worldMat, 0);

	//Top part
	D3DXMatrixTranslation(&topPointTrans, 1.5f, 0, 0);
	D3DXMatrixTranslation(&topTrans, pos.x-2.5f, pos.y, pos.z-0.5f);
	D3DXMatrixIdentity(&topRotation);
	topScale = scale;
	//topPointTrans = scale * topPointTrans;
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
		int ret = projectiles.at(i)->update(dt);
		if(ret == 0)
		{
			Projectile* temp = projectiles.at(i);
			projectiles.erase(projectiles.begin() + i);
			delete temp;
		}
		else if(ret == 2)
		{
			
			Projectile* temp = projectiles.at(i);
			giveXp(temp->getTarget()->getXp());
			projectiles.erase(projectiles.begin() + i);
			delete temp;

		}
	}

	topTower->worldMat = topScale * topPointTrans * topRotation * topTrans;

	cooldown -= dt;
	if(target != NULL)
	{
		rotateTop();
		if(cooldown <= 0)
		{
			if(range > D3DXVec3Length(&(target->getPosition() - getPosition())))
			{
				projectiles.push_back(new Projectile(D3DXVECTOR3(getPosition().x-2, 6, getPosition().z), ENTITY_PROJECTILE, 0, 0, 0, target, projectileSpeed, damage));
				cooldown = attackSpeed;
				SoundSystem::Getinstance()->playSound(sound);
			}
			else	//target har g�tt bortanf�r tornets range.
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

void Tower::giveXp(int xp)
{
	this->experience += xp;
	cout << "xp: " << experience << "/" << xpToNextLvl << endl;
	if(experience >= xpToNextLvl)
	{
		cout << "lvlup!!!!!" << endl;
		lvlUp();
	}
}
void Tower::lvlUp()
{
	this->scaleFactor++;
	D3DXMatrixScaling(&scale,scaleFactor,scaleFactor,scaleFactor);
	D3DXMatrixScaling(&topScale,scaleFactor,scaleFactor,scaleFactor);
	this->attackSpeed -= 0.1f;
	this->cooldown -= 0.1f;
	this->damage += 5;
	this->hp += 5;
	this->maxHp += 5;
	this->projectileSpeed += 0.1f;
	this->xpToNextLvl += 10*level*2;
	this->experience = 0;
	this->level++;
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

//Rotates towards target
void Tower::rotateTop()
{
	D3DXVECTOR3 pos = getPosition();
	pos.y = 0;
	D3DXVECTOR3 look = D3DXVECTOR3(target->getPosition().x, 0, target->getPosition().z) - pos;

	D3DXVec3Normalize(&look, &look);

	float dot = D3DXVec3Dot(&look, &D3DXVECTOR3(-1, 0, 0));
	float yaw = acos(dot);

	if(look.z > 0)
		D3DXMatrixRotationY(&topRotation, yaw);
	else
		D3DXMatrixRotationY(&topRotation, -yaw);
}