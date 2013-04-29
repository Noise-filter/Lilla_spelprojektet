#ifndef D3D11HANDLER_H
#define D3D11HANDLER_H

#include "D3DGraphicUtility.h"
#include "D3DMathUtility.h"
#include "WindowsUtility.h"
#include "Shader.h"

class D3D11Handler		
{
	private:
		D3D11_VIEWPORT			viewPort;
		ID3D11RenderTargetView	*pRenderTargetView;
		
		ID3D11Texture2D			*pDepthStencil;
		ID3D11DepthStencilView	*pDepthStencilView;

		//Deferred targets
		ID3D11Texture2D				**pDeferredTargets;
		ID3D11RenderTargetView		**pMultipleRTVs;
		ID3D11ShaderResourceView	**pMultipleSRVs;
		ID3D11ShaderResourceView	**pNullSRVs;
		ID3D11DepthStencilView		*pDSVDeferred;
		int							 iNrOfDeferred;
		std::vector<Shader*>         vShaders;

	private:
		bool initSwapChainAndDevice(HWND hWnd);
		void initAndSetViewPort();
		bool initDepthStencil();
		bool initRenderTargetView();
		bool initShaders();

		//Deferred initiation
		bool initDeferred();
		bool bindResources(D3D11_TEXTURE2D_DESC &texDesc);
		//--------------

		

	public:
		ID3D11Device		*pDevice;
		ID3D11DeviceContext *pDeviceContext;
		IDXGISwapChain		*pSwapChain;

	public:
		D3D11Handler();
		virtual~D3D11Handler();

		bool initDirect3D(HWND hWnd);

		Shader *setPass(PASS_STATE pass);

		void clearAndBindRenderTarget();
	
};

#endif