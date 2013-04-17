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

	vbs2[0]->Release();
	vbs2[1]->Release();
	indexBuffer2->Release();
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

	//Skapa nodernas buffrar
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
	vbd2.ByteWidth = sizeof(InstancedData) * 10000;
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



	//Skapa strukturernas buffrar
	Vertex vertices2[4];
	vertices2[0].pos = D3DXVECTOR3(-1, 0, -1);
	vertices2[1].pos = D3DXVECTOR3(-1, 0, 1);
	vertices2[2].pos = D3DXVECTOR3(1, 0, 1);
	vertices2[3].pos = D3DXVECTOR3(1, 0, -1);

	int indices2[6];
	indices2[0] = 0;
	indices2[1] = 1;
	indices2[2] = 2;
	indices2[3] = 0;
	indices2[4] = 3;
	indices2[5] = 2;

	//Vertex buffer
	D3D11_BUFFER_DESC vbd3;
	vbd3.Usage = D3D11_USAGE_IMMUTABLE;
	vbd3.ByteWidth = sizeof(Vertex) * 4;
	vbd3.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd3.CPUAccessFlags = 0;
	vbd3.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA initDataVB2;
	initDataVB2.pSysMem = vertices2;
	d3d->device->CreateBuffer(&vbd3, &initDataVB2, &vbs2[0]);
	
	//instanced buffer
	D3D11_BUFFER_DESC vbd4;
	vbd4.Usage = D3D11_USAGE_DYNAMIC;
	vbd4.ByteWidth = sizeof(InstancedData) * 10000;
	vbd4.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd4.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd4.MiscFlags = 0;

	d3d->device->CreateBuffer(&vbd4, NULL, &vbs2[1]);

	//index buffer
	D3D11_BUFFER_DESC ibd2;
	ibd2.Usage = D3D11_USAGE_IMMUTABLE;
	ibd2.ByteWidth = sizeof(int) * 6;
	ibd2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd2.CPUAccessFlags = 0;
	ibd2.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initDataIB2;
	initDataIB2.pSysMem = indices2;
	d3d->device->CreateBuffer(&ibd2, &initDataIB2, &indexBuffer2);



	//Skapa fiendernas buffrar
	Vertex vertices3[4];
	vertices3[0].pos = D3DXVECTOR3(-1, 0, -1);
	vertices3[1].pos = D3DXVECTOR3(-1, 0, 1);
	vertices3[2].pos = D3DXVECTOR3(1, 0, 1);
	vertices3[3].pos = D3DXVECTOR3(1, 0, -1);

	int indices3[6];
	indices3[0] = 0;
	indices3[1] = 1;
	indices3[2] = 2;
	indices3[3] = 0;
	indices3[4] = 3;
	indices3[5] = 2;

	//Vertex buffer
	D3D11_BUFFER_DESC vbd5;
	vbd5.Usage = D3D11_USAGE_IMMUTABLE;
	vbd5.ByteWidth = sizeof(Vertex) * 4;
	vbd5.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd5.CPUAccessFlags = 0;
	vbd5.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA initDataVB3;
	initDataVB3.pSysMem = vertices3;
	d3d->device->CreateBuffer(&vbd5, &initDataVB3, &vbs3[0]);
	
	//instanced buffer
	D3D11_BUFFER_DESC vbd6;
	vbd6.Usage = D3D11_USAGE_DYNAMIC;
	vbd6.ByteWidth = sizeof(InstancedData) * 10000;
	vbd6.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd6.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd6.MiscFlags = 0;

	d3d->device->CreateBuffer(&vbd6, NULL, &vbs3[1]);

	//index buffer
	D3D11_BUFFER_DESC ibd3;
	ibd3.Usage = D3D11_USAGE_IMMUTABLE;
	ibd3.ByteWidth = sizeof(int) * 6;
	ibd3.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd3.CPUAccessFlags = 0;
	ibd3.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA initDataIB3;
	initDataIB3.pSysMem = indices3;
	d3d->device->CreateBuffer(&ibd2, &initDataIB3, &indexBuffer3);

	return true; // allt gick bra
}

void Engine::render(D3DXMATRIX& vp)
{
	static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	d3d->deviceContext->ClearRenderTargetView( d3d->renderTargetView, ClearColor );
	
    //clear depth info
	d3d->deviceContext->ClearDepthStencilView(d3d->depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	//set topology
	d3d->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->SetMatrix("gVP", vp);
	shader->Apply(0);

	UINT stride[2] = {sizeof(Vertex), sizeof(InstancedData)};
	UINT offset[2] = {0, 0};
	d3d->deviceContext->IASetVertexBuffers(0, 2, vbs, stride, offset);
	d3d->deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	d3d->deviceContext->DrawIndexedInstanced(3, 10000, 0, 0, 0);


	d3d->deviceContext->IASetVertexBuffers(0, 2, vbs2, stride, offset);
	d3d->deviceContext->IASetIndexBuffer(indexBuffer2, DXGI_FORMAT_R32_UINT, 0);

	d3d->deviceContext->DrawIndexedInstanced(6, 10000, 0, 0, 0);


	d3d->deviceContext->IASetVertexBuffers(0, 2, vbs3, stride, offset);
	d3d->deviceContext->IASetIndexBuffer(indexBuffer3, DXGI_FORMAT_R32_UINT, 0);

	d3d->deviceContext->DrawIndexedInstanced(6, 10000, 0, 0, 0);

	if(FAILED(d3d->swapChain->Present( 0, 0 )))
	{
		return;
	}
}

void Engine::setRenderData(vector<vector<RenderData*>> renderData)
{
	D3D11_MAPPED_SUBRESOURCE mappedData, mappedData2, mappedData3;
	d3d->deviceContext->Map(vbs[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	d3d->deviceContext->Map(vbs2[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData2);
	d3d->deviceContext->Map(vbs3[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData3);

	InstancedData* dataView = reinterpret_cast<InstancedData*>(mappedData.pData);
	InstancedData* dataView2 = reinterpret_cast<InstancedData*>(mappedData2.pData);
	InstancedData* dataView3 = reinterpret_cast<InstancedData*>(mappedData3.pData);

	int a = 0, b = 0, c = 0;
	for(int j = 0; j < (int)renderData.size(); j++)
	{
		for(int i = 0; i < (int)renderData.at(j).size(); i++)
		{
			if(renderData.at(j).at(i)->meshID == 0)
				dataView[a++].matrix = renderData[j][i]->worldMat;
			else if(renderData.at(j).at(i)->meshID == 1)
				dataView2[b++].matrix = renderData[j][i]->worldMat;
			else if(renderData.at(j).at(i)->meshID == 2)
				dataView3[c++].matrix = renderData[j][i]->worldMat;
		}
	}

	d3d->deviceContext->Unmap(vbs[1], 0);
	d3d->deviceContext->Unmap(vbs2[1], 0);
	d3d->deviceContext->Unmap(vbs3[1], 0);
}

MouseState* Engine::getMouseState()
{
	return this->win32->mState;
}
HWND Engine::getHWND()
{
	return win32->getHWND();
}

HINSTANCE Engine::gethInstance()
{
	return win32->gethInst();
}