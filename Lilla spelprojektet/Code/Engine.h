#pragma once
#include "D3D11Handler.h"
#include "WinHandler.h"
#include <vector>

using namespace std;
class Engine
{
public:
	Engine(void);
	~Engine(void);

	void render(D3DXMATRIX& vp);
	bool init(HINSTANCE hInstance, int cmdShow);

	void setRenderData(vector<vector<RenderData*>> renderData);

	void updateBuffers();
	
	HWND getHWND();
	HINSTANCE gethInstance();

	MouseState* getMouseState();

private:
	struct Vertex
	{
		D3DXVECTOR3 pos;
	};

	struct InstancedData
	{
		D3DXMATRIX matrix;
	};

	D3D11Handler* d3d;
	WinHandler* win32;

	Shader* shader;
	ID3D11Buffer* vbs[2];
	ID3D11Buffer* indexBuffer;

	ID3D11Buffer* vbs2[2];
	ID3D11Buffer* indexBuffer2;

	ID3D11Buffer* vbs3[2];
	ID3D11Buffer* indexBuffer3;
};

