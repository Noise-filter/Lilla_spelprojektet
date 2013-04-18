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


	return true; // allt gick bra
}

void Engine::render()
{
	static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	d3d->pDeviceContext->ClearRenderTargetView( d3d->pRTV, ClearColor );
	
    //clear depth info
	d3d->pDeviceContext->ClearDepthStencilView(d3d->pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	//set topology
	d3d->pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->d3d->setPass(GEOMETRY);

	this->d3d->setPass(LIGHT);

	this->d3d->setFSQDepth();
	this->d3d->setPass(FULLSCREENQUAD);

	this->d3d->resetDSS();


	if(FAILED(d3d->pSwapChain->Present( 0, 0 )))
	{
		return;
	}
}
