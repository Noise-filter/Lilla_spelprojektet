#pragma once
#include "Engine.h"
#include "GameLogic.h"
#include "Input.h"
#include "Camera.h"
#include "SoundSystem.h"
#include "ParticleSystem.h"
#include <fstream>



using namespace std;
class Game
{
public:
	Game(void);
	~Game(void);

	void render();
	int update(float dt);
	bool init(HINSTANCE hInstance, int cmdShow);
	void handleInput(float dt);

private:
	GameSettings readSettingsFromFile(string fileName);
	void changeState();

private:
	Engine* engine;
	GameLogic* gameLogic;
	Input* input;
	Camera* camera;
	SoundSystem* soundSystem;
	ParticleSystem* pSystem;
	Sound* playlist;
	Sound* menuSound;
	GUI* gui;

	int gameState;
	int oldGameState;
	int pausedGameStateSaved;
};

