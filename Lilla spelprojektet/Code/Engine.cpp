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
	//D3DXMATRIX world, world2, world3, view, proj, wvp, wvp2;
	//D3DXMatrixRotationY(&world, rot);
	//D3DXMatrixTranslation(&world2, 3, sin(rot), 0);
	//D3DXMatrixTranslation(&world3, -3, -sin(rot), 0);
	//D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0,0,-10), &D3DXVECTOR3(0,0, 1), &D3DXVECTOR3(0,1,0));
	//D3DXMatrixPerspectiveFovLH(&proj, (float)D3DX_PI * 0.45f, SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 100.0f);

	//std::vector<std::vector<RenderData*>> temp3;
	//std::vector<RenderData*> temp1, temp2, tem3, tem4;
	//RenderData dtemp1[1], dtemp2[1], dtemp3[1], dtemp4[1];
	//dtemp1[0].iEntityID = (int)ENTITY_MAINBUILDING;
	//dtemp1[0].iLightID = LIGHT_NONE;
	//dtemp1[0].worldTex.iTextureID = 0;
	//dtemp1[0].worldTex.mWorld = world;
	//temp2.push_back(&dtemp1[0]);
	//temp3.push_back(temp2);

	//dtemp2[0].iEntityID = (int)ENTITY_SUPPLY;
	//dtemp2[0].iLightID = LIGHT_NONE;
	//dtemp2[0].worldTex.iTextureID = 0;
	//dtemp2[0].worldTex.mWorld = world2;
	//temp1.push_back(&dtemp2[0]);
	//temp3.push_back(temp1);

	//dtemp3[0].iEntityID = (int)ENTITY_TOWER;
	//dtemp3[0].iLightID = LIGHT_NONE;
	//dtemp3[0].worldTex.iTextureID = 0;
	//dtemp3[0].worldTex.mWorld = world3;
	//tem3.push_back(&dtemp3[0]);
	//temp3.push_back(tem3);

	//dtemp4[0].iEntityID = (int)ENTITY_NODE;
	//dtemp4[0].iLightID = LIGHT_NONE;
	//dtemp4[0].worldTex.iTextureID = 0;
	//dtemp4[0].worldTex.mWorld = world3;
	//tem4.push_back(&dtemp4[0]);
	//temp3.push_back(tem4);



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

	pGeoManager->applyQuadBuffer(d3d->pDeviceContext, pGeoManager->getNrOfBuffer()+1, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
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
	for(int i = 0; i < (int)renderData.size(); i++)
	{
		pGeoManager->updateParticles(d3d->pDeviceContext, renderData[i], renderData.size());
	}
}

//void Engine::setRenderData(vector<vector<VertexColor>> renderData)
//{
	/*D3D11_MAPPED_SUBRESOURCE mappedData;
	d3d->deviceContext->Map(particleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

	VertexColor* dataView = reinterpret_cast<VertexColor*>(mappedData.pData);

	particleNum = 0;

	for(int j = 0; j < (int)renderData.size(); j++)
	{
		for(int i = 0; i < (int)renderData.at(j).size(); i++)
		{
			dataView[i + particleNum] = renderData.at(j).at(i);
		}
		particleNum += renderData.at(j).size();
	}

	d3d->deviceContext->Unmap(particleBuffer, 0);*/
//}

MouseState* Engine::getMouseState()
{
	return this->win32->mState;
}

HWND Engine::getHWND()
{
	return win32->getHWND();
}
