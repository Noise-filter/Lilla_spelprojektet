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

Tower::Tower(Vec3 pos, int meshID, int textureID, float hp, int lightID, float damage, float attackSpeed, float range, float projectileSpeed, bool fakeBuilding)
	: Structure(pos, meshID, textureID, hp, lightID, fakeBuilding)
{
	target = NULL;
	this->damage = damage;
	this->attackSpeed = attackSpeed;
	this->range = range;
	this->projectileSpeed = projectileSpeed;
	this->cooldown = 0;
	this->xpToNextLvl = 300;
	this->level = 1;
	this->experience = 0;

	sound = SoundSystem::Getinstance()->createSound("Swoosh_test.mp3");

	topTower = new RenderData(ENTITY_TOWERTOP, textureID, this->renderData.worldMat, 0);

	//Top part
	D3DXMatrixTranslation(&topPointTrans, 2, 0, 0);
	D3DXMatrixTranslation(&topTrans, pos.x-0.2, pos.y, pos.z);
	D3DXMatrixIdentity(&topRotation);
	D3DXMatrixTranslation(&translate, pos.x+2.1f, pos.y, pos.z);
	//scaleFactor = 1;
	D3DXMatrixScaling(&scale, scaleFactor, scaleFactor, scaleFactor);
	topScale = scale;

	rotationSpeed = 7.0f;
	rotY = 0;
	oldRotY = 0;
	
	renderData.worldMat = scale * pointTranslate * rotation * translate;
	topTower->worldMat = topScale * topPointTrans * topRotation * topTrans;
}

void Tower::setTextureID(int ID)
{
	renderData.textureID = ID;
	topTower->textureID = ID;
}

void Tower::giveUpgrade(UpgradeStats &stats)
{
	this->damage += stats.dmg;
	this->attackSpeed -= stats.atkSpeed;
	this->maxHp += stats.hp;
	this->hp += stats.hp;
	this->range += stats.range;

	if(attackSpeed < 0.8f)
		attackSpeed = 0.8f;

}

void Tower::removeUpgrade(UpgradeStats &stats)
{
	this->damage -= stats.dmg;
	this->attackSpeed += stats.atkSpeed;
	this->maxHp -= stats.hp;
	this->hp -= stats.hp;
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
			i--;
			delete temp;
		}
		else if(ret == 2)
		{
			Projectile* temp = projectiles.at(i);
			giveXp(temp->getTarget()->getXp());
			projectiles.erase(projectiles.begin() + i);
			i--;
			delete temp;
		}
	}

	topTower->worldMat = topScale * topPointTrans * topRotation * topTrans;

	cooldown -= dt;
	if(target != NULL)
	{
		if(rotateTop(dt))
		{
			if(cooldown <= 0)
			{
				if(range > D3DXVec3Length(&(target->getPosition() - getPosition())))
				{
					projectiles.push_back(new Projectile(Vec3(getPosition().x-2, 6, getPosition().z), ENTITY_PROJECTILE, 0, 0, 0, target, projectileSpeed, damage));
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
	}
	else	//Tornet har inget target
	{
		return 2;
	}

	return 1;
}

void Tower::giveXp(int xp)
{
	if(this->level != 3) //max level 3
	{
		this->experience += xp;
		cout << "xp: " << experience << "/" << xpToNextLvl << endl;
		if(experience >= xpToNextLvl)
		{
			cout << "lvlup!!!!!" << endl;
			lvlUp();
		}	
	}
	
}
void Tower::lvlUp()
{
	this->attackSpeed -= 0.1f;
	this->damage += 5;
	this->hp += 5;
	this->maxHp += 5;
	this->xpToNextLvl += xpToNextLvl/2;
	this->experience = 0;
	this->level++;
	this->renderData.textureID++;
	this->topTower->textureID++;
	if(level == 3)
	{
		Statistics::Getinstance()->totalNrOfMaxLvlTowers++;
	}
}

void Tower::aquireTarget(vector<Enemy*>* enemies)
{
	Vec3 vec;
	Enemy* t = NULL;
	float closestLength = 99999;
	float length;

	for(int i = 0; i < (int)enemies->size(); i++)
	{
		if(enemies->at(i) && !enemies->at(i)->isDead())
		{
			vec = enemies->at(i)->getPosition() - getPosition();
			length = D3DXVec3Length(&vec);

			if(length < closestLength)
			{
				t = enemies->at(i);
				closestLength = length;
			}
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
bool Tower::rotateTop(float dt)
{
	bool done = false;
	Vec3 pos = getPosition();
	Vec3 targetPos = target->getPosition();

	float rotation = atan2(targetPos.x - pos.x, targetPos.z - pos.z);
	rotation += PI/2;

	if(rotY - rotation > PI)
		rotY += rotationSpeed * dt;
	else if(rotY - rotation < -PI)
		rotY -= rotationSpeed * dt;
	else if(rotY < rotation)
		rotY += rotationSpeed * dt;
	else if(rotY > rotation)
		rotY -= rotationSpeed * dt;

	if(rotY >= PI*2 - PI/2 && oldRotY < PI*2 - PI/2)
		rotY -= PI*2;

	else if(rotY <= 0 - PI/2 && oldRotY > 0 - PI/2)
		rotY += PI*2;

	if(abs(rotation - rotY) < rotationSpeed * 1.4 * dt)
	{
		rotY = rotation;
		done = true;
	}
	oldRotY = rotY;

	D3DXMatrixRotationY(&topRotation, rotY);

	return done;
}