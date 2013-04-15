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

bool GameLogic::init(int mapSize)
{
	this->level->init(mapSize);
	//this->eHandler->init(); does not exist yet, need to add this

	return true;
}

RenderData* GameLogic::getRenderData()
{
	return this->level->getRenderData();	
}