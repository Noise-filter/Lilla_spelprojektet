#include "Game.h"
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <vld.h>


void SetStdOutToNewConsole()
{
    // allocate a console for this app
    AllocConsole();

    // redirect unbuffered STDOUT to the console
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    int fileDescriptor = _open_osfhandle((intptr_t)consoleHandle, _O_TEXT);
    FILE *fp = _fdopen( fileDescriptor, "w" );
    *stdout = *fp;
    setvbuf( stdout, NULL, _IONBF, 0 );
	
    // give the console window a nicer title
	char str[256];
	sprintf_s(str, "Debug Output");

    SetConsoleTitle(str);

    // give the console window a bigger buffer size
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if ( GetConsoleScreenBufferInfo(consoleHandle, &csbi) )
    {
        COORD bufferSize;
        bufferSize.X = csbi.dwSize.X;
        bufferSize.Y = 50;
        SetConsoleScreenBufferSize(consoleHandle, bufferSize);
    }
}

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	srand((int)time(NULL));
	MSG msg = {0};
	__int64 cntsPerSec = 0;
	__int64 prevTimeStamp = 0;
	__int64 currTimeStamp = 0;
	float dt = 0.0f;

	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);
	//SetStdOutToNewConsole();
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

			if(dt < 0.5)
			{
				//main logic and draw calls
				if(game->update(dt) == 0)
					break;
				game->render();
			}
			prevTimeStamp = currTimeStamp;
		}
	}

	delete game;

	return 0;
}
