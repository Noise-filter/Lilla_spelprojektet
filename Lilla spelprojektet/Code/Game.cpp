#include "Game.h"


Game::Game(void)
{
	engine = new Engine();
	gameLogic = new GameLogic();
	input = new Input();
	camera = new Camera();
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

	if(!gameLogic->init(10,10))
		return false;

	input->init(engine->gethInstance(), engine->getHWND(), 1024, 768);

	camera->LookAt(D3DXVECTOR3(45,60,45), D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(0, 1, 0));
	camera->SetLens((float)D3DX_PI * 0.45f, 1024.0f / 768.0f, 0.1f, 1000.0f);

	//initiate other game resources such as level or whatever


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
	input->frame();

	int x, y;
	input->getDiffMouseLocation(x, y);

	camera->Yaw((float)x*0.5f);
	camera->Pitch((float)y*0.5f);

	if(input->isKeyPressed(DIK_W))	//W
		camera->Walk(30.0f * dt);

	if(input->isKeyPressed(DIK_A))	//A
		camera->Strafe(-30.0f * dt);

	if(input->isKeyPressed(DIK_S))	//S
		camera->Walk(-30.0f * dt);

	if(input->isKeyPressed(DIK_D))	//D
		camera->Strafe(30.0f * dt);

	camera->UpdateViewMatrix();

	if(gameLogic->update(dt))
		return 0; // error


	return 1;

}