#include "Engine.h"

Engine::Engine(void)
{
	d3d = new D3D11Handler();
	win32 = new WinHandler();
	pGeoManager = new GeometryManager();
}

Engine::~Engine(void)
{
	SAFE_DELETE(d3d);
	SAFE_DELETE(win32);
	SAFE_DELETE(pGeoManager);
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

	//pGeoManager->init(d3d->pDevice);

	return true; // allt gick bra
}

void Engine::render(std::vector<std::vector<RENDERDATA*>> data)
{
	d3d->clearViews();

	int index = 0;
	PRIMITIVE_TOPOLOGIES topology = TOPOLOGY_UNDEFINED;
	


	D3DXMATRIX world, view, proj, wvp;
	D3DXMatrixIdentity(&world);
	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0,0,-5), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0));
	D3DXMatrixPerspectiveFovLH(&proj, (float)D3DX_PI * 0.45f, 1024.0f / 768.0f, 1.0f, 100.0f);
	wvp = world * view * proj;
	
	Shader* temp;
	temp = this->d3d->setPass(PASS_GEOMETRY);
	temp->SetMatrix("world", world);
	temp->SetMatrix("viewProj", view * proj);
	temp->SetMatrix("worldViewProj", wvp);

	this->pGeoManager->geoDebugBuffer(d3d->pDeviceContext, d3d->pDevice);
	temp->Apply(0);
	d3d->pDeviceContext->Draw(6, 0);

	/*
	while(index < (int)data.size())
	{
		topology = changeTopology(data[index][0]->iEntityID);

		pGeoManager->updateBuffer(d3d->pDeviceContext, data[index], index);
		
		pGeoManager->applyBuffer(d3d->pDeviceContext, data[index][0], (D3D11_PRIMITIVE_TOPOLOGY)topology);

		index++;
	}

	index = 0;
	this->d3d->setPass(PASS_LIGHT);
	while(index < (int)data.size())
	{
		if(data[index][0]->iLightID > -1)
		{
			if(topology != PASS_LIGHT) topology = changeTopology(data[index][0]->iEntityID);		

			pGeoManager->applyBuffer(d3d->pDeviceContext, data[index][0], (D3D11_PRIMITIVE_TOPOLOGY)topology);
		}
		index++;
	}
	*/

	temp = this->d3d->setPass(PASS_FULLSCREENQUAD);
	this->pGeoManager->debugApplyBuffer(d3d->pDeviceContext, d3d->pDevice);
	temp->Apply(0);
	this->d3d->pDeviceContext->Draw(6, 0);
	
	if(FAILED(d3d->pSwapChain->Present( 0, 0 )))
	{
		return;
	}
}

PRIMITIVE_TOPOLOGIES Engine::changeTopology(int ID)
{
	if(ID != ENTITY_PARTICLESYSTEM) return TOPOLOGY_TRIANGLELIST;
	else return TOPOLOGY_POINTLIST;
}
