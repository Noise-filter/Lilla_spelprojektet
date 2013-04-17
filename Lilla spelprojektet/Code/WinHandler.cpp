#include "WinHandler.h"


namespace
{
	WinHandler* winHandler = NULL;
}

WinHandler::WinHandler(void)
{
	HINSTANCE				g_hInst					= NULL;  
	HWND					g_hWnd					= NULL;

	winHandler = this;

	mState = new MouseState();
	
}


WinHandler::~WinHandler(void)
{
	winHandler = NULL;
	SAFE_DELETE(mState);
}


HRESULT WinHandler::initWindow(HINSTANCE hInstance, int cmdShow)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = 0;
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = "ErikSamEngine";
	wcex.hIconSm        = 0;

	if( !RegisterClassEx(&wcex) )
		return E_FAIL;

	hInst = hInstance; 
	RECT rc = { 0, 0, screenWidth, screenHeight };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	if(!(hWnd = CreateWindow
		(
			"ErikSamEngine",
			"ErikSamEngine - D3D",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rc.right - rc.left,
			rc.bottom - rc.top,
			NULL,
			NULL,
			hInstance,
			NULL
		)))
	{
		return E_FAIL;
	}

	ShowWindow(hWnd, cmdShow );

	return S_OK;
}
void WinHandler::setMouseState(LPARAM mousePos)
{
	mState->xPos = GET_X_LPARAM(mousePos);
	mState->yPos = GET_Y_LPARAM(mousePos);
}
void WinHandler::setButtonState(WPARAM btnState)
{
	mState->btnState = btnState;
}

LRESULT CALLBACK WinHandler::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	

	switch (message) 
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
		}
		break;
	case WM_LBUTTONDOWN:
		winHandler->setButtonState(wParam);
			break;
	case WM_MOUSEHWHEEL:
		winHandler->setButtonState(wParam);
		break;

	case WM_MOUSEMOVE:
		winHandler->setMouseState(lParam);
			break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	

	return 0;
}

HWND WinHandler::getHWND()
{
	return this->hWnd;
}

HINSTANCE WinHandler::gethInst()
{
	return hInst;
}