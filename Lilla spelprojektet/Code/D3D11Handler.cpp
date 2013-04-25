#include "D3D11Handler.h"

//D3D11Handler::D3D11Handler()
//{
//	this->pSwapChain        = NULL;
//	this->pDevice           = NULL;
//	this->pDeviceContext    = NULL;
//
//	this->mrtRT             = NULL;
//	this->mrtRTV            = NULL;
//	this->mrtDS             = NULL;
//	this->mrtDSV            = NULL;
//	this->iNrRTS            = 0;
//
//	this->bbDS              = NULL;
//	this->bbDSV             = NULL;
//	this->bbRTV             = NULL;
//
//	this->pVP               = NULL;
//}
//
//D3D11Handler::~D3D11Handler()
//{	
//	SAFE_RELEASE(this->pSwapChain);
//	SAFE_RELEASE(this->pDevice);
//	SAFE_RELEASE(this->pDeviceContext);
//
//	SAFE_RELEASE(this->mrtRT);
//	SAFE_RELEASE(this->mrtRTV);
//	SAFE_RELEASE(this->mrtDS);
//	SAFE_RELEASE(this->mrtDSV);
//	SAFE_RELEASE(this->mrtSRV);
//	
//	SAFE_RELEASE(this->bbDS);
//	SAFE_RELEASE(this->bbDSV);
//	SAFE_RELEASE(this->bbRTV);
//	
//
//	SAFE_DELETE(this->pVP);
//
//
//	for(int i = 0; i < this->vShaders.size(); i++)
//	{
//		delete this->vShaders.at(i);
//	}
//}
//
//
//HRESULT D3D11Handler::InitDirect3D(HWND hWnd)
//{
//	HRESULT hr = S_OK;
//
//	this->iNrRTS = 3;
//
//	hr = initSwapChainAndDevice(hWnd);
//	if(FAILED(hr))
//	{
//		return hr;
//	}
//
//	hr = initShaders();
//	if(FAILED(hr))
//	{
//		return hr;
//	}
//
//	initVP();
//
//	hr = initDeferredMRT();
//	if(FAILED(hr))
//	{
//		return hr;
//	}
//
//	hr = initBackBufferRT();
//	if(FAILED(hr))
//	{
//		return hr;
//	}
//
//
//	return S_OK;
//}
//
//
//HRESULT D3D11Handler::initSwapChainAndDevice(HWND hWnd)
//{
//	HRESULT hr = S_OK;
//
//	UINT createDeviceFlags = 0;
//
//#ifdef _DEBUG
//	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif
//	
//	D3D_DRIVER_TYPE driverType;
//	
//	D3D_DRIVER_TYPE driverTypes[] =
//	{
//		D3D_DRIVER_TYPE_HARDWARE,
//		D3D_DRIVER_TYPE_REFERENCE
//	};
//
//	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);
//	
//	DXGI_SWAP_CHAIN_DESC sd;
//	ZeroMemory(&sd, sizeof(sd));
//	sd.BufferCount = 1;
//	sd.BufferDesc.Width = SCREEN_WIDTH;
//	sd.BufferDesc.Height = SCREEN_HEIGHT;
//	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	sd.BufferDesc.RefreshRate.Numerator = 60;
//	sd.BufferDesc.RefreshRate.Denominator = 1;
//	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	sd.OutputWindow = hWnd;
//	sd.SampleDesc.Count = 1;
//	sd.SampleDesc.Quality = 0;
//	sd.Windowed = TRUE;
//
//	D3D_FEATURE_LEVEL featureLevelsToTry[] = {
//		D3D_FEATURE_LEVEL_11_0,
//		D3D_FEATURE_LEVEL_10_1,
//		D3D_FEATURE_LEVEL_10_0
//	};
//
//	D3D_FEATURE_LEVEL initiatedFeatureLevel;
//
//	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
//	{
//		driverType = driverTypes[driverTypeIndex];
//		hr = D3D11CreateDeviceAndSwapChain(
//			NULL,
//			driverType,
//			NULL,
//			createDeviceFlags,
//			featureLevelsToTry,
//			ARRAYSIZE(featureLevelsToTry),
//			D3D11_SDK_VERSION,
//			&sd,
//			&this->pSwapChain,
//			&this->pDevice,
//			&initiatedFeatureLevel,
//			&this->pDeviceContext);
//
//		if( SUCCEEDED( hr ) )
//		{
//			char title[256];
//			sprintf_s(
//				title,
//				sizeof(title),
//				"BTH - Direct3D 11.0 Template | Direct3D 11.0 device initiated with Direct3D %s feature level",
//				FeatureLevelToString(initiatedFeatureLevel)
//			);
//			SetWindowText(hWnd, title);
//
//			break;
//		}
//	}
//	if( FAILED(hr) )
//		return hr;
//
//	return hr;
//}
//
//HRESULT D3D11Handler::initDepthStencil()
//{
//	HRESULT hr = S_OK;
//
//	//Depth Stencil
//	D3D11_TEXTURE2D_DESC ds;
//	ds.Width              = (UINT)SCREEN_WIDTH;
//	ds.Height             = (UINT)SCREEN_HEIGHT;
//	ds.MipLevels          = 1;
//	ds.ArraySize          = 1;//this->iNrRTS;
//	ds.SampleDesc.Count   = 1;
//	ds.SampleDesc.Quality = 0;
//	ds.Format             = DXGI_FORMAT_D32_FLOAT;
//	ds.Usage              = D3D11_USAGE_DEFAULT;
//	ds.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
//	ds.CPUAccessFlags     = 0;
//	ds.MiscFlags          = 0;
//
//	hr = this->pDevice->CreateTexture2D(&ds , NULL, &this->mrtDS);
//	if(FAILED(hr))
//	{
//		return hr;
//	}
//
//	//Depth Stencil View
//	D3D11_DEPTH_STENCIL_VIEW_DESC  dsv;
//	dsv.Format             = DXGI_FORMAT_D32_FLOAT;
//	dsv.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
//	dsv.Texture2D.MipSlice = 0;
//	dsv.Flags              = 0;
//
//	hr = this->pDevice->CreateDepthStencilView(this->mrtDS, &dsv, &this->mrtDSV);
//	if(FAILED(hr))
//	{
//		return hr;
//	}
//
//	return hr;
//}
//
//HRESULT D3D11Handler::initRenderTargetView()
//{
//	HRESULT hr = S_OK;
//
//	// Create a render target view
//	ID3D11Texture2D* pBackBuffer;
//	hr = this->pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&pBackBuffer );
//	if( FAILED(hr) ) return hr;
//
//	hr = this->pDevice->CreateRenderTargetView( pBackBuffer, NULL, &this->bbRTV );
//	pBackBuffer->Release();
//	if( FAILED(hr) ) return hr;
//
//	return hr;
//}
//void D3D11Handler::initVP()
//{
//	this->pVP = new D3D11_VIEWPORT;
//	this->pVP->Width = SCREEN_WIDTH;
//	this->pVP->Height = SCREEN_HEIGHT;
//	this->pVP->MinDepth = 0.0f;
//	this->pVP->MaxDepth = 1.0f;
//	this->pVP->TopLeftX = 0;
//	this->pVP->TopLeftY = 0;
//
//	this->pDeviceContext->RSSetViewports( 1, this->pVP );
//}
//HRESULT D3D11Handler::initShaders()
//{
//	HRESULT hr;
//
//	D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
//	{
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TEXTCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//
//		/*{ "TEXTUREID", 0, DXGI_FORMAT_R32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
//
//		{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 36, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
//		{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 52, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
//		{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 68, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
//		{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 84, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
//		*/
//	};
//
//	Shader* temp = new Shader();
//	this->vShaders.push_back(temp);
//	hr = this->vShaders.at(PASS_GEOMETRY)->Init(this->pDevice, this->pDeviceContext, "../Shaders/Geometry.fx", inputDesc, 3);
//	if(FAILED(hr))
//	{
//		return hr;
//	}
//
//	temp = new Shader();
//	this->vShaders.push_back(temp);
//	//hr = this->vShaders.at(PASS_LIGHT)->Init(this->pDevice, this->pDeviceContext, "../Shaders/Lightning.fx", inputDesc, 3);
//	if(FAILED(hr))
//	{
//		return hr;
//	}
//	
//
//	D3D11_INPUT_ELEMENT_DESC tempInput[] = 
//	{
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	};
//
//
//	temp = new Shader();
//	this->vShaders.push_back(temp);
//	hr = this->vShaders.at(PASS_FULLSCREENQUAD)->Init(this->pDevice, this->pDeviceContext, "../Shaders/FullScreenQuad.fx", tempInput, 1);
//	if(FAILED(hr))
//	{
//		return hr;
//	}
//}
//
//HRESULT D3D11Handler::initDeferredMRT()
//{
//	D3D11_TEXTURE2D_DESC texDesc;
//
//	texDesc.Width = 
//	//create 3 rendertargets, position + diffuse albedo texturemap + normalmap;
//	ds.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
//	ds.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
//
//	hr = this->pDevice->CreateTexture2D(&ds, NULL, &this->mrtRT);
//	if(FAILED(hr))
//	{
//		return hr;
//	}
//
//	//create 3 rendertarget view from the rendertargets
//	D3D11_RENDER_TARGET_VIEW_DESC rtv;
//	rtv.Format = ds.Format;
//	rtv.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
//	rtv.Texture2DArray.FirstArraySlice = 0;
//	rtv.Texture2DArray.ArraySize = this->iNrRTS;
//	rtv.Texture2DArray.MipSlice = 0;
//	
//	hr = this->pDevice->CreateRenderTargetView(this->mrtRT, &rtv, &this->mrtRTV);
//	if(FAILED(hr))
//	{
//		return hr;
//	}
//
//	D3D11_SHADER_RESOURCE_VIEW_DESC srv;
//	srv.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
//	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
//	srv.Texture2DArray.MostDetailedMip = 0;
//	srv.Texture2DArray.FirstArraySlice = 0;
//	srv.Texture2DArray.ArraySize = this->iNrRTS;
//	srv.Texture2DArray.MipLevels = 1;
//	
//	hr = this->pDevice->CreateShaderResourceView(this->mrtRT, &srv, &this->mrtSRV);
//	if(FAILED(hr))
//	{
//		return hr;
//	}
//
//	return hr;
//}
//
//
//HRESULT D3D11Handler::initBackBufferRT()
//{
//	HRESULT hr = S_OK;
//	
//	hr = initBbRTV();
//	if(FAILED(hr))
//	{
//		return hr;
//	}
//
//	hr = initBbZBuffer();
//	if(FAILED(hr))
//	{
//		return hr;
//	}
//
//	return hr;
//}
//HRESULT D3D11Handler::initBbZBuffer()
//{
//	HRESULT hr = S_OK;
//	// Create depth stencil texture
//	D3D11_TEXTURE2D_DESC descDepth;
//	descDepth.Width = SCREEN_WIDTH;
//	descDepth.Height = SCREEN_HEIGHT;
//	descDepth.MipLevels = 1;
//	descDepth.ArraySize = 1;
//	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
//	descDepth.SampleDesc.Count = 1;
//	descDepth.SampleDesc.Quality = 0;
//	descDepth.Usage = D3D11_USAGE_DEFAULT;
//	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
//	descDepth.CPUAccessFlags = 0;
//	descDepth.MiscFlags = 0;
//	hr = this->pDevice->CreateTexture2D( &descDepth, NULL, &this->bbDS );
//	if( FAILED(hr) )
//		return hr;
//
//	// Create the depth stencil view
//	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
//	ZeroMemory(&descDSV, sizeof(descDSV));
//	descDSV.Format = descDepth.Format;
//	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
//	descDSV.Texture2D.MipSlice = 0;
//	hr = this->pDevice->CreateDepthStencilView( this->bbDS, &descDSV, &this->bbDSV);
//	if( FAILED(hr) )
//		return hr;
//
//	return hr;
//}
//
//
//void D3D11Handler::clearViews()
//{
//	static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
//	this->pDeviceContext->ClearRenderTargetView(this->bbRTV, ClearColor);
//	this->pDeviceContext->ClearRenderTargetView(this->mrtRTV, ClearColor);
//
//	
//	this->pDeviceContext->ClearDepthStencilView(this->bbDSV, D3D11_CLEAR_DEPTH, 1.0f, 0 );
//	this->pDeviceContext->ClearDepthStencilView(this->mrtDSV, D3D11_CLEAR_DEPTH, 1.0f, 0 );
//}
//Shader* D3D11Handler::setPass(PASS_STATE pass)
//{
//	UINT i;
//
//	switch(pass)
//	{
//		case PASS_GEOMETRY:
//			this->pDeviceContext->OMSetRenderTargets(1, &this->mrtRTV, this->mrtDSV);
//			return this->vShaders.at(PASS_GEOMETRY);
//			break;
//
//		case PASS_LIGHT:
//			return this->vShaders.at(PASS_LIGHT);
//			break;
//
//		case PASS_FULLSCREENQUAD:
//			this->pDeviceContext->OMSetRenderTargets(1, &this->bbRTV, this->bbDSV);
//			this->vShaders.at(PASS_FULLSCREENQUAD)->SetResource("resources" , this->mrtSRV);
//			//this->vShaders.at(PASS_FULLSCREENQUAD)->SetResource("positionMap" , &this->mrtSRV[0]);
//			//this->vShaders.at(PASS_FULLSCREENQUAD)->SetResource("diffuseAlbedoMap" , &this->mrtSRV[1]);
//			//this->vShaders.at(PASS_FULLSCREENQUAD)->SetResource("normalMap" , &this->mrtSRV[2]);
//
//			return this->vShaders.at(PASS_FULLSCREENQUAD);
//			break;
//	}
//}
//
//
//char* D3D11Handler::FeatureLevelToString(D3D_FEATURE_LEVEL featureLevel)
//{
//	if(featureLevel == D3D_FEATURE_LEVEL_11_0)
//		return "11.0";
//	if(featureLevel == D3D_FEATURE_LEVEL_10_1)
//		return "10.1";
//	if(featureLevel == D3D_FEATURE_LEVEL_10_0)
//		return "10.0";
//
//	return "Unknown";
//}

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
	pDSVDeferred		= NULL; //Needed?
	iNrOfDeferred		= 3;
}

