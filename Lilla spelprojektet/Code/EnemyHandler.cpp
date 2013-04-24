#include "EnemyHandler.h"

EnemyHandler::EnemyHandler(void)
{
	ai = new AI();
}

bool EnemyHandler::init(Structure*** structures, Node** nodes, int mapSize)
{
	int x, z;
	for(int i = 0; i < 10; i++)
	{
		x = rand() % 100;
		z = rand() % 100;

		enemies.push_back(new Enemy(D3DXVECTOR3(x, 0, z), 2, 0, 10, 0, 0, 0));
	}
	
	string scripts[3];
	scripts[0] = "astar.lua";
	scripts[1] = "targetFind.lua";
	scripts[2] = "spawning.lua";

	ai->init(structures, nodes, scripts, mapSize);

	return true;
}

EnemyHandler::~EnemyHandler(void)
{
	for(int i = 0; i < (int)enemies.size(); i++)
		delete enemies.at(i);
}

int EnemyHandler::update(float dt)
{
	vector<Enemy*> spawnedEnemies = ai->spawnEnemies(dt,0);

	for(int i = 0; i < spawnedEnemies.size(); i++)
		enemies.push_back(spawnedEnemies.at(i));

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
			enemies.at(i)->update(dt);
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