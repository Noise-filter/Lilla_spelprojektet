#include "Engine.h"


Engine::Engine(void)
{
	d3d = new D3D11Handler();
	win32 = new WinHandler();
}


Engine::~Engine(void)
{
	SAFE_DELETE(d3d);
	SAFE_DELETE(win32);

	delete shader;
	vbs[0]->Release();
	vbs[1]->Release();
	indexBuffer->Release();
}

bool Engine::init(HINSTANCE hInstance, int cmdShow)
{
	HRESULT hr = (win32->initWindow(hInstance, cmdShow)); // initierar win32
	if(FAILED(hr))
	{
		return false;
	}

	hr = d3d->InitDirect3D(win32->getHWND()); // initierar directX
	if(FAILED(hr))
	{
		return false;
	}

	//shader
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
	
	shader = new Shader();
	shader->Init(d3d->device, d3d->deviceContext, "../Shaders/instanced.fx", inputDesc, 5);

	//Skapa buffrar
	Vertex vertices[3];
	vertices[0].pos = D3DXVECTOR3(-1, 0, -1);
	vertices[1].pos = D3DXVECTOR3(0, 0, 1);
	vertices[2].pos = D3DXVECTOR3(1, 0, -1);

	int indices[3];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	//Vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 3;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initDataVB;
	initDataVB.pSysMem = vertices;
	d3d->device->CreateBuffer(&vbd, &initDataVB, &vbs[0]);
	
	//instanced buffer
	D3D11_BUFFER_DESC vbd2;
	vbd2.Usage = D3D11_USAGE_DYNAMIC;
	vbd2.ByteWidth = sizeof(InstancedData) * 100;
	vbd2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd2.MiscFlags = 0;

	d3d->device->CreateBuffer(&vbd2, NULL, &vbs[1]);

	//index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * 3;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initDataIB;
	initDataIB.pSysMem = indices;
	d3d->device->CreateBuffer(&ibd, &initDataIB, &indexBuffer);

	return true; // allt gick bra
}

void Engine::render()
{
	D3DXMATRIX v, p;
	static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	d3d->deviceContext->ClearRenderTargetView( d3d->renderTargetView, ClearColor );
	
    //clear depth info
	d3d->deviceContext->ClearDepthStencilView(d3d->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	//set topology
	d3d->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DXMatrixLookAtLH(&v,&D3DXVECTOR3(45,60,45),&D3DXVECTOR3(45,0,45),&D3DXVECTOR3(0,0,1));
	D3DXMatrixPerspectiveFovLH(&p, (float)D3DX_PI * 0.45f, 1024.0f / 768.0f, 1.0f, 1000.0f);

	shader->SetMatrix("gVP", v*p);
	shader->Apply(0);

	UINT stride[2] = {sizeof(Vertex), sizeof(InstancedData)};
	UINT offset[2] = {0, 0};
	d3d->deviceContext->IASetVertexBuffers(0, 2, vbs, stride, offset);
	d3d->deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	d3d->deviceContext->DrawIndexedInstanced(3, 100, 0, 0, 0);


	if(FAILED(d3d->swapChain->Present( 0, 0 )))
	{
		return;
	}
}

void Engine::setRenderData(vector<vector<RenderData*>> renderData)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	d3d->deviceContext->Map(vbs[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

	InstancedData* dataView = reinterpret_cast<InstancedData*>(mappedData.pData);

	for(int i = 0; i < 100; i++)
		dataView[i].matrix = renderData[0][i]->worldMat;

	d3d->deviceContext->Unmap(vbs[1], 0);
}