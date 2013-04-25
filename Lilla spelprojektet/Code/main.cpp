#include "stdafx.h"
#include "Game.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	MSG msg = {0};
	__int64 cntsPerSec = 0;
	__int64 prevTimeStamp = 0;
	__int64 currTimeStamp = 0;
	float dt = 0.0f;

	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	Game *game = new Game();

	if(!game->init(hInstance,nCmdShow))
	{
		return -1;
	}

	// Main message loop
	while(WM_QUIT != msg.message)
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			currTimeStamp = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			dt = (currTimeStamp - prevTimeStamp) * secsPerCnt;

			//main logic and draw calls
			game->update(dt);
			game->render(dt);
	

			prevTimeStamp = currTimeStamp;
		}
	}

	delete game;

	return 0;
}
