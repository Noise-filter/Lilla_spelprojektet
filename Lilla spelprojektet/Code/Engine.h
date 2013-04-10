#pragma once
#include "D3D11Handler.h"
#include "WinHandler.h"

class Engine
{
public:
	Engine(void);
	~Engine(void);

	void render();
	bool init(HINSTANCE hInstance, int cmdShow);

private:
	D3D11Handler* d3d;
	WinHandler* win32;

};

