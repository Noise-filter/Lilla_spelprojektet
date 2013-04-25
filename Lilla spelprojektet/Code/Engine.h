#pragma once
#include "D3D11Handler.h"
#include "WinHandler.h"
#include "GeometryManager.h"

class Engine
{
public:
	Engine(void);
	~Engine(void);

	void render(float deltaTime, std::vector<std::vector<RENDERDATA*>> data);
	bool init(HINSTANCE hInstance, int cmdShow);
	PRIMITIVE_TOPOLOGIES changeTopology(int ID);

private:
	D3D11Handler* d3d;
	WinHandler* win32;
	GeometryManager *pGeoManager;

};