D3D11Handler::~D3D11Handler()
{
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pDepthStencil);
	SAFE_RELEASE(pDepthStencilView);

	//Deferred targets
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

		case PASS_FULLSCREENQUAD:
			this->pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
			this->vShaders.at(PASS_FULLSCREENQUAD)->SetResource("resources" , pMultipleSRVs[0]);
			//this->vShaders.at(PASS_FULLSCREENQUAD)->SetResource("positionMap" , &this->mrtSRV[0]);
			//this->vShaders.at(PASS_FULLSCREENQUAD)->SetResource("diffuseAlbedoMap" , &this->mrtSRV[1]);
			//this->vShaders.at(PASS_FULLSCREENQUAD)->SetResource("normalMap" , &this->mrtSRV[2]);

			return this->vShaders.at(PASS_FULLSCREENQUAD);
			break;
	}
}

void D3D11Handler::clearAndBindRenderTarget()
{
	static float clearColour[4] = { 0.6f, 0.3f, 0.3f, 0.4f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, clearColour);
	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	pDeviceContext->OMSetRenderTargets(this->iNrOfDeferred, pMultipleRTVs, pDSVDeferred);
	pDeviceContext->ClearDepthStencilView(pDSVDeferred, 1, 1.0f, 0);

	for(int i = 0; i < this->iNrOfDeferred; i++) pDeviceContext->ClearRenderTargetView(pMultipleRTVs[i], clearColour);
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
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	//createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;

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

		/*{ "TEXTUREID", 0, DXGI_FORMAT_R32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },

		{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 36, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 52, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 68, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 84, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		*/
	};

	Shader* temp = new Shader();
	this->vShaders.push_back(temp);
	hr = this->vShaders.at(PASS_GEOMETRY)->Init(this->pDevice, this->pDeviceContext, "../Shaders/Geometry.fx", inputDesc, 3);
	if(FAILED(hr))
	{
		return false;
	}

	temp = new Shader();
	this->vShaders.push_back(temp);
	//hr = this->vShaders.at(PASS_LIGHT)->Init(this->pDevice, this->pDeviceContext, "../Shaders/Lightning.fx", inputDesc, 3);
	if(FAILED(hr))
	{
		return false;
	}
	

	D3D11_INPUT_ELEMENT_DESC tempInput[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	temp = new Shader();
	this->vShaders.push_back(temp);
	hr = this->vShaders.at(PASS_FULLSCREENQUAD)->Init(this->pDevice, this->pDeviceContext, "../Shaders/FullScreenQuad.fx", tempInput, 1);
	if(FAILED(hr))
	{
		return false;
	}

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
