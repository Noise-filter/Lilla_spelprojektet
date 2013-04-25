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

	hr = d3d->InitDirect3D(win32->getHWND()); // initierar directX
	if(FAILED(hr))
	{
		return false;
	}

	//pGeoManager->init(d3d->pDevice);

	return true; // allt gick bra
}

void Engine::render(std::vector<std::vector<RENDERDATA*>> data)
{
	d3d->clearViews();

	int index = 0;
	PRIMITIVE_TOPOLOGIES topology = TOPOLOGY_UNDEFINED;
	


	D3DXMATRIX world, view, proj, wvp;
	D3DXMatrixIdentity(&world);
	D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0,0,-5), &D3DXVECTOR3(0,0,0), &D3DXVECTOR3(0,1,0));
	D3DXMatrixPerspectiveFovLH(&proj, (float)D3DX_PI * 0.45f, 1024.0f / 768.0f, 1.0f, 100.0f);
	wvp = world * view * proj;
	
	Shader* temp;
			temp = this->d3d->setPass(PASS_GEOMETRY);
	temp->SetMatrix("world", world);
	temp->SetMatrix("viewProj", view * proj);
	temp->SetMatrix("worldViewProj", wvp);
	
			UINT strides = sizeof(MESH_P);
	UINT offset = 0;
	

		
	


	ID3D11Buffer *b;
		MESH_P mesh[] = {

		MESH_P(Vec3(1.0,0,0)),
		MESH_P(Vec3(-1.0,0,0)),
		MESH_P(Vec3(0,1.0,0)),
		MESH_P(Vec3(0,0,1.0)),
		MESH_P(Vec3(0,0,-1.0)),
		MESH_P(Vec3(0,-1.0,0))
	};
		D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(MESH_P)*6;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA xdata;
	xdata.pSysMem = mesh;
	xdata.SysMemPitch = 0;
	xdata.SysMemSlicePitch = 0;

	d3d->pDevice->CreateBuffer(&desc, &xdata, &b);

	d3d->pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3d->pDeviceContext->IASetVertexBuffers(0, 1, &b, &strides, &offset);

	//this->pGeoManager->geoDebugBuffer(d3d->pDeviceContext, d3d->pDevice);
	temp->Apply(0);
	d3d->pDeviceContext->Draw(6, 0);

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
	ID3D11Buffer *bxv;
	MESH_P p[] = {  MESH_P(D3DXVECTOR3(1,-1,0)),
									MESH_P(D3DXVECTOR3(-1,-1,0)), 
									MESH_P(D3DXVECTOR3(1,1,0)),
									MESH_P(D3DXVECTOR3(1,1,0)),  
									MESH_P(D3DXVECTOR3(-1,-1,0)), 
									MESH_P(D3DXVECTOR3(-1,1,0)), 
	};

	D3D11_BUFFER_DESC zdesc;
	zdesc.Usage = D3D11_USAGE_DEFAULT;
	zdesc.ByteWidth = sizeof(MESH_P)*6;
	zdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	zdesc.CPUAccessFlags = 0;
	zdesc.MiscFlags = 0;
	zdesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA zdata;
	zdata.pSysMem = p;
	zdata.SysMemPitch = 0;
	zdata.SysMemSlicePitch = 0;

	d3d->pDevice->CreateBuffer(&zdesc, &zdata, &bxv);
	
	d3d->pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3d->pDeviceContext->IASetVertexBuffers(0, 1, &bxv, &strides, &offset);
	//this->pGeoManager->debugApplyBuffer(d3d->pDeviceContext, d3d->pDevice);
	temp->Apply(0);
	this->d3d->pDeviceContext->Draw(6, 0);
	
	if(FAILED(d3d->pSwapChain->Present( 0, 0 )))
	{
		return;
	}
}

PRIMITIVE_TOPOLOGIES Engine::changeTopology(int ID)
{
	if(ID != ENTITY_PARTICLESYSTEM) return TOPOLOGY_TRIANGLELIST;
	else return TOPOLOGY_POINTLIST;
}
