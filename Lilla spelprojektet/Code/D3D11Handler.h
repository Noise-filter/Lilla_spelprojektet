#ifndef D3D11HANDLER_H
#define D3D11HANDLER_H

#include "D3DGraphicUtility.h"
#include "D3DMathUtility.h"
#include "WindowsUtility.h"
#include "Shader.h"

class D3D11Handler		
{
	private:
		char*   FeatureLevelToString(D3D_FEATURE_LEVEL featureLevel);
		void    setRT(int nrOfRT, ID3D11RenderTargetView* pRTVs);
		HRESULT initSwapChainAndDevice(HWND hWnd);
		HRESULT initShaders();
		void    initVP();
		
		//private methods
		HRESULT initDeferredMRT();
			HRESULT initMrtZBuffer();
			HRESULT initMrtRT();

		HRESULT initBackBufferRT();
			HRESULT initBbRTV();
			HRESULT initBbZBuffer();


		//Variables
		D3D11_VIEWPORT*          pVP;
		
		//rendertargets for deferred
		ID3D11Texture2D*          mrtRT;
		ID3D11RenderTargetView*   mrtRTV;
		ID3D11Texture2D*          mrtDS;
		ID3D11DepthStencilView*   mrtDSV;
		ID3D11ShaderResourceView* mrtSRV;
		int iNrRTS;

		//renderTarget for Full ScreenQuad
		ID3D11RenderTargetView* bbRTV;
		ID3D11Texture2D*        bbDS;
		ID3D11DepthStencilView* bbDSV;


		//Shaders for different passes
		std::vector<Shader*> vShaders;

	public:
		//Variables
		ID3D11Device*			pDevice;
		ID3D11DeviceContext*	pDeviceContext;
		IDXGISwapChain*         pSwapChain;	
			

		//Functions
		D3D11Handler();
		~D3D11Handler();
		HRESULT InitDirect3D(HWND hWnd);
		
		Shader* setPass(PASS_STATE pass);
		void clearViews();
	
};

#endif