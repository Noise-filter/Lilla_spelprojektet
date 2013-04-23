#include "Game.h"


Game::Game(void)
{
	engine = new Engine();
}


Game::~Game(void)
{
	SAFE_DELETE(engine);
}

bool Game::init(HINSTANCE hInstance, int cmdShow)
{
	if(!engine->init(hInstance,cmdShow))
	{
		return false;
	}

	//initiate other game resources such as level or whatever






	//initiate other game resources such as level or whatever


	return true; // all initiates went well
}


void Game::render()
{
	//build engines renderContent with addRenderData then do render to execute those renders
	std::vector<std::vector<RENDERDATA*>> data;

	engine->render(data);
}

int Game::update(float deltaTime)
{
	return 1;
}