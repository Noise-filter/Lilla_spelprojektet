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
	pFontWrapper->Release();
	pFW1Factory->Release();
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

	pGeoManager->init(d3d->pDevice, d3d->pDeviceContext, mapSize);


	HRESULT hResult = FW1CreateFactory(FW1_VERSION, &pFW1Factory);
	hResult = pFW1Factory->CreateFontWrapper(d3d->pDevice, L"Arial", &pFontWrapper);


	return true; // allt gick bra
}

void Engine::render(Matrix& vp, Text* text, int nrOfText)
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
	ID3D11ShaderResourceView *nulls;
	nulls = NULL;
	for(int i = 0; i < this->pGeoManager->getNrOfEntities(); i++)
	{
		if(pGeoManager->getNrOfInstances(i) > 0)
		{
			temp->SetResource("textures", pGeoManager->getTextures(i));
			temp->SetResource("glowMaps", pGeoManager->getGlowMaps(i));
			pGeoManager->applyEntityBuffer(d3d->pDeviceContext, i, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			temp->Apply(0);
			d3d->pDeviceContext->DrawInstanced(pGeoManager->getNrOfVertexPoints(i), pGeoManager->getNrOfInstances(i), 0, 0);
		}
	}
	
	//skicka in camerapos , halfpix , dela upp vp , skicka in invertVP
	temp = this->d3d->setPass(PASS_LIGHT);
	

	blurTexture(temp);

	world = world * vp;
	temp = this->d3d->setPass(PASS_PARTICLE);
	temp->SetMatrix("gWVP" , world);
	temp->Apply(0);
	pGeoManager->applyParticleBuffer(d3d->pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	d3d->pDeviceContext->Draw(pGeoManager->getNrOfParticles(), 0);

	


	//Draw hp bars
	temp = this->d3d->setPass(PASS_HPBARS);
	pGeoManager->applyHpBarBuffer(d3d->pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	temp->Apply(0);
	this->d3d->pDeviceContext->DrawInstanced(6, pGeoManager->getNrOfHPBars(), 0, 0);

	//Rita ut gui
	pGeoManager->applyGUIBuffer(d3d->pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	temp->Apply(0);
	this->d3d->pDeviceContext->DrawInstanced(6, pGeoManager->getNrOfGUIObjects(), 0, 0);

	
	renderDebug(vp);
	
	temp = this->d3d->setPass(PASS_FULLSCREENQUAD);
	pGeoManager->applyQuadBuffer(d3d->pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	temp->Apply(0);
	this->d3d->pDeviceContext->Draw(6, 0);

	if(text != NULL)
	{
		renderText(text, nrOfText);
	}
	
	if(FAILED(d3d->pSwapChain->Present( 0, 0 )))
	{
		return;
	}
}

void Engine::renderGui(int state, Text* text)
{
	
}

void Engine::renderText(Text* text, int nrOfText)
{
	for(int i = 0; i < nrOfText; i++)
	{
		pFontWrapper->DrawString(
			d3d->pDeviceContext,
			text[i].text,// String
			text[i].textSize,// Font size
			text[i].pos.x,// X position
			text[i].pos.y,// Y position
			text[i].textColor,// Text color, 0xAaBbGgRr
			FW1_VCENTER | FW1_CENTER | FW1_RESTORESTATE// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
		);
	}
}

void Engine::setRenderData(vector<vector<RenderData*>>& renderData)
{
	pGeoManager->updateEntityBuffer(d3d->pDeviceContext, renderData);
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

void Engine::setGUI(GUI_Panel* data, int nrOfPanels)
{
	pGeoManager->updateGUI(d3d->pDeviceContext, data, nrOfPanels);
}

MouseState* Engine::getMouseState()
{
	return this->win32->mState;
}

HWND Engine::getHWND()
{
	return win32->getHWND();
}


void Engine::blurTexture(Shader *temp)
{
	temp = d3d->setPass(PASS_BLURH);
	pGeoManager->applyGUIBuffer(d3d->pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	temp->Apply(0);
	this->d3d->pDeviceContext->Draw(6, 0);

	temp = d3d->setPass(PASS_BLURV);
	temp->Apply(1);
	this->d3d->pDeviceContext->Draw(6, 0);
}

void Engine::renderDebug(Matrix &vp)
{
	Matrix world, scaling, translation;
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&scaling);
	D3DXMatrixIdentity(&translation);

	D3DXMatrixScaling(&scaling, 0.3f , 0.3f , 0.3f);
	D3DXMatrixTranslation(&translation, 0.7f , 0.7f , 0);
	world = scaling * translation;

	this->pGeoManager->applyQuadBuffer(d3d->pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Shader *temp = this->d3d->setPass(PASS_DEBUG);

	temp->SetResource("debugMap" , this->d3d->debugGetSRV(5));
	temp->SetMatrix("world", world);
	
	temp->Apply(1);
	this->d3d->pDeviceContext->Draw(6, 0);




	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&scaling);
	D3DXMatrixIdentity(&translation);

	D3DXMatrixScaling(&scaling, 0.3f , 0.3f , 0.3f);
	D3DXMatrixTranslation(&translation, 0.4f, 0.0f, 0);
	world = scaling * translation;

	temp->SetResource("debugMap" , this->d3d->debugGetSRV(0));
	temp->SetMatrix("world", world);

	temp->Apply(0);
	this->d3d->pDeviceContext->Draw(6, 0);











	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&scaling);
	D3DXMatrixIdentity(&translation);

	D3DXMatrixScaling(&scaling, 0.3f , 0.3f , 0.3f);
	D3DXMatrixTranslation(&translation, 0.7f, 0.0f, 0);
	world = scaling * translation;

	temp->SetResource("debugMap" , this->d3d->debugGetSRV(1));
	temp->SetMatrix("world", world);

	temp->Apply(0);
	this->d3d->pDeviceContext->Draw(6, 0);

	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&scaling);
	D3DXMatrixIdentity(&translation);

	D3DXMatrixTranslation(&translation, 0.7f, -0.7f, 0);
	D3DXMatrixScaling(&scaling, 0.3f , 0.3f , 0.3f);
	world = scaling * translation;

	ID3D11ShaderResourceView* srv = this->d3d->debugGetSRV(2);
	temp->SetResource("debugMap" , srv);
	temp->SetMatrix("world", world);

	temp->Apply(0);
	this->d3d->pDeviceContext->Draw(6, 0);
}