#pragma once
#include "stdafx.h"
#include <WindowsX.h>
#include <iostream>
using namespace std;
class WinHandler
{
	public:
		WinHandler(void);
		~WinHandler(void);
		MouseState* mState;

		HWND getHWND();
		HINSTANCE gethInst();
		HRESULT initWindow(HINSTANCE hInstance, int cmdShow);
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	protected:
		HINSTANCE   hInst;
		HWND        hWnd;

		void setMouseState(LPARAM mousePos,WPARAM btnState);
		void setButtonState(WPARAM btnState);
		
		


};


