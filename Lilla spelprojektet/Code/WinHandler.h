#pragma once
#include "WindowsUtility.h"
#include <WindowsX.h>
#include <Windows.h>
class WinHandler
{
	public:
		WinHandler(void);
		~WinHandler(void);
		
		MouseState* mState;
		HWND getHWND();
		HRESULT initWindow(HINSTANCE hInstance, int cmdShow);
		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
		virtual void mouseOnMove(WPARAM btnState, int x, int y){};

	private:
		HINSTANCE   hInst;
		HWND        hWnd;

		void setMouseState(LPARAM mousePos);
		void setButtonState(WPARAM btnState);

};


