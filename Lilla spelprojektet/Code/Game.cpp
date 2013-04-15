#include "Game.h"


Game::Game(void)
{
	engine = new Engine();
	gameLogic = new GameLogic();
}


Game::~Game(void)
{
	SAFE_DELETE(engine);
	SAFE_DELETE(gameLogic);
}

bool Game::init(HINSTANCE hInstance, int cmdShow)
{
	if(!engine->init(hInstance,cmdShow))
		return false;

	//initiate other game resources such as level or whatever

	if(!gameLogic->init(10))
		return false;






	//initiate other game resources such as level or whatever


	return true; // all initiates went well
}


void Game::render()
{
	//build engines renderContent with addRenderData then do render to execute those renders
	delete gameLogic->getRenderData();

	engine->render();
}

int Game::update(float dt)
{
	if(gameLogic->update(dt))
		return 0; // error


	return 1;

}