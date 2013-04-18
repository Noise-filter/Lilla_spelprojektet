#include "Game.h"


Game::Game(void)
{
	engine = new Engine();
	gameLogic = new GameLogic();
	input = new Input();
	camera = new Camera();
	soundSystem = soundSystem->Getinstance();
}


Game::~Game(void)
{
	SAFE_DELETE(engine);
	SAFE_DELETE(gameLogic);
	SAFE_DELETE(camera);
	SAFE_DELETE(input);
	soundSystem->shutdown();
	delete playlist;
}

bool Game::init(HINSTANCE hInstance, int cmdShow)
{
	if(!engine->init(hInstance,cmdShow))
		return false;

	//initiate other game resources such as level or whatever

	if(!gameLogic->init(10,10))
		return false;

	camera->LookAt(D3DXVECTOR3(45,60,45), D3DXVECTOR3(45, 0, 45), D3DXVECTOR3(-1, 0, 0));
	camera->SetLens((float)D3DX_PI * 0.45f, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);

	//initiate other game resources such as level or whatever

	soundSystem->init();
	playlist = soundSystem->createPlaylist("playlist.m3u");

	return true; // all initiates went well
}


void Game::render()
{
	//build engines renderContent with addRenderData then do render to execute those renders
	engine->setRenderData(gameLogic->getRenderData());

	engine->render(camera->ViewsProj());
}

int Game::update(float dt)
{
	input->updateMs(engine->getMouseState());
	
	camera->Yaw((float)this->input->mouseRotateY());
	camera->Pitch((float)this->input->mousePitch());
	
	soundSystem->update();
	
	if(input->checkKeyDown(0x57))	//W
		camera->Walk(3000.0f * dt);

	if(input->checkKeyDown(0x41))	//A
		camera->Strafe(-30.0f * dt);

	if(input->checkKeyDown(0x53))	//S
		camera->Walk(-30.0f * dt);

	if(input->checkKeyDown(0x44))	//D
		camera->Strafe(30.0f * dt);

	camera->UpdateViewMatrix();

	if(!gameLogic->update(dt,input->getMs(), camera->View(), camera->Proj(), camera->GetPosition()))
		return 0; // error

	input->resetBtnState();
	

	return 1;

}