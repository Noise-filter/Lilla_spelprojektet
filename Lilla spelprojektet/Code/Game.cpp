#include "Game.h"

Game::Game(void)
{

	engine = new Engine();
	gameLogic = new GameLogic();
	input = new Input();
	camera = new Camera();
	soundSystem = soundSystem->Getinstance();
	pSystem = pSystem->Getinstance();
	gameState = STATE_MENU;
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
	pSystem->shutdown();

}

bool Game::init(HINSTANCE hInstance, int cmdShow)
{

	//read settings from file
	GameSettings settings = readSettingsFromFile("settings.txt");

	//use settings for the game
	


	if(!engine->init(hInstance,cmdShow))
		return false;

	soundSystem->init();
	playlist = soundSystem->createPlaylist("playlist.m3u");
	//initiate other game resources such as level or whatever

	if(!gameLogic->init(10,settings))
		return false;

	camera->LookAt(D3DXVECTOR3(45,45,45), D3DXVECTOR3(35, 0, 45), D3DXVECTOR3(-1, 0, 0));
	camera->SetLens((float)D3DX_PI * 0.45f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);

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

	if(nrOfBtns != 0)
	{
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
		
	}
	else
	{ 
		tempSize = nrOfBoxes;
		temp = new Text[tempSize];
		for(int i = nrOfBtns; i < nrOfBoxes; i++)
		{
			temp[i] = text[i];
		}
	}

	//build engines renderContent with addRenderData then do render to execute those renders
	engine->setRenderData(gameLogic->getRenderData());

	engine->setRenderData(pSystem->getVertexData());

	engine->render(camera->ViewsProj(), temp, tempSize);

	//engine->renderGui(gameState);
	
	delete temp;
	temp = NULL;

}

int Game::update(float dt)
{
	handleInput(dt);
	if(gameState == STATE_PLAYING || gameState == STATE_GAMESTART )
	{
		
		camera->UpdateViewMatrix();
		if(!gameLogic->update(gameState, dt,input->getMs(), camera->View(), camera->Proj(), camera->GetPosition()))
			return 0; // error
		
		pSystem->update(dt);
	}
	 if(gameState == STATE_WIN) 
	{
		cout << "YOU WON" << endl;
	}
	else if(gameState == STATE_LOSE)
	{
		//hantera win/lose state
		cout << "YOU LOSE" << endl;
	}
	
	gui->update(input->getMs(), gameState);

	input->resetBtnState();
	char title[255];
	sprintf_s(title, "%f", 1/dt);
	SetWindowTextA(engine->getHWND(), title);

	return 1;
}

void Game::handleInput(float dt)
{
	static bool pausedMusic = true;

	input->updateMs(engine->getMouseState());
	
	soundSystem->update();
	
	if(input->checkMovement('W'))	//W
		camera->Walk(-100.0f * dt);

	if(input->checkMovement('A'))	//A
		camera->Strafe(-100.0f * dt);

	if(input->checkMovement('S'))	//S
		camera->Walk(100.0f * dt);

	if(input->checkMovement('D'))	//D
		camera->Strafe(100.0f * dt);

	if(input->checkKeyDown(0x20))	//Space
	{
		soundSystem->setPaused(pausedMusic);
		pausedMusic = !pausedMusic;
	}

	if(gameState == STATE_PLAYING)
	{
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