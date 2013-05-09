#include "D3D11Handler.h"

D3D11Handler::D3D11Handler()
{
	pRenderTargetView	= NULL;	
	pDepthStencil		= NULL;
	pDepthStencilView	= NULL;

	//Deferred targets
	pDeferredTargets	= NULL;
	pMultipleRTVs		= NULL;
	pMultipleSRVs		= NULL;
	pNullSRVs			= NULL;
	pDSVDeferred		= NULL; 
	iNrOfDeferred		= 3;

	this->vShaders.resize(NROFSHADERS);
}

D3D11Handler::~D3D11Handler()
{
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pDepthStencil);
	SAFE_RELEASE(pDepthStencilView);

	for(int i = 0; i < (int)this->vShaders.size(); i++)
	{
		SAFE_DELETE(this->vShaders.at(i));
	}

	//Deferred targets
	for(int i = 0; i < this->iNrOfDeferred+1; i++)
	{
		SAFE_RELEASE(this->pDeferredTargets[i]);
		SAFE_RELEASE(this->pMultipleSRVs[i]);
		SAFE_RELEASE(this->pNullSRVs[i]);
	}

	for(int i = 0; i < this->iNrOfDeferred; i++)
	{
		SAFE_RELEASE(this->pMultipleRTVs[i]);
	}

	SAFE_DELETE_ARRAY(pDeferredTargets);
	SAFE_DELETE_ARRAY(pMultipleRTVs);		
	SAFE_DELETE_ARRAY(pMultipleSRVs);		
	SAFE_DELETE_ARRAY(pNullSRVs);			
	SAFE_RELEASE(pDSVDeferred);
}

bool D3D11Handler::initDirect3D(HWND hWnd)
{
	if(!initSwapChainAndDevice(hWnd)) return false;

	if(!initRenderTargetView()) return false;

	if(!initDepthStencil()) return false;

	if(!initShaders()) return false;

	if(!initDeferred()) return false;

	initAndSetViewPort();

	return true;
}

Shader *D3D11Handler::setPass(PASS_STATE pass)
{
	switch(pass)
	{
		case PASS_GEOMETRY:
			this->pDeviceContext->OMSetRenderTargets(this->iNrOfDeferred, pMultipleRTVs, pDSVDeferred);
			return this->vShaders.at(PASS_GEOMETRY);
			break;

		case PASS_LIGHT:
			return this->vShaders.at(PASS_LIGHT);
			break;

		case PASS_PARTICLE:
			return this->vShaders.at(PASS_PARTICLE);

		case PASS_MENY:
			this->pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
			return this->vShaders.at(PASS_MENY);

		case PASS_FULLSCREENQUAD:
			this->pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
			this->vShaders.at(PASS_FULLSCREENQUAD)->SetResource("positionMap" , this->pMultipleSRVs[0]);
			this->vShaders.at(PASS_FULLSCREENQUAD)->SetResource("diffuseAlbedoMap" , this->pMultipleSRVs[1]);
			this->vShaders.at(PASS_FULLSCREENQUAD)->SetResource("normalMap" , this->pMultipleSRVs[2]);

			return this->vShaders.at(PASS_FULLSCREENQUAD);

		case PASS_HPBAR:
			this->pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
			return this->vShaders.at(PASS_HPBAR);

			break;
	}
	return NULL;
}

void D3D11Handler::clearAndBindRenderTarget()
{
	static float clearColour[4] = { 0.1f , 0.1f, 0.1f, 0.1f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, clearColour);
	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	pDeviceContext->PSSetShaderResources(0, this->iNrOfDeferred+1, this->pNullSRVs);
	pDeviceContext->ClearDepthStencilView(pDSVDeferred, 1, 1.0f, 0);

	for(int i = 0; i < this->iNrOfDeferred; i++) pDeviceContext->ClearRenderTargetView(pMultipleRTVs[i], clearColour);
}

ID3D11Device* D3D11Handler::returnDevice()
{
	return this->pDevice;
}

