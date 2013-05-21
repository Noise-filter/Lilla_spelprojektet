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

	pGeoManager->init(d3d->pDevice, d3d->pDeviceContext);
	//pGeoManager->initMeshes(d3d->pDevice, d3d->pDeviceContext, mapSize);

	HRESULT hResult = FW1CreateFactory(FW1_VERSION, &pFW1Factory);
	hResult = pFW1Factory->CreateFontWrapper(d3d->pDevice, L"Arial", &pFontWrapper);


	return true; // allt gick bra
}

void Engine::start(int mapSize)
{

	pGeoManager->initMeshes(d3d->pDevice, d3d->pDeviceContext, mapSize);

}

void Engine::render(Matrix& view, Matrix& proj, Text* text, int nrOfText,  Vec3 cameraPos)
{
	d3d->clearAndBindRenderTarget();

	int index = 0;
	PRIMITIVE_TOPOLOGIES topology = TOPOLOGY_UNDEFINED;
	
	static float rot = 0;
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	Shader* temp;

	temp = this->d3d->setPass(PASS_GEOMETRY);
	temp->SetMatrix("view", view);
	temp->SetMatrix("proj", proj);
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




	//till för ljuset

	ID3D11BlendState* blendState = NULL;
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	blendDesc.RenderTarget->BlendEnable = TRUE;
	blendDesc.RenderTarget->SrcBlend = D3D11_BLEND_SRC_COLOR;
	blendDesc.RenderTarget->DestBlend = D3D11_BLEND_DEST_COLOR;
	blendDesc.RenderTarget->BlendOp = D3D11_BLEND_OP_MAX;
	blendDesc.RenderTarget->SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget->DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget->BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = S_OK;

	hr = this->d3d->pDevice->CreateBlendState(&blendDesc, &blendState);
	if(FAILED(hr))
		return;
	this->d3d->pDeviceContext->OMSetBlendState(blendState, 0, 0xffffffff);

	//ljus
	Matrix ViewProj = view * proj;
	Matrix invertViewProj;
	float determinant = D3DXMatrixDeterminant(&invertViewProj);
	D3DXMatrixInverse(&invertViewProj, NULL , &ViewProj);

	temp = this->d3d->setPass(PASS_LIGHT);
	temp->SetMatrix("view", view);
	temp->SetMatrix("projection", proj);
	temp->SetMatrix("invertViewProjection", invertViewProj);
	temp->SetFloat4("cameraPos" , Vec4(cameraPos , 0));

	temp->Apply(0);
	this->pGeoManager->applyLightBuffer(d3d->pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->d3d->pDeviceContext->DrawInstanced(pGeoManager->getNrOfLightVertices(LIGHT_POINT), pGeoManager->getNrOfLightInstances(LIGHT_POINT) , 0 ,0);

	this->d3d->pDeviceContext->OMSetBlendState(NULL, NULL, 0xffffffff);

	world = world * view * proj;
	temp = this->d3d->setPass(PASS_PARTICLE);
	temp->SetMatrix("gWVP" , world);
	temp->Apply(0);
	pGeoManager->applyParticleBuffer(d3d->pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	d3d->pDeviceContext->Draw(pGeoManager->getNrOfParticles(), 0);

	//Rita ut gui
	temp = this->d3d->setPass(PASS_HPBARS);
	pGeoManager->applyGUIBuffer(d3d->pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	temp->Apply(0);
	this->d3d->pDeviceContext->DrawInstanced(6, pGeoManager->getNrOfGUIObjects(), 0, 0);

	//Draw hp bars
	temp = this->d3d->setPass(PASS_HPBARS);
	pGeoManager->applyHpBarBuffer(d3d->pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	temp->Apply(0);
	this->d3d->pDeviceContext->DrawInstanced(6, pGeoManager->getNrOfHPBars(), 0, 0);
	
	blurTexture(temp);

	view = view * proj;
	renderDebug(view);
	
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

void Engine::renderGui(Text* text, int nrOfText)
{
	d3d->clearAndBindRenderTarget();
	Shader* temp;
	temp = this->d3d->setPass(PASS_HPBARS);
	pGeoManager->applyGUIBuffer(d3d->pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	temp->Apply(0);
	this->d3d->pDeviceContext->DrawInstanced(6, pGeoManager->getNrOfGUIObjects(), 0, 0);

	if(text != NULL)
	{
		renderText(text, nrOfText);
	}
	
	if(FAILED(d3d->pSwapChain->Present( 0, 0 )))
	{
		return;
	}
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
	pGeoManager->applyQuadBuffer(d3d->pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
	D3DXMatrixTranslation(&translation, 0.0f, -0.7f, 0);
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