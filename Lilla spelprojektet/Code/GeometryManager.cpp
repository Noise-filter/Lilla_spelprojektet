#include "GeometryManager.h"

GeometryManager::GeometryManager()
{
	pBufferObj = new Buffer();
}

GeometryManager::~GeometryManager()
{
	SAFE_DELETE(pBufferObj);
}

//bool importMesh(); TBD

void GeometryManager::init(ID3D11Device *device)
{
	BUFFER_INIT bufferInit		= BUFFER_INIT();
	BUFFER_INIT instanceInit	= BUFFER_INIT();
	
	MESH_P mesh[] = {

		MESH_P(Vec3(1.0,0,0)),
		MESH_P(Vec3(-1.0,0,0)),
		MESH_P(Vec3(0,1.0,0)),
		MESH_P(Vec3(0,0,1.0)),
		MESH_P(Vec3(0,0,-1.0)),
		MESH_P(Vec3(0,-1.0,0))
	};

	bufferInit.desc.eUsage = D3D11_USAGE_DEFAULT;
	bufferInit.desc.uByteWidth = sizeof(MESH_P)*6;
	bufferInit.desc.uBindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferInit.desc.uCPUAccessFlags = 0;
	bufferInit.desc.uMiscFlags = 0;
	bufferInit.desc.uStructureByteStride = 0;

	bufferInit.data.pInitData = mesh;
	bufferInit.data.uSysMemPitch = 0;
	bufferInit.data.uSysMemSlicePitch = 0;

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
	bufferInit.desc.uCPUAccessFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferInit.desc.uCPUAccessFlags = 0;
	bufferInit.desc.uMiscFlags = 0;
	bufferInit.desc.uStructureByteStride = 0;

	bufferInit.data.pInitData = p;
	bufferInit.data.uSysMemPitch = 0;
	bufferInit.data.uSysMemSlicePitch = 0;

	initVertexBuffer(device, bufferInit);

	/*initIndexBuffer(device, bufferInit);

	initInstance(device, instanceInit);*/
}

void GeometryManager::applyBuffer(ID3D11DeviceContext *dc, RENDERDATA *obj, D3D_PRIMITIVE_TOPOLOGY topology, UINT32 misc)
{
	UINT strides[2]		= {0, sizeof(OBJECT_WORLD_AND_TEXTURE)};
	UINT offset[2]		= {0, 0};
	buffers[0] = vVertexBuffer[(int)obj->iEntityID];
	buffers[1] = vInstanceBuffer[(int)obj->iEntityID];

	if(obj->iEntityID != ENTITY_GUI) strides[0] = sizeof(MESH_PNUV);
	else strides[0] = sizeof(MESH_PUV);

	dc->IASetVertexBuffers(misc, 2, buffers, strides, offset);
	
	dc->IASetIndexBuffer(vIndexBuffer[(int)obj->iEntityID], DXGI_FORMAT_R32_UINT, offset[0]);

	dc->IASetPrimitiveTopology(topology);
}

void GeometryManager::updateBuffer(ID3D11DeviceContext *dc, std::vector<RENDERDATA*> data, int index)
{
	D3D11_MAPPED_SUBRESOURCE *mappedData;
	OBJECT_WORLD_AND_TEXTURE *instance;

	mappedData = (D3D11_MAPPED_SUBRESOURCE*)map(dc, vInstanceBuffer[index]);
	instance = (OBJECT_WORLD_AND_TEXTURE*)mappedData->pData;

	for(int i = 0; i < (int)data.size(); i++)
	{
		instance[i].mWorld			= data[index][i].worldTex.mWorld;
		instance[i].iTextureID		= data[index][i].worldTex.iTextureID;
	}

	unmap(dc, vInstanceBuffer[index]); 
}

void GeometryManager::debugApplyBuffer(ID3D11DeviceContext *dc, int ID)
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

void *GeometryManager::map(ID3D11DeviceContext *dc, ID3D11Buffer *buffer)
{
	void* ret = NULL;
	D3D11_BUFFER_DESC bufferUsage;
	buffer->GetDesc(&bufferUsage);

	if(bufferUsage.CPUAccessFlags == WRITE_DISCARD || bufferUsage.Usage == WRITE_NO_OVERWRITE)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		D3D11_MAP mapType;

		if(bufferUsage.CPUAccessFlags == (UINT)WRITE_DISCARD) mapType = (D3D11_MAP)WRITE_DISCARD;
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
			ret = mappedResource.pData;
		}
	}

	return ret;

}

void GeometryManager::unmap(ID3D11DeviceContext *dc, ID3D11Buffer *buffer)
{
	dc->Unmap(buffer, 0);
}