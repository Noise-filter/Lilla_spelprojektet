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

		enemies.push_back(new Enemy(D3DXVECTOR3(x, 0, z), 2, 0, 0, 0, 0, 0));
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
		enemies.at(i)->update(dt);

	return 1;
}

vector<RenderData*> EnemyHandler::getRenderData()
{
	renderData.clear();

	for(int i = 0; i < (int)enemies.size(); i++)
		renderData.push_back(&enemies.at(i)->getRenderData());

	return renderData;
}