ID3D11DeviceContext* D3D11Handler::returnDeviceContext()
{
	return this->pDeviceContext;
}
/*
#########################################
		Private functions
#########################################
*/

bool D3D11Handler::initSwapChainAndDevice(HWND hWnd)
{
	bool deviceAndSwapCreated = false;
	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverType;

	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount							= 1;
	swapChainDesc.BufferDesc.Width						= SCREEN_WIDTH;
	swapChainDesc.BufferDesc.Height						= SCREEN_HEIGHT;
	swapChainDesc.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	swapChainDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow							= hWnd;
	swapChainDesc.SampleDesc.Count						= 1;
	swapChainDesc.SampleDesc.Quality					= 0;
	swapChainDesc.Windowed								= TRUE;

	D3D_FEATURE_LEVEL featureLevelsToTry[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	for(UINT i = 0; i < numDriverTypes && !deviceAndSwapCreated; i++)
	{
		driverType = driverTypes[i];

		if(SUCCEEDED(D3D11CreateDeviceAndSwapChain(
					 NULL,
					 driverType,
					 NULL,
					 createDeviceFlags,
					 featureLevelsToTry,
					 ARRAYSIZE(featureLevelsToTry),
					 D3D11_SDK_VERSION,
					 &swapChainDesc,
					 &pSwapChain,
					 &pDevice,
					 &initiatedFeatureLevel,
					 &pDeviceContext)))
		{
			//When created we break the loop
			deviceAndSwapCreated = true;
		}
	 
	}
	return deviceAndSwapCreated;
}

void D3D11Handler::initAndSetViewPort()
{
	viewPort.Width	= (float)SCREEN_WIDTH;
	viewPort.Height	= (float)SCREEN_HEIGHT;
	viewPort.MinDepth	= 0.0f;
	viewPort.MaxDepth	= 1.0f;
	viewPort.TopLeftX	= 0.0f;
	viewPort.TopLeftY	= 0.0f;

	pDeviceContext->RSSetViewports(1, &viewPort);
}

bool D3D11Handler::initDepthStencil()
{
	D3D11_TEXTURE2D_DESC depthDesc;
	depthDesc.Width					= SCREEN_WIDTH;
	depthDesc.Height				= SCREEN_HEIGHT;
	depthDesc.MipLevels				= 1;
	depthDesc.ArraySize				= 1;
	depthDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	depthDesc.SampleDesc.Count		= 1;
	depthDesc.SampleDesc.Quality	= 0;
	depthDesc.Usage					= D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags				= D3D11_BIND_DEPTH_STENCIL;
	depthDesc.MiscFlags				= 0;
	depthDesc.CPUAccessFlags		= 0;

	if(FAILED(pDevice->CreateTexture2D(&depthDesc, NULL, &pDepthStencil))) return false;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Format				= depthDesc.Format;
	dsvDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice	= 0;

	if(FAILED(pDevice->CreateDepthStencilView(pDepthStencil, &dsvDesc, &pDepthStencilView))) return false;

	return true;
}

bool D3D11Handler::initRenderTargetView()
{
	ID3D11Texture2D *backBuffer;

	if(FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer))) return false;

	if(FAILED(pDevice->CreateRenderTargetView(backBuffer, NULL, &pRenderTargetView))) return false;

	SAFE_RELEASE(backBuffer);

	//RenderTargetView initiated
	return true;
}

bool D3D11Handler::initShaders()
{
	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXTUREID", 0, DXGI_FORMAT_R32_UINT, 2, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		
	};

	Shader* temp = new Shader();
	this->vShaders.at(PASS_GEOMETRY) = temp;
	hr = this->vShaders.at(PASS_GEOMETRY)->Init(this->pDevice, this->pDeviceContext, "../Shaders/Geometry.fx", inputDesc, 8);
	if(FAILED(hr))
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC ParticleInput[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	temp = new Shader();
	this->vShaders.at(PASS_PARTICLE) = temp;
	hr = this->vShaders.at(PASS_PARTICLE)->Init(this->pDevice, this->pDeviceContext, "../Shaders/ParticleColor.fx", ParticleInput, 3);
	if(FAILED(hr))
	{
		return false;
	}
	

	D3D11_INPUT_ELEMENT_DESC tempInput[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	temp = new Shader();
	this->vShaders.at(PASS_MENY) = temp;
	hr = this->vShaders.at(PASS_MENY)->Init(this->pDevice, this->pDeviceContext, "../Shaders/FullScreenQuad.fx", tempInput, 1);
	if(FAILED(hr))
	{
		return false;
	}


	temp = new Shader();
	this->vShaders.at(PASS_FULLSCREENQUAD) = temp;
	hr = this->vShaders.at(PASS_FULLSCREENQUAD)->Init(this->pDevice, this->pDeviceContext, "../Shaders/FullScreenQuad.fx", tempInput, 1);
	if(FAILED(hr))
	{
		return false;
	}
/*
	temp = new Shader();
	this->vShaders.push_back(temp);
	hr = this->vShaders.at(PASS_HPBAR)->Init(this->pDevice, this->pDeviceContext, "../Shaders/hp bars testing.fx", tempInput, 1);
	if(FAILED(hr))
	{
		return false;
	}
*/
	return true;
}

bool D3D11Handler::initDeferred()
{
	D3D11_TEXTURE2D_DESC texDesc;	
	texDesc.Width				= SCREEN_WIDTH;
	texDesc.Height				= SCREEN_HEIGHT;
	texDesc.MipLevels			= 1;
	texDesc.ArraySize			= 1;
	texDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count	= 1;
	texDesc.SampleDesc.Quality	= 0;
	texDesc.Usage				= D3D11_USAGE_DEFAULT;
	texDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags		= 0;
	texDesc.MiscFlags			= 0;

	pDeferredTargets	= new ID3D11Texture2D *[this->iNrOfDeferred + 1];
	pMultipleRTVs		= new ID3D11RenderTargetView *[this->iNrOfDeferred];
	pMultipleSRVs		= new ID3D11ShaderResourceView *[this->iNrOfDeferred + 1];
	pNullSRVs			= new ID3D11ShaderResourceView *[this->iNrOfDeferred + 1];

	for(int i = 0; i < this->iNrOfDeferred; i++)
	{
		if(FAILED(pDevice->CreateTexture2D(&texDesc, NULL, &pDeferredTargets[i]))) return false;

		if(FAILED(pDevice->CreateRenderTargetView(pDeferredTargets[i], NULL, &pMultipleRTVs[i]))) return false;

		if(FAILED(pDevice->CreateShaderResourceView(pDeferredTargets[i], NULL, &pMultipleSRVs[i]))) return false;

		pNullSRVs[i] = NULL;
	}

	pNullSRVs[this->iNrOfDeferred] = NULL;

	if(!bindResources(texDesc)) return false;

	return true;
}

bool D3D11Handler::bindResources(D3D11_TEXTURE2D_DESC &texDesc)
{
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Format		= DXGI_FORMAT_R32_TYPELESS;
	if(FAILED(pDevice->CreateTexture2D(&texDesc, NULL, &pDeferredTargets[this->iNrOfDeferred]))) return false;

	D3D11_DEPTH_STENCIL_VIEW_DESC desc;
	desc.Format				= DXGI_FORMAT_D32_FLOAT;
	desc.Flags				= 0;
	desc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice	= 0;
	if(FAILED(pDevice->CreateDepthStencilView(pDeferredTargets[this->iNrOfDeferred], &desc, &pDSVDeferred))) return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.Format						= DXGI_FORMAT_R32_FLOAT;
	SRVDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels			= 1;
	SRVDesc.Texture2D.MostDetailedMip	= 0;
	if(FAILED(pDevice->CreateShaderResourceView(pDeferredTargets[this->iNrOfDeferred], &SRVDesc, &pMultipleSRVs[this->iNrOfDeferred]))) return false;

	return true;
}
