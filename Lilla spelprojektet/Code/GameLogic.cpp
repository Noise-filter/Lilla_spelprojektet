#include "GameLogic.h"


GameLogic::GameLogic(void)
{
	this->level = new Level();
	this->eHandler = new EnemyHandler();
}


GameLogic::~GameLogic(void)
{
	SAFE_DELETE(this->level);
	SAFE_DELETE(this->eHandler);
}

int GameLogic::update(float dt)
{
	if(level->update(dt))
		return 0; //error
	if(eHandler->update(dt))
		return 0; //error


	return 1;//all went good
}

bool GameLogic::init(int mapSize, int quadSize)
{
	this->level->init(mapSize,quadSize);
	//this->eHandler->init(); does not exist yet, need to add this

	return true;
}

vector<vector<RenderData*>> GameLogic::getRenderData()
{
	rDataList.clear();

	rDataList.push_back(level->getRenderData());
	//rDataList.push_back(eHandler->getRenderData());


	return rDataList;	
}