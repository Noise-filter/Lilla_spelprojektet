#pragma once
#include "D3D11Handler.h"
#include "WinHandler.h"
#include "GeometryManager.h"
#include <vector>
using namespace std;

class Engine
{
public:
	Engine(void);
	~Engine(void);

	void render(D3DXMATRIX& vp);
	bool init(HINSTANCE hInstance, int cmdShow);
	PRIMITIVE_TOPOLOGIES changeTopology(int ID);

	void setRenderData(vector<vector<RenderData*>> renderData);
	void setRenderData(vector<vector<MESH_PNC>> renderData);
	MouseState* getMouseState();
	HWND getHWND();

private:
	D3D11Handler* d3d;
	WinHandler* win32;
	GeometryManager *pGeoManager;

};

