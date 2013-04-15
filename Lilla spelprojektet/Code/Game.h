#pragma once
#include "Engine.h"
#include "GameLogic.h"

class Game
{
public:
	Game(void);
	~Game(void);

	void render();
	int update(float dt);
	bool init(HINSTANCE hInstance, int cmdShow);

private:
	Engine* engine;
	GameLogic* gameLogic;
};

