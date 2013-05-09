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

bool Engine::init(HINSTANCE hInstance, int cmdShow, int mapSize)
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

	pGeoManager->init(d3d->pDevice,mapSize);

	return true; // allt gick bra
}

void Engine::render(Matrix& vp)
{
	d3d->clearAndBindRenderTarget();

	int index = 0;
	PRIMITIVE_TOPOLOGIES topology = TOPOLOGY_UNDEFINED;
	
	static float rot = 0;
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	Shader* temp;

	temp = this->d3d->setPass(PASS_GEOMETRY);
	temp->SetMatrix("view", vp);
	//temp->SetMatrix("proj", proj);
	temp->Apply(0);

	for(int i = 0; i < this->pGeoManager->getNrOfEntities(); i++)
	{
		if(pGeoManager->getNrOfInstances(i) > 0)
		{
			pGeoManager->applyEntityBuffer(d3d->pDeviceContext, i, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			d3d->pDeviceContext->DrawInstanced(pGeoManager->getNrOfVertexPoints(i), pGeoManager->getNrOfInstances(i), 0, 0);
		}
	}


	world = world * vp;
	temp = this->d3d->setPass(PASS_PARTICLE);
	temp->SetMatrix("gWVP" , world);
	temp->Apply(0);
	pGeoManager->applyParticleBuffer(d3d->pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	d3d->pDeviceContext->Draw(pGeoManager->getNrOfParticles(), 0);

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

	//Draw hp bars
	temp = this->d3d->setPass(PASS_HPBARS);
	pGeoManager->applyHpBarBuffer(d3d->pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	temp->Apply(0);
	this->d3d->pDeviceContext->DrawInstanced(6, pGeoManager->getNrOfHPBars(), 0, 0);
	
	temp = this->d3d->setPass(PASS_FULLSCREENQUAD);
	pGeoManager->applyQuadBuffer(d3d->pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
		pGeoManager->updateEntityBuffer(d3d->pDeviceContext, renderData[i], i);
	}
}

void Engine::setRenderData(vector<vector<MESH_PNC>> renderData)
{
	pGeoManager->updateParticles(d3d->pDeviceContext, renderData);
}

void Engine::setHPBars(vector<HPBarInfo>& bars)
{
	//set hp bars
	pGeoManager->updateHPBars(d3d->pDeviceContext, bars);
}

MouseState* Engine::getMouseState()
{
	return this->win32->mState;
}

HWND Engine::getHWND()
{
	return win32->getHWND();
}