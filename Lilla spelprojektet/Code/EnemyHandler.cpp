#include "EnemyHandler.h"

EnemyHandler::EnemyHandler(void)
{
	ai = new AI();
}

bool EnemyHandler::init(Structure*** structures, Node** nodes, int mapSize, int quadSize,int enemiesPerMin,int difficulty)
{
	string scripts[3];
	scripts[0] = "astar.lua";
	scripts[1] = "targetFind.lua";
	scripts[2] = "spawningBosses.lua";

	ai->init(structures, nodes, scripts, mapSize, quadSize, enemiesPerMin, difficulty);

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
	vector<Enemy*> spawnedEnemies = ai->spawnEnemies(dt,enemies.size());
	int nrOfKilledEnemies = 0;

	for(int i = 0; i < (int)spawnedEnemies.size(); i++)
		enemies.push_back(spawnedEnemies.at(i));

	for(int i = 0; i < (int)enemies.size(); i++)
	{
		if(enemies.at(i) && enemies.at(i)->isDead())
		{
			nrOfKilledEnemies++; 
			SAFE_DELETE(enemies.at(i));
			enemies.erase(enemies.begin() + i);
		}
		else
		{
			if(enemies.at(i)->update(dt) == 2)
			{
				vector<float> target = ai->findTarget(enemies.at(i)->getCurrentWaypoint(), 1);
				float x = target[0];
				float y = target[1];
				enemies.at(i)->setTarget(ai->getStrucutre((int)x, (int)y, target[2]));
				Waypoint w = enemies.at(i)->getCurrentWaypoint();
				vector<Waypoint> wp = ai->findPath(enemies.at(i)->getCurrentWaypoint(), Waypoint((int)x, (int)y), 1);
				if(wp.size() > 0)
					enemies.at(i)->setPath(wp);
			}
		}
	}

	return nrOfKilledEnemies;
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

void EnemyHandler::getHPBarInfo(vector<HPBarInfo>& hpBars)
{
	for(int i = 0; i < (int)enemies.size(); i++)
	{
		if(!enemies.at(i)->isDead())
			hpBars.push_back(enemies.at(i)->getHPBarInfo());
	}
}