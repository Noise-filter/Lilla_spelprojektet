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

	// initierar directX
	if(!d3d->initDirect3D(win32->getHWND()))
	{
		return false;
	}

	pGeoManager->init(d3d->pDevice);

	return true; // allt gick bra
}

void Engine::render(float deltaTime, std::vector<std::vector<RENDERDATA*>> data)
{
	d3d->clearAndBindRenderTarget();

	int index = 0;
	PRIMITIVE_TOPOLOGIES topology = TOPOLOGY_UNDEFINED;
	
	static float rot = 0;
	rot += deltaTime;
	D3DXMATRIX world, world2, view, proj, wvp, wvp2;
	D3DXMatrixRotationY(&world, rot);
	D3DXMatrixTranslation(&world2, 3, sin(rot), 0);
	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0,0,-5), &D3DXVECTOR3(0,5,0), &D3DXVECTOR3(0,1,0));
	D3DXMatrixPerspectiveFovLH(&proj, (float)D3DX_PI * 0.45f, SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 100.0f);
	/*wvp = view * proj;
	wvp2 = world2 * view * proj;*/

	RENDERDATA temp1[2];
	temp1[0].iEntityID = (int)ENTITY_MAINBUILDING;
	temp1[0].iLightID = LIGHT_NONE;
	temp1[0].worldTex.mWorld = world;
	temp1[1].iEntityID = (int)ENTITY_MAINBUILDING;
	temp1[1].iLightID = LIGHT_NONE;
	temp1[1].worldTex.mWorld = world2;
	std::vector<RENDERDATA*> dtest;
	dtest.push_back(&temp1[0]);
	dtest.push_back(&temp1[1]);
	std::vector<std::vector<RENDERDATA*>> test;
	test.push_back(dtest);
	FLOAT meh;

	pGeoManager->updateBuffer(d3d->pDeviceContext, test[0], 0, view, proj);

	Shader* temp;
	temp = this->d3d->setPass(PASS_GEOMETRY);
	//D3DXMatrixInverse(&wvp, &meh, &view);
	//D3DXMatrixTranspose(&view, &wvp);
	//D3DXMatrixInverse(&wvp2, &meh, &proj);
	//D3DXMatrixTranspose(&proj, &wvp2);
	//temp->SetMatrix("view", wvp);
	//temp->SetMatrix("proj", wvp2);
	temp->SetMatrix("view", view);
	temp->SetMatrix("proj", proj);
	/*
	temp->SetRawData("view", view, sizeof(D3DXMATRIX));
	temp->SetRawData("proj", proj, sizeof(D3DXMATRIX));*/
	pGeoManager->applyBuffer(d3d->pDeviceContext, test[0], D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 0);
	temp->Apply(0);
	d3d->pDeviceContext->DrawInstanced(3, 2, 0, 0);

	//pGeoManager->applyBuffer(d3d->pDeviceContext, test[1][0], D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 0);
	
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
	pGeoManager->debugApplyBuffer(d3d->pDeviceContext, 1);

	
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
