#pragma once
#include "stdafx.h"
#include <WindowsX.h>
#include <Windows.h>
class WinHandler
{
	public:
		WinHandler(void);
		~WinHandler(void);
		
		HWND getHWND();
		HRESULT initWindow(HINSTANCE hInstance, int cmdShow);
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
		virtual void mouseOnMove(WPARAM btnState, int x, int y){};

	private:
		HINSTANCE   hInst;
		HWND        hWnd;



};


