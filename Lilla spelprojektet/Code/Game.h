#pragma once
#include "Engine.h"

class Game
{
public:
	Game(void);
	~Game(void);

	void render(float deltaTime);
	int update(float deltaTime);
	bool init(HINSTANCE hInstance, int cmdShow);

private:
	Engine* engine;
};

