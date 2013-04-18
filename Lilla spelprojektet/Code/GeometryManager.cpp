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
void GeometryManager::initVertexBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit)
{
	vVertexBuffer.push_back(pBufferObj->initBuffer(device, bufferInit));
}

void GeometryManager::initIndexBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit)
{
	vIndexBuffer.push_back(pBufferObj->initBuffer(device, bufferInit));
}

void GeometryManager::ApplyBuffer(ID3D11DeviceContext *dc, RENDERDATA obj, D3D_PRIMITIVE_TOPOLOGY topology, UINT32 misc = 0)
{
	UINT strides	= 0;
	UINT offset		= 0;

	if(obj.iEntityID != GUI)
		strides = sizeof(MESH_PNUV);
	else
		strides = sizeof(MESH_PUV);

	dc->IASetVertexBuffers(misc, 1, &vVertexBuffer[obj.iEntityID], &strides, &offset);
	
	dc->IASetIndexBuffer(vIndexBuffer[obj.iEntityID], DXGI_FORMAT_R32_UINT, offset);

	dc->IASetPrimitiveTopology(topology);
}