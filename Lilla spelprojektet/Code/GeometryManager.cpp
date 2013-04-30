#include "GeometryManager.h"

GeometryManager::GeometryManager()
{
	pBufferObj = new Buffer();
}

GeometryManager::~GeometryManager()
{
	SAFE_RELEASE(buffers[0]);
	SAFE_RELEASE(buffers[1]);

	for(int i = 0; i < vIndexBuffer.size(); i++)
	{
		vIndexBuffer[i]->Release();
	}

	for(int i = 0; i < vVertexBuffer.size(); i++)
	{
		vVertexBuffer[i]->Release();
	}

	for(int i = 0; i < vInstanceBuffer.size(); i++)
	{
		vInstanceBuffer[i]->Release();
	}

	SAFE_DELETE(pBufferObj);
}

//bool importMesh(); TBD

void GeometryManager::init(ID3D11Device *device)
{
	BUFFER_INIT bufferInit		= BUFFER_INIT();
	BUFFER_INIT instanceInit	= BUFFER_INIT();
	
	MESH_PNUV mesh[] = {

		MESH_PNUV(Vec3(1.0,0,0), Vec3(0,1,1), Vec2(0,0)),
		MESH_PNUV(Vec3(-1.0,0,0), Vec3(0,0,1), Vec2(0,0)),
		MESH_PNUV(Vec3(0,1.0,0), Vec3(1,0,1), Vec2(0,0)),
	};

	bufferInit.desc.eUsage = D3D11_USAGE_DEFAULT;
	bufferInit.desc.uByteWidth = sizeof(MESH_PNUV)*3;
	bufferInit.desc.uBindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferInit.desc.uCPUAccessFlags = 0;
	bufferInit.desc.uMiscFlags = 0;
	bufferInit.desc.uStructureByteStride = 0;

	bufferInit.data.pInitData = mesh;
	bufferInit.data.uSysMemPitch = 0;
	bufferInit.data.uSysMemSlicePitch = 0;

	initVertexBuffer(device, bufferInit);

	MESH_PNUV diamond[] ={
		MESH_PNUV(Vec3(1.0,0,0), Vec3(1,0,0), Vec2(1,1)),
		MESH_PNUV(Vec3(-1.0,0,0), Vec3(0,0,1), Vec2(-1,0)),
		MESH_PNUV(Vec3(0,1.0,0), Vec3(0,1,0), Vec2(1,0)),

		MESH_PNUV(Vec3(1,0,0), Vec3(0,0,1), Vec2(0,0)),
		MESH_PNUV(Vec3(-1,0,0), Vec3(0,1,0), Vec2(0,-1)),
		MESH_PNUV(Vec3(0,-1.0,0), Vec3(1,0,0), Vec2(1,1)),
	};

	bufferInit.desc.uByteWidth = sizeof(MESH_PNUV)*6;
	bufferInit.data.pInitData = diamond;

	initVertexBuffer(device, bufferInit);

	MESH_PNUV quad[] ={
		MESH_PNUV(Vec3(-1,1,0), Vec3(1,0,1), Vec2(1,1)),
		MESH_PNUV(Vec3(1,1,0), Vec3(1,0,1), Vec2(-1,0)),
		MESH_PNUV(Vec3(1,0,0), Vec3(0,1,0), Vec2(1,0)),

		MESH_PNUV(Vec3(-1,1,0), Vec3(1,0,1), Vec2(0,0)),
		MESH_PNUV(Vec3(-1,0,0), Vec3(0,1,1), Vec2(0,-1)),
		MESH_PNUV(Vec3(1,0,0), Vec3(1,1,0), Vec2(1,1)),
	};

		bufferInit.desc.uByteWidth = sizeof(MESH_PNUV)*6;
	bufferInit.data.pInitData = quad;

	initVertexBuffer(device, bufferInit);

	MESH_PNUV node[] = {

		MESH_PNUV(Vec3(0,0.5,0), Vec3(1,1,0), Vec2(0,0)),
		MESH_PNUV(Vec3(0,0,0.5), Vec3(1,0,1), Vec2(0,0)),
		MESH_PNUV(Vec3(0,-0.5,0), Vec3(1,0,0), Vec2(0,0)),
	};
		bufferInit.desc.uByteWidth = sizeof(MESH_PNUV)*3;
	bufferInit.data.pInitData = node;
	initVertexBuffer(device, bufferInit);

		MESH_P p[] = {  MESH_P(D3DXVECTOR3(1,-1,0)),
						MESH_P(D3DXVECTOR3(-1,-1,0)), 
						MESH_P(D3DXVECTOR3(1,1,0)),
						MESH_P(D3DXVECTOR3(1,1,0)),  
						MESH_P(D3DXVECTOR3(-1,-1,0)), 
						MESH_P(D3DXVECTOR3(-1,1,0)), 
	};

	bufferInit.desc.eUsage = D3D11_USAGE_DEFAULT;
	bufferInit.desc.uByteWidth = sizeof(MESH_P)*6;
	bufferInit.desc.uBindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferInit.desc.uCPUAccessFlags = 0;
	bufferInit.desc.uMiscFlags = 0;
	bufferInit.desc.uStructureByteStride = 0;

	bufferInit.data.pInitData = p;
	bufferInit.data.uSysMemPitch = 0;
	bufferInit.data.uSysMemSlicePitch = 0;

	initVertexBuffer(device, bufferInit);

	/*initIndexBuffer(device, bufferInit);*/

	instanceInit.desc.eUsage				= D3D11_USAGE_DYNAMIC;
	instanceInit.desc.uByteWidth			= sizeof(OBJECT_WORLD_AND_TEXTURE);
	instanceInit.desc.uBindFlags			= D3D11_BIND_VERTEX_BUFFER;
	instanceInit.desc.uCPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	instanceInit.desc.uMiscFlags			= 0;
	instanceInit.desc.uStructureByteStride	= 0;

	initInstance(device, instanceInit);
	initInstance(device, instanceInit);
	initInstance(device, instanceInit);
	initInstance(device, instanceInit);

	//initInstance(device, instanceInit);
}

void GeometryManager::applyBuffer(ID3D11DeviceContext *dc, std::vector<RenderData*> obj, D3D_PRIMITIVE_TOPOLOGY topology, UINT32 misc)
{
	UINT strides[2]		= {0, sizeof(OBJECT_WORLD_AND_TEXTURE)};
	UINT offset[2]		= {0, 0};
	buffers[0] = vVertexBuffer[(int)obj[0]->iEntityID];
	buffers[1] = vInstanceBuffer[(int)obj[0]->iEntityID];

	if(obj[0]->iEntityID != ENTITY_GUI) strides[0] = sizeof(MESH_PNUV);
	else strides[0] = sizeof(MESH_PUV);

	dc->IASetVertexBuffers(misc, 2, buffers, strides, offset);
	
	//dc->IASetIndexBuffer(vIndexBuffer[(int)obj->iEntityID], DXGI_FORMAT_R32_UINT, offset[0]);

	dc->IASetPrimitiveTopology(topology);
}

void GeometryManager::updateBuffer(ID3D11DeviceContext *dc, std::vector<RenderData*> data, int index)
{
	D3D11_MAPPED_SUBRESOURCE *mappedData = map(dc, vInstanceBuffer[index]);

	OBJECT_WORLD_AND_TEXTURE *instance = reinterpret_cast<OBJECT_WORLD_AND_TEXTURE*>(mappedData->pData);

	for(int i = 0; i < (int)data.size(); i++)
	{
		instance[i].mWorld		= data[i]->worldTex.mWorld;
		instance[i].iTextureID	= data[i]->worldTex.iTextureID;
	}

	unmap(dc, vInstanceBuffer[index]); 
}

void GeometryManager::applyQuadBuffer(ID3D11DeviceContext *dc, int ID)
{
	UINT strides = sizeof(MESH_P);
	UINT offset = 0;

	dc->IASetVertexBuffers(0, 1, &vVertexBuffer[ID], &strides, &offset);
	dc->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

//Private functions
void GeometryManager::initVertexBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit)
{
	vVertexBuffer.push_back(pBufferObj->initBuffer(device, bufferInit));
}

void GeometryManager::initIndexBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit)
{
	vIndexBuffer.push_back(pBufferObj->initBuffer(device, bufferInit));
}

void GeometryManager::initInstance(ID3D11Device *device, BUFFER_INIT &bufferInit)
{
	vInstanceBuffer.push_back(pBufferObj->initInstance(device, bufferInit));
}

D3D11_MAPPED_SUBRESOURCE *GeometryManager::map(ID3D11DeviceContext *dc, ID3D11Buffer *buffer)
{
	D3D11_MAPPED_SUBRESOURCE* ret = NULL;
	D3D11_BUFFER_DESC bufferUsage;
	buffer->GetDesc(&bufferUsage);
	UINT i = bufferUsage.CPUAccessFlags;
	i = (UINT)bufferUsage.Usage;
	if(bufferUsage.CPUAccessFlags == (UINT)D3D11_CPU_ACCESS_WRITE || bufferUsage.Usage == WRITE_NO_OVERWRITE)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D11_MAP mapType;

		if(bufferUsage.CPUAccessFlags == (UINT)D3D11_CPU_ACCESS_WRITE) mapType = (D3D11_MAP)WRITE_DISCARD;
		else if(bufferUsage.CPUAccessFlags == (UINT)WRITE_NO_OVERWRITE) mapType = (D3D11_MAP)WRITE_NO_OVERWRITE;

		HRESULT hr = S_OK;
		if(FAILED(hr = dc->Map(
			buffer,
			0,
			mapType,
			0,
			&mappedResource)))
		{
			ret = NULL;
		}
		else
		{
			ret = &mappedResource;
		}
	}

	return ret;
}

void GeometryManager::unmap(ID3D11DeviceContext *dc, ID3D11Buffer *buffer)
{
	dc->Unmap(buffer, 0);
}

int GeometryManager::getNrOfInstances(int index)
{
	D3D11_BUFFER_DESC temp;
	int ret = 0;
	vInstanceBuffer[index]->GetDesc(&temp);
	return temp.ByteWidth / sizeof(OBJECT_WORLD_AND_TEXTURE);
}

int GeometryManager::getNrOfVertexPoints(int index)
{
	D3D11_BUFFER_DESC temp;
	int ret = 0;
	vVertexBuffer[index]->GetDesc(&temp);
	return temp.ByteWidth / sizeof(MESH_PNUV);
}