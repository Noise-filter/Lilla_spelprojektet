#include "D3D11Handler.h"

D3D11Handler::D3D11Handler()
{
	this->pSwapChain        = NULL;
	this->pBackBufferV      = NULL;

	this->pDepthStencil     = NULL;
	this->pDepthStencilView = NULL;
	this->pDepthState       = NULL;

	this->pDevice           = NULL;
	this->pDeviceContext    = NULL;
	
	this->pRTs              = NULL;
	this->pRTVs             = NULL;
	this->iNrRTS            = 0;
}

D3D11Handler::~D3D11Handler()
{	
	for(int i = 0; i < this->vShaders.size(); i++)
	{
		delete this->vShaders.at(i);
	}
	
	SAFE_RELEASE(this->pDepthState);
	SAFE_RELEASE(this->pSwapChain)
	SAFE_RELEASE(this->pRTs);
	SAFE_RELEASE(this->pRTVs);
	SAFE_RELEASE(this->pDepthStencil);
	SAFE_RELEASE(this->pDepthStencilView);
	SAFE_RELEASE(this->pDevice);
	SAFE_RELEASE(this->pDeviceContext);
	SAFE_RELEASE(this->pBackBufferV);

	SAFE_DELETE(this->pViewPort);
}


HRESULT D3D11Handler::InitDirect3D(HWND hWnd)
{
	HRESULT hr = S_OK;

	hr = initSwapChainAndDevice(hWnd);
	if(FAILED(hr))
	{
		return hr;
	}


	hr = initZBuffer();
	if(FAILED(hr))
	{
		return hr;
	}

	hr = createDSS();
	if(FAILED(hr))
	{
		return hr;
	}
	
	//create forward RTV and RT
	hr = createBackBufferV();
	if(FAILED(hr))
	{
		return hr;
	}
	
	//set renderTargetview backBuffer
	setRT(1, this->pBackBufferV);

	// Setup the viewport
	setupVP();

	//initiate shaders for deferred
	hr = initShaders();
	if(FAILED(hr))
	{
		return hr; 
	}

	return S_OK;
}

Shader* D3D11Handler::setPass(PASS_STATE pass)
{
	switch(pass)
	{
		case GEOMETRY:
			return this->vShaders.at(GEOMETRY);
			break;

		case LIGHT:
			return this->vShaders.at(LIGHT);
			break;

		case FULLSCREENQUAD:
			return this->vShaders.at(FULLSCREENQUAD);
			break;
	}
}

HRESULT D3D11Handler::initShaders()
{
	HRESULT hr;

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	
	Shader* temp = new Shader();
	this->vShaders.push_back(temp);
	hr = this->vShaders.at(GEOMETRY)->Init(this->pDevice, this->pDeviceContext, "../Shaders/Geometry.fx", inputDesc, 3);
	if(FAILED(hr))
	{
		return hr;
	}

	temp = new Shader();
	this->vShaders.push_back(temp);
	hr = this->vShaders.at(LIGHT)->Init(this->pDevice, this->pDeviceContext, "../Shaders/Lightning.fx", inputDesc, 3);
	if(FAILED(hr))
	{
		return hr;
	}
	
	temp = new Shader();
	this->vShaders.push_back(temp);
	hr = this->vShaders.at(FULLSCREENQUAD)->Init(this->pDevice, this->pDeviceContext, "../Shaders/FullScreenQuad.fx", inputDesc, 3);
	if(FAILED(hr))
	{
		return hr;
	}
	
}
void D3D11Handler::setRT(int nrOfRT, ID3D11RenderTargetView* pRTVs)
{
	this->pDeviceContext->OMSetRenderTargets(nrOfRT, &pRTVs, this->pDepthStencilView);
}
HRESULT D3D11Handler::createBackBufferV()
{
	HRESULT hr = S_OK;
	// Create a render target view
	ID3D11Texture2D* pBackBuffer;
	hr = this->pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&pBackBuffer );
	if( FAILED(hr) )
		return hr;

	hr = this->pDevice->CreateRenderTargetView( pBackBuffer, NULL, &this->pBackBufferV );
	pBackBuffer->Release();
	if( FAILED(hr) )
		return hr;

	return hr;
}
void D3D11Handler::setupVP()
{
	this->pViewPort = new D3D11_VIEWPORT;
	this->pViewPort->Width = SCREEN_WIDTH;
	this->pViewPort->Height = SCREEN_HEIGHT;
	this->pViewPort->MinDepth = 0.0f;
	this->pViewPort->MaxDepth = 1.0f;
	this->pViewPort->TopLeftX = 0;
	this->pViewPort->TopLeftY = 0;

	this->pDeviceContext->RSSetViewports( 1, this->pViewPort );
}
HRESULT D3D11Handler::initZBuffer()
{
	HRESULT hr = S_OK;
	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = SCREEN_WIDTH;
	descDepth.Height = SCREEN_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = this->pDevice->CreateTexture2D( &descDepth, NULL, &this->pDepthStencil );
	if( FAILED(hr) )
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = this->pDevice->CreateDepthStencilView( this->pDepthStencil, &descDSV, &this->pDepthStencilView );
	if( FAILED(hr) )
		return hr;

	return hr;
}
HRESULT D3D11Handler::initSwapChainAndDevice(HWND hWnd)
{
	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	
	D3D_DRIVER_TYPE driverType;
	
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_REFERENCE
	};

	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);
	
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevelsToTry[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			driverType,
			NULL,
			createDeviceFlags,
			featureLevelsToTry,
			ARRAYSIZE(featureLevelsToTry),
			D3D11_SDK_VERSION,
			&sd,
			&this->pSwapChain,
			&this->pDevice,
			&initiatedFeatureLevel,
			&this->pDeviceContext);

		if( SUCCEEDED( hr ) )
		{
			char title[256];
			sprintf_s(
				title,
				sizeof(title),
				"BTH - Direct3D 11.0 Template | Direct3D 11.0 device initiated with Direct3D %s feature level",
				FeatureLevelToString(initiatedFeatureLevel)
			);
			SetWindowText(hWnd, title);

			break;
		}
	}
	if( FAILED(hr) )
		return hr;

	return hr;
}
char* D3D11Handler::FeatureLevelToString(D3D_FEATURE_LEVEL featureLevel)
{
	if(featureLevel == D3D_FEATURE_LEVEL_11_0)
		return "11.0";
	if(featureLevel == D3D_FEATURE_LEVEL_10_1)
		return "10.1";
	if(featureLevel == D3D_FEATURE_LEVEL_10_0)
		return "10.0";

	return "Unknown";
}
void D3D11Handler::resetRT()
{
	this->pDeviceContext->OMSetRenderTargets( 1, &this->pBackBufferV, this->pDepthStencilView );

	this->pDeviceContext->RSSetViewports( 1, this->pViewPort);
}
HRESULT D3D11Handler::createDSS()
{
	HRESULT hr = S_OK;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		depthStencilDesc.DepthEnable = FALSE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = FALSE;
		depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

		hr =this->pDevice->CreateDepthStencilState(&depthStencilDesc, &this->pDepthState);
	
	return hr;
}

void D3D11Handler::setFSQDepth()
{
	this->pDeviceContext->OMSetDepthStencilState(this->pDepthState, 1);
}

void D3D11Handler::resetDSS()
{
	this->pDeviceContext->OMSetDepthStencilState(NULL, 0);
}