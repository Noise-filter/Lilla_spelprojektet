#include "EnemyHandler.h"

EnemyHandler::EnemyHandler(void)
{
	ai = new AI();
}

bool EnemyHandler::init(Structure*** structures, Node** nodes, int mapSize)
{
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

	SAFE_DELETE(ai);
}

int EnemyHandler::update(float dt)
{
	//vector<Enemy*> spawnedEnemies = ai->spawnEnemies(dt,0);

	//for(int i = 0; i < spawnedEnemies.size(); i++)
	//	enemies.push_back(spawnedEnemies.at(i));

	//ai->findPath(1, 99, 1);

	static bool Switch = false;

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
			if(enemies.at(i)->update(dt) == 2)
			{
				vector<Waypoint> wp = ai->findPath(enemies.at(i)->getCurrentWaypoint1D(), rand() % 100, 1);
				if(wp.size() > 0)
					enemies.at(i)->setPath(wp);
			}
		}
	}

	if(enemies.size() < 1000)
	{
		enemies.push_back(new Enemy(D3DXVECTOR3(0, 0, 0), 2, 0, 3, 0, 10, 0));
		vector<Waypoint> wp = ai->findPath(0, rand() % 100, 1);
		if(wp.size() > 0)
			enemies.at(enemies.size()-1)->setPath(wp);
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