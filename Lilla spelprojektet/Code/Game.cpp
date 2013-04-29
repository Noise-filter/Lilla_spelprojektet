#include "Game.h"


Game::Game(void)
{
	engine = new Engine();
	gameLogic = new GameLogic();
	input = new Input();
	camera = new Camera();
	soundSystem = soundSystem->Getinstance();
	pSystem = pSystem->Getinstance();
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
	if(!engine->init(hInstance,cmdShow))
		return false;

	soundSystem->init();
	playlist = soundSystem->createPlaylist("playlist.m3u");
	//initiate other game resources such as level or whatever

	if(!gameLogic->init(20,10))
		return false;

	camera->LookAt(D3DXVECTOR3(45,60,45), D3DXVECTOR3(35, 0, 45), D3DXVECTOR3(-1, 0, 0));
	camera->SetLens((float)D3DX_PI * 0.45f, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);

	return true; // all initiates went well
}

void Game::render()
{
	//build engines renderContent with addRenderData then do render to execute those renders
	engine->setRenderData(gameLogic->getRenderData());

	engine->setRenderData(pSystem->getVertexData());

	engine->render(camera->ViewsProj());
}

int Game::update(float dt)
{
	static bool pausedMusic = true;

	input->updateMs(engine->getMouseState());
	
	//camera->Yaw((float)this->input->mouseRotateY());
	//camera->Pitch((float)this->input->mousePitch());
	
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


	camera->UpdateViewMatrix();

	if(!gameLogic->update(dt,input->getMs(), camera->View(), camera->Proj(), camera->GetPosition()))
		return 0; // error

	input->resetBtnState();

	pSystem->update();

	return 1;
}