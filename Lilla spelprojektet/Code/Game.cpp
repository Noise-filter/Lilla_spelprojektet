#include "Game.h"

Game::Game(void)
{
	engine = new Engine();
	gameLogic = NULL;
	input = new Input();
	camera = new Camera();
	soundSystem = soundSystem->Getinstance();
	pSystem = pSystem->Getinstance();
	gameState = STATE_MENU;
	oldGameState = STATE_MENU;
	pausedGameStateSaved = STATE_MENU;
	gui = new GUI();
	
}

Game::~Game(void)
{
	SAFE_DELETE(engine);
	SAFE_DELETE(gameLogic);
	SAFE_DELETE(camera);
	SAFE_DELETE(input);
	soundSystem->shutdown();
	SAFE_DELETE(playlist);
	SAFE_DELETE(gui);
	pSystem->shutdown();
}

bool Game::init(HINSTANCE hInstance, int cmdShow)
{

	//read settings from file
	settings = readSettingsFromFile("settings.txt");

	//use settings for the game
	this->hInstance = hInstance;
	this->cmdShow = cmdShow;

	if(!engine->init(hInstance,cmdShow))
		return false;

	soundSystem->init();
	playlist = soundSystem->createPlaylist("playlist.m3u");
	menuSound = soundSystem->createStream("SeductressDubstep_Test.mp3");
	soundSystem->playSound(menuSound);
	



	camera->LookAt(Vec3(50,40,45), Vec3(35, 0, 45), Vec3(-1, 0, 0));
	camera->SetLens((float)D3DX_PI * 0.45f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
	camera->UpdateViewMatrix();

	gameState = STATE_MENU;

	return true; // all initiates went well
}

void Game::render()
{
	Button* btns;
	Text* text;
	Text* temp;
	int tempSize;
	gui->render(btns, text);

	int nrOfBtns = gui->getNrOfBtns();
	int nrOfBoxes = gui->getNrOfText();

	tempSize = nrOfBtns + nrOfBoxes;
	temp = new Text[tempSize];
	for(int i = 0; i < nrOfBoxes; i++)
	{
		temp[i] = text[i];
	}
	for(int i = nrOfBoxes; i < nrOfBtns + nrOfBoxes; i++)
	{
		temp[i] = btns[i-nrOfBoxes].text;
	}
	
	GUI_Panel* panels = gui->getPanels();
	int nrOfPanels = gui->getNrOfPanels();
	

	//build engines renderContent with addRenderData then do render to execute those renders
	
	if(gameState == STATE_GAMESTART || gameState == STATE_PLAYING)
	{
		engine->setRenderData(gameLogic->getRenderData());

		engine->setRenderData(pSystem->getVertexData());
		//if(nrOfPanels != 0)
		{
			engine->setGUI(panels, nrOfPanels);
		}

		//Get hp bars and put them in the correct position
		vector<HPBarInfo> hp = gameLogic->getHPBarInfo();
		Vec3 pos;
		Vec3 cameraPosition = camera->GetPosition();
		for(int i = 0; i < (int)hp.size(); i++)
		{
			pos.x = hp[i].translate._41;
			pos.y = hp[i].translate._42;
			pos.z = hp[i].translate._43;

			Vec3 toCam, right, up;
			toCam = cameraPosition - pos;
			D3DXVec3Normalize(&toCam, &toCam);
			D3DXVec3Cross(&right, &camera->GetUp(), &toCam);
			D3DXVec3Cross(&up, &toCam, &right);
			right = Vec3(0, 0, -1);
			Matrix orient(right.x,right.y, right.z, 0, up.x, up.y, up.z, 0, toCam.x, toCam.y, toCam.z, 0, 0, 0, 0, 1);

			float x = cameraPosition.x - pos.x;
			float y = cameraPosition.y - pos.y;
			float z = cameraPosition.z - pos.z;
			hp[i].translate._41 -= x * 0.1;
			hp[i].translate._43 -= z * 0.1;
			hp[i].translate = orient * hp[i].translate;
		}
		engine->setHPBars(hp);

		engine->render(camera->View(), camera->Proj(), temp, tempSize, camera->GetPosition());
	}
	else //if(gameState == STATE_MENU || gameState == STATE_SETTINGS || gameState == STATE_NEWGAME || gameState == STATE_PAUSED || gameState == STATE_WIN || gameState == STATE_LOSE)
	{
		engine->setGUI(panels, nrOfPanels);
		engine->renderGui(temp, tempSize);
	}
	delete[] temp;
	temp = NULL;
}

int Game::update(float dt)
{
	static bool muted = false;
	handleInput(dt);
	soundSystem->update();

	if(gameState == STATE_MENU || gameState == STATE_NEWGAME || gameState == STATE_PAUSED || gameState == STATE_WIN || gameState == STATE_LOSE)
	{
		soundSystem->setPaused(playlist, true);
	}
	if(gameState == STATE_PLAYING || gameState == STATE_GAMESTART )
	{
		camera->UpdateViewMatrix();
		if(!gameLogic->update(gameState, dt,input->getMs(), camera->View(), camera->Proj(), camera->GetPosition()))
			return 0; // error
		
		pSystem->update(dt);
		int resource = gameLogic->getResource();
		int supply = gameLogic->getSupply();
		float currPercent = gameLogic->getCurrPercent();
		float winPercent = gameLogic->getWinPercent();
		int selectedStructure = gameLogic->getSelectedBuilding();
		gui->setInGameText(resource, supply, currPercent, winPercent, selectedStructure);
		

	}
	//if(gameState == STATE_WIN) 
	//{
	//	cout << "YOU WON" << endl;
	//}
	//else if(gameState == STATE_LOSE)
	//{
	//	//hantera win/lose state
	//	cout << "YOU LOSE" << endl;
	//}
	else if(gameState == STATE_QUIT)
	{
		return 0;
	}
	bool retry;
	gui->update(input->getMs(), gameState, muted, retry);
	if(oldGameState != gameState)
	{
		changeState(retry);
		oldGameState = gameState;
	}
	
	soundSystem->setMute(muted);
	input->resetBtnState();
	char title[255];
	sprintf_s(title, "%f", 1/dt);
	SetWindowTextA(engine->getHWND(), title);

	return 1;
}

void Game::changeState(bool retry)
{
	if((gameState == STATE_MENU || gameState == STATE_PAUSED || gameState == STATE_WIN || gameState == STATE_LOSE) && (oldGameState != STATE_NEWGAME && oldGameState != STATE_SETTINGS && oldGameState != STATE_PAUSED))
	{
		menuSound = soundSystem->createStream("SeductressDubstep_Test.mp3");
		soundSystem->playSound(menuSound);
	}
	if(gameState == STATE_GAMESTART && retry == false && oldGameState != STATE_PAUSED)
	{
		newLevel(gui->getCurrentLevel(), gui->getCurrentDiff());
	}
	if((gameState == STATE_GAMESTART || (gameState == STATE_PLAYING && oldGameState != STATE_GAMESTART)) && retry == false)
	{
		soundSystem->stopSound(menuSound);
		soundSystem->setPaused(playlist, false);
	}
	if(oldGameState == STATE_PAUSED && (gameState == STATE_PLAYING || gameState == STATE_GAMESTART))
	{
		gameState = pausedGameStateSaved;
	}
	if(gameState == STATE_GAMESTART && retry == true)
	{
		soundSystem->stopSound(menuSound);
		soundSystem->setPaused(playlist, false);
		retrylevel(gui->getCurrentLevel(), gui->getCurrentDiff());
	}
}

void Game::handleInput(float dt)
{
	static bool pausedMusic = true;

	input->updateMs(engine->getMouseState());
	

	
	if(gameState == STATE_GAMESTART || gameState == STATE_PLAYING) 
	{
		if(input->checkMovement('W') && camera->GetPosition().x > (gameLogic->getMapSize()-5 * -1))	//W
			camera->Walk(-100.0f * dt);

		if(input->checkMovement('A') && camera->GetPosition().z > (gameLogic->getMapSize()-5 * -1))	//A
			camera->Strafe(-100.0f * dt);

		if(input->checkMovement('S') && camera->GetPosition().x < (gameLogic->getMapSize()) * 10)	//S
			camera->Walk(100.0f * dt);

		if(input->checkMovement('D') && camera->GetPosition().z < (gameLogic->getMapSize()) * 10)	//D
			camera->Strafe(100.0f * dt);

		if(input->checkKeyDown(VK_ESCAPE))
		{
			pausedGameStateSaved = gameState;
			gameState = STATE_PAUSED;
		}
	}

	if(input->checkKeyDown(0x20))	//Space
	{
		soundSystem->setPaused(pausedMusic);
		pausedMusic = !pausedMusic;
	}

	if(gameState == STATE_PLAYING)
	{
		if(input->checkKeyDown(0x31))
		{
			gameLogic->setSelectedStructure(1);//supply
		}
		if(input->checkKeyDown(0x32))
		{
			gameLogic->setSelectedStructure(2);//tower
		}
		if(input->checkKeyDown(0x33))
		{
			gameLogic->setSelectedStructure(3);//offensive
		}
		if(input->checkKeyDown(0x34))
		{
			gameLogic->setSelectedStructure(4);//defensive
		}
		if(input->checkKeyDown(0x35))
		{
			gameLogic->setSelectedStructure(5);//resourse
		}
		if(input->checkKeyDown(0x45)) // E
		{
			//byt byggnad +1
			gameLogic->incrementSelectedStructure(1);
		}

		if(input->checkKeyDown(0x51)) // Q
		{
			//byt byggnad -1
			gameLogic->incrementSelectedStructure(-1);
		}	
	}
}

GameSettings Game::readSettingsFromFile(string fileName)
{
	GameSettings settings;
	ifstream fin;
	fin.open(fileName);
	string attribute;
	int value;

	if(fin.fail() == true)
	{
		cout << "FAILED TO READ SETTINGS FROM FILE" << endl;
	}

	while(!fin.eof())
	{
		fin >> attribute >> value;
		
		if(attribute == "enemiesPerMin")	
		{
			settings.enemiesPerMin = value;
		}
		if(attribute == "difficulty")	
		{
			settings.difficulty = value;
		}
		if(attribute == "resPerTick")	
		{
			settings.resPerTick = value;
		}
		if(attribute == "resCD")	
		{
			settings.resCD = value;
		}
	}

	//läs från fil istället

	fin.close();
	return settings;
}

void Game::newLevel(string filename, int difficulty)
{
	
	SAFE_DELETE(gameLogic);
	pSystem->shutdown();

	gameLogic = new GameLogic();
	pSystem = pSystem->Getinstance();


	loadlevel(filename+".txt", difficulty);

	int mapSize = gameLogic->getMapSize();

	engine->start(mapSize);

}

void Game::loadlevel(string filename, int difficulty)
{	
	gameLogic->init(10,settings, filename, difficulty);
}

void Game::retrylevel(string filename, int difficulty)
{	
	SAFE_DELETE(gameLogic);
	pSystem->shutdown();
	

	gameLogic = new GameLogic();
	pSystem = pSystem->Getinstance();
	gui->restartStats();

	loadlevel(filename+".txt", difficulty);
}