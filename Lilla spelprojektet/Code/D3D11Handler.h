#ifndef D3D11HANDLER_H
#define D3D11HANDLER_H

#include "D3DGraphicUtility.h"
#include "D3DMathUtility.h"
#include "WindowsUtility.h"
#include "Shader.h"

class D3D11Handler		
{
	private:
		//private functions
		char* FeatureLevelToString(D3D_FEATURE_LEVEL featureLevel);
		HRESULT initSwapChainAndDevice(HWND hWnd);
		HRESULT initZBuffer();
		void setupVP();
		HRESULT createBackBufferV();
		void setRT(int nrOfRT, ID3D11RenderTargetView* pRTVs);
		HRESULT createDSS();
		HRESULT initShaders();
		HRESULT createMRTS();


		//Variables
		ID3D11DepthStencilState* pDepthState;
		ID3D11Texture2D*         pDepthStencil; 
		D3D11_VIEWPORT*          pViewPort;
		
		//rendertargets for deferred
		ID3D11Texture2D* pRTs;
		ID3D11RenderTargetView* pRTVs;
		int iNrRTS;

		//Shaders for different passes
		std::vector<Shader*> vShaders;

	public:
		//Variables
		ID3D11Device*			pDevice;
		ID3D11DeviceContext*	pDeviceContext;
		IDXGISwapChain*         pSwapChain;	
		ID3D11DepthStencilView* pDepthStencilView;
		ID3D11RenderTargetView* pBackBufferV;	

		//Functions
		D3D11Handler();
		~D3D11Handler();
		HRESULT InitDirect3D(HWND hWnd);
		void resetRT();
		Shader* setPass(PASS_STATE pass);
		void setFSQDepth();
		void resetDSS();
};

#endif