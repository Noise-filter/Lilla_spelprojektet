#pragma once
#include "Engine.h"
#include "GameLogic.h"
#include "Input.h"
#include "Camera.h"
#include "SoundSystem.h"
#include "ParticleSystem.h"
#include "GUI.h"
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

private:
	Engine* engine;
	GameLogic* gameLogic;
	Input* input;
	Camera* camera;
	SoundSystem* soundSystem;
	ParticleSystem* pSystem;
	Sound* playlist;
	GUI* GUI;
	GAMESTATES state;


	int gameState;

};

