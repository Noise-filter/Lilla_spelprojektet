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

RenderData* GameLogic::getRenderData()
{
	return NULL;	
}