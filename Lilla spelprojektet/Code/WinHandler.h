#pragma once
#include "stdafx.h"
#include <WindowsX.h>

class WinHandler
{
	public:
		WinHandler(void);
		~WinHandler(void);
		
		HWND getHWND();
		HRESULT initWindow(HINSTANCE hInstance, int cmdShow);
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	protected:
		HINSTANCE   hInst;
		HWND        hWnd;



};


