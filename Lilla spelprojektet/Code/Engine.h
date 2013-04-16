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

	void render();
	bool init(HINSTANCE hInstance, int cmdShow);

	void setRenderData(vector<vector<RenderData*>> renderData);

	void updateBuffers();

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

};

