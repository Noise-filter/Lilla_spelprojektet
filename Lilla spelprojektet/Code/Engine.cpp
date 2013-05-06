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

void Engine::render(Matrix& vp)
{
	d3d->clearAndBindRenderTarget();

	int index = 0;
	PRIMITIVE_TOPOLOGIES topology = TOPOLOGY_UNDEFINED;
	
	static float rot = 0;
	//rot += deltaTime;
	D3DXMATRIX world, world2, world3, view, proj, wvp, wvp2;
	D3DXMatrixIdentity(&world);
	//D3DXMatrixRotationY(&world, rot);
	//D3DXMatrixTranslation(&world2, 3, sin(rot), 0);
	//D3DXMatrixTranslation(&world3, -3, -sin(rot), 0);
	//D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0,0,-10), &D3DXVECTOR3(0,0, 1), &D3DXVECTOR3(0,1,0));
	//D3DXMatrixPerspectiveFovLH(&proj, (float)D3DX_PI * 0.45f, SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 100.0f);


	Shader* temp;

	temp = this->d3d->setPass(PASS_GEOMETRY);
	temp->SetMatrix("view", vp);
	//temp->SetMatrix("proj", proj);
	temp->Apply(0);

	for(int i = 0; i < this->pGeoManager->getNrOfBuffer(); i++)
	{
		if(pGeoManager->getNrOfInstances(i) > 0)
		{
			pGeoManager->applyBuffer(d3d->pDeviceContext, i, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 0);
			d3d->pDeviceContext->DrawInstanced(pGeoManager->getNrOfVertexPoints(i), pGeoManager->getNrOfInstances(i), 0, 0);
		}
	}


	world = world * vp;
	temp = this->d3d->setPass(PASS_LIGHT);
	temp->SetMatrix("gWVP" , world);
	temp->Apply(0);
	pGeoManager->applyParticleBuffer(d3d->pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	d3d->pDeviceContext->Draw(pGeoManager->getNrOfParticles(), 0);
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
	pGeoManager->applyQuadBuffer(d3d->pDeviceContext, this->pGeoManager->getNrOfBuffer() , D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	temp->Apply(0);
	this->d3d->pDeviceContext->Draw(6, 0);

	if(FAILED(d3d->pSwapChain->Present( 0, 0 )))
	{
		return;
	}
}

void Engine::setRenderData(vector<vector<RenderData*>>& renderData)
{
	for(int i = 0; i < (int)renderData.size(); i++)
	{
		pGeoManager->updateBuffer(d3d->pDeviceContext, renderData[i], i , renderData[i].size());
	}
}

void Engine::setRenderData(vector<vector<MESH_PNC>> renderData)
{
	this->pGeoManager->setNrOfParticles(0);

	for(int i = 0; i < (int)renderData.size(); i++)
	{
		pGeoManager->updateParticles(d3d->pDeviceContext, renderData[i], renderData.size());
	}
}

MouseState* Engine::getMouseState()
{
	return this->win32->mState;
}

HWND Engine::getHWND()
{
	return win32->getHWND();
}