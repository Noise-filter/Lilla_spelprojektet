#include "EnemyHandler.h"

EnemyHandler::EnemyHandler(void)
{
}

bool EnemyHandler::init()
{
	int x, z;
	for(int i = 0; i < 10; i++)
	{
		x = rand() % 100;
		z = rand() % 100;

		enemies.push_back(new Enemy(D3DXVECTOR3(x, 0, z), 2, 0, 10, 0, 0, 0));
	}

	return true;
}

EnemyHandler::~EnemyHandler(void)
{
	for(int i = 0; i < (int)enemies.size(); i++)
		delete enemies.at(i);
}

int EnemyHandler::update(float dt)
{
	for(int i = 0; i < (int)enemies.size(); i++)
	{
		if(enemies.at(i)->isDead())
		{
			Enemy* temp = enemies.at(i);
			enemies.erase(enemies.begin() + i);
			SAFE_DELETE(temp);
		}
		else
		{
			int id = enemies.at(i)->update(dt);
		}
	}

	if(enemies.size() < 10)
	{
		enemies.push_back(new Enemy(D3DXVECTOR3(rand() % 100, 0, rand() % 100), 2, 0, 10, 0, 0, 0));
	}


	return 1;
}

vector<Enemy*>& EnemyHandler::getEnemies()
{
	return enemies;
}

void EnemyHandler::getRenderData(vector<vector<RenderData*>>& rData)
{
	for(int i = 0; i < (int)enemies.size(); i++)
		rData.at(enemies.at(i)->getRenderData().meshID).push_back(&enemies.at(i)->getRenderData());
}