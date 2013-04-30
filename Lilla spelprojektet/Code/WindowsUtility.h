#ifndef WINDOWSUTILITY_H
#define WINDOWSUTILITY_H

#include <vector>
#include <WindowsX.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <ctime>

#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define SAFE_DELETE_ARRAY(x) if( x ) { delete[](x); (x) = NULL; }

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

struct MouseState
{
	int xPos;
	int yPos;
	WPARAM btnState;

	MouseState(int xPos, int yPos, WPARAM btnState)
	{
		this->xPos = xPos;
		this->yPos = yPos;
		this->btnState = btnState;
	}
	MouseState()
	{

	}
};

#endif