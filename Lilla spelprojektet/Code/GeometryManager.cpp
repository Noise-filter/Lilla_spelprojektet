#include "GeometryManager.h"

GeometryManager::GeometryManager()
{
	this->pBufferObj = new Buffer();
	this->iNrOfBuffers = NROFDIFFERENTMESHES;
	this->vVertexBuffer.resize(iNrOfBuffers + 2, NULL);
	this->vInstanceBuffer.resize(iNrOfBuffers, NULL);
	this->vNrOfInstances.resize(iNrOfBuffers, NULL);
	this->importer = NULL;

	this->iNrOfParticles = 0;

}
GeometryManager::~GeometryManager()
{
	SAFE_RELEASE(buffers[0]);
	SAFE_RELEASE(buffers[1]);

	for(int i = 0; i < (int)vIndexBuffer.size(); i++)
	{
		vIndexBuffer[i]->Release();
	}

	for(int i = 0; i < (int)vVertexBuffer.size(); i++)
	{
		vVertexBuffer[i]->Release();
	}

	for(int i = 0; i < (int)vInstanceBuffer.size(); i++)
	{
		vInstanceBuffer[i]->Release();
	}

	SAFE_DELETE(pBufferObj);
	SAFE_DELETE(this->importer);
}

//bool importMesh(); TBD

void GeometryManager::init(ID3D11Device *device)
{
	BUFFER_INIT bufferInit		= BUFFER_INIT();
	BUFFER_INIT instanceInit	= BUFFER_INIT();
	BUFFER_INIT indexInit		= BUFFER_INIT();
	importer = new OBJReader();
	//Vertex buffer init
	bufferInit.desc.eUsage               = D3D11_USAGE_DEFAULT;
	bufferInit.desc.uBindFlags           = D3D11_BIND_VERTEX_BUFFER;
	bufferInit.desc.uCPUAccessFlags      = 0;
	bufferInit.desc.uMiscFlags           = 0;
	bufferInit.desc.uStructureByteStride = 0;

	bufferInit.data.uSysMemPitch         = 0;
	bufferInit.data.uSysMemSlicePitch    = 0;
	//---------------------------------------------------------------

	//Index buffer init
	// TBD!!!!!!!
	//---------------------------------------------------------------

	//Instance buffer init
	instanceInit.desc.eUsage				= D3D11_USAGE_DYNAMIC;
	instanceInit.desc.uBindFlags			= D3D11_BIND_VERTEX_BUFFER;
	instanceInit.desc.uCPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	instanceInit.desc.uMiscFlags			= 0;
	instanceInit.desc.uStructureByteStride	= 0;
	instanceInit.data.pInitData             = NULL;
	instanceInit.data.uSysMemPitch          = NULL;
	instanceInit.data.uSysMemSlicePitch     = NULL;
	//---------------------------------------------------------------



	//temp meshes
	//------------------------------------------------------------------
	MESH_PNUV mainBuilding[] = {

		MESH_PNUV(Vec3(1.0,0,0), Vec3(0,1,1), Vec2(0,0)),
		MESH_PNUV(Vec3(-1.0,0,0), Vec3(0,0,1), Vec2(0,0)),
		MESH_PNUV(Vec3(0,1.0,0), Vec3(1,0,1), Vec2(0,0)),
			
	};
	vector<string> temp;
	int nrOfVerts = 0;
	MESH_PNUV* mainBuilding2 = importer->getOBJfromFile("Meshar/Main Building.obj", nrOfVerts, temp, temp);
	initEntity(device, bufferInit, instanceInit, mainBuilding2, nrOfVerts, 100, ENTITY_MAINBUILDING );
	delete [] mainBuilding2;

	MESH_PNUV supply[] ={
		MESH_PNUV(Vec3(1.0,0,0), Vec3(1,0,0), Vec2(1,1)),
		MESH_PNUV(Vec3(-1.0,0,0), Vec3(0,0,1), Vec2(-1,0)),
		MESH_PNUV(Vec3(0,1.0,0), Vec3(0,1,0), Vec2(1,0)),

		MESH_PNUV(Vec3(1,0,0), Vec3(0,0,1), Vec2(0,0)),
		MESH_PNUV(Vec3(-1,0,0), Vec3(0,1,0), Vec2(0,-1)),
		MESH_PNUV(Vec3(0,-1.0,0), Vec3(1,0,0), Vec2(1,1)),
	};

	nrOfVerts = 0;
	MESH_PNUV* supply2 = importer->getOBJfromFile("Meshar/Power Building part 1.obj", nrOfVerts, temp, temp);
	initEntity(device, bufferInit, instanceInit, supply2, nrOfVerts, 100, ENTITY_SUPPLYBASE);
	delete[] supply2;

	nrOfVerts = 0;
	MESH_PNUV* supply3 = importer->getOBJfromFile("Meshar/Power Building part 2.obj", nrOfVerts, temp, temp);
	initEntity(device, bufferInit, instanceInit, supply3, nrOfVerts, 100, ENTITY_SUPPLYTOP);
	delete[] supply3;
	
	nrOfVerts = 0;
	MESH_PNUV* tower = importer->getOBJfromFile("Meshar/Tower Part 1.obj", nrOfVerts, temp, temp);
	initEntity(device, bufferInit, instanceInit, tower, nrOfVerts, 100, ENTITY_TOWERTOP );
	delete [] tower;

	nrOfVerts = 0;
	MESH_PNUV* tower2 = importer->getOBJfromFile("Meshar/Tower Part 2.obj", nrOfVerts, temp, temp);
	initEntity(device, bufferInit, instanceInit, tower2, nrOfVerts, 100, ENTITY_TOWERBASE );
	delete [] tower2;

	MESH_PNUV node[] = {

		MESH_PNUV(Vec3(0,0.5,0), Vec3(1,1,0), Vec2(0,0)),
		MESH_PNUV(Vec3(0,0,0.5), Vec3(1,0,1), Vec2(0,0)),
		MESH_PNUV(Vec3(0,-0.5,0), Vec3(1,0,0), Vec2(0,0)),
	};

	nrOfVerts = 0;
	MESH_PNUV* node2 = importer->getOBJfromFile("Meshar/Green node.obj", nrOfVerts, temp, temp);
	initEntity(device, bufferInit, instanceInit, node2, nrOfVerts, 400, ENTITY_NODE_GREEN );
	delete [] node2;

	nrOfVerts = 0;
	MESH_PNUV* node3 = importer->getOBJfromFile("Meshar/Green node.obj", nrOfVerts, temp, temp);
	initEntity(device, bufferInit, instanceInit, node3, nrOfVerts, 400, ENTITY_NODE_RED );
	delete [] node3;

	nrOfVerts = 0;
	MESH_PNUV* enemy = importer->getOBJfromFile("Meshar/Enemy (broken node).obj", nrOfVerts, temp, temp);
	initEntity(device, bufferInit, instanceInit, enemy, nrOfVerts, 400, ENTITY_ENEMY );
	delete [] enemy;

	nrOfVerts = 0;
	MESH_PNUV* proj = importer->getOBJfromFile("Meshar/Very basic disc (projectile).obj", nrOfVerts, temp, temp);
	initEntity(device, bufferInit, instanceInit, proj, nrOfVerts, 400, ENTITY_PROJECTILE );
	delete [] proj;

	//-----------------------------------------------------------------

	//initEntity(device, bufferInit, instanceInit, mainBuilding, 3, 100, ENTITY_MAINBUILDING );
	//initEntity(device, bufferInit, instanceInit, supply,       6, 100, ENTITY_SUPPLY       );
	//initEntity(device, bufferInit, instanceInit, tower,        6, 100, ENTITY_TOWERBASE    );
	//initEntity(device, bufferInit, instanceInit, tower,        6, 100, ENTITY_TOWERTOP     );
	//initEntity(device, bufferInit, instanceInit, node,         3, 400, ENTITY_NODE         );
	//initEntity(device, bufferInit, instanceInit, node,         3, 100, ENTITY_ENEMY        );
	//initEntity(device, bufferInit, instanceInit, mainBuilding, 3, 200, ENTITY_PROJECTILE   );
	initEntity(device, bufferInit, instanceInit, supply,       6, 100, ENTITY_UPGRADE_HP   );
	initEntity(device, bufferInit, instanceInit, supply,       6, 100, ENTITY_UPGRADE_ATKSP);
	initEntity(device, bufferInit, instanceInit, supply,       6, 100, ENTITY_UPGRADE_DMG  );
	initEntity(device, bufferInit, instanceInit, supply,       6, 100, ENTITY_UPGRADE_PRJSP);
	initEntity(device, bufferInit, instanceInit, supply,       6, 100, ENTITY_UPGRADE_RANGE);

	initFullScreenQuad(device, bufferInit);
	initParticleBuffer(device, instanceInit);
}

void GeometryManager::applyBuffer(ID3D11DeviceContext *dc, int ID, D3D_PRIMITIVE_TOPOLOGY topology, UINT32 misc)
{
	UINT strides[2]		= {0, sizeof(INSTANCEDATA)};
	UINT offset[2]		= {0, 0};
	buffers[0] = vVertexBuffer[ID];
	buffers[1] = vInstanceBuffer[ID];

	if(ID != (int)ENTITY_GUI) strides[0] = sizeof(MESH_PNUV);
	else strides[0] = sizeof(MESH_PUV);

	dc->IASetVertexBuffers(misc, 2, buffers, strides, offset);
	
	//dc->IASetIndexBuffer(vIndexBuffer[(int)obj->iEntityID], DXGI_FORMAT_R32_UINT, offset[0]);

	dc->IASetPrimitiveTopology(topology);
}

void GeometryManager::updateBuffer(ID3D11DeviceContext *dc, std::vector<RenderData*> data, int index , int nrOfInstances)
{
	D3D11_MAPPED_SUBRESOURCE *mappedData = map(dc, vInstanceBuffer[index]);

	INSTANCEDATA *instance = reinterpret_cast<INSTANCEDATA*>(mappedData->pData);

	for(int i = 0; i < (int)data.size(); i++)
	{
		instance[i].mWorld		= data[i]->worldMat;
		instance[i].iTextureID	= data[i]->textureID;
	}

	unmap(dc, vInstanceBuffer[index]); 

	this->vNrOfInstances.at(index) = nrOfInstances;
}

void GeometryManager::updateParticles(ID3D11DeviceContext *dc, std::vector<std::vector<MESH_PNC>> data , int nrOfVertices)
{
	D3D11_MAPPED_SUBRESOURCE *mappedData = map(dc, this->vVertexBuffer[this->iNrOfBuffers+1]);

	MESH_PNC *mesh = reinterpret_cast<MESH_PNC*>(mappedData->pData);

	for(int j = 0; j < (int)data.size(); j++)
	{
		for(int i = 0; i < (int)data[j].size(); i++)
		{
			mesh[i + this->iNrOfParticles].pos     = data[j][i].pos;
			mesh[i + this->iNrOfParticles].normal  = data[j][i].normal;
			mesh[i + this->iNrOfParticles].color   = data[j][i].color;
		}
		this->iNrOfParticles += data[j].size();
	}

	unmap(dc, vVertexBuffer[this->iNrOfBuffers+1]);
	
}

void GeometryManager::applyQuadBuffer(ID3D11DeviceContext *dc, int ID , D3D_PRIMITIVE_TOPOLOGY topology)
{
	UINT strides = sizeof(MESH_P);
	UINT offset = 0;

	dc->IASetVertexBuffers(0, 1, &vVertexBuffer[ID], &strides, &offset);
	dc->IASetPrimitiveTopology(topology);

}

void GeometryManager::applyParticleBuffer(ID3D11DeviceContext *dc , D3D_PRIMITIVE_TOPOLOGY topology)
{
	UINT strides = sizeof(MESH_PNC);
	UINT offset = 0;
	dc->IASetVertexBuffers(0 , 1 , &vVertexBuffer[this->iNrOfBuffers+1] , &strides, &offset);
	dc->IASetPrimitiveTopology(topology);
}

int GeometryManager::getNrOfInstances(int index)
{
	if((int)vNrOfInstances.size() > index)
	{
		return this->vNrOfInstances.at(index);
	}

	return -1;
}

int GeometryManager::getNrOfVertexPoints(int index)
{
	D3D11_BUFFER_DESC temp;
	if((int)vVertexBuffer.size() > index)
	{
		vVertexBuffer[index]->GetDesc(&temp);
		return temp.ByteWidth / sizeof(MESH_PNUV);
	}

	return -1;
}

int GeometryManager::getNrOfBuffer()
{
	return this->iNrOfBuffers;
}
int GeometryManager::getNrOfParticles()
{
	return this->iNrOfParticles;
}

void GeometryManager::setNrOfParticles(int value)
{
	this->iNrOfParticles = value;
}
/*
######################################
---------Private functions------------
######################################
*/
void GeometryManager::initVertexBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit, int index)
{
	vVertexBuffer.at(index) = pBufferObj->initBuffer(device, bufferInit);
}

void GeometryManager::initIndexBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit, int index)
{
	vIndexBuffer.at(index) = pBufferObj->initBuffer(device, bufferInit);
}

void GeometryManager::initInstanceBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit, int index)
{
	vInstanceBuffer.at(index) = pBufferObj->initInstance(device, bufferInit);
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

void GeometryManager::initEntity(ID3D11Device *device, BUFFER_INIT &bufferInit, BUFFER_INIT &instanceInit, MESH_PNUV *mesh, int nrOfVertices, int nrOfInstances , ENTITY_FLAGS building)
{
	bufferInit.desc.uByteWidth    = sizeof(MESH_PNUV) * nrOfVertices;
	bufferInit.data.pInitData     = mesh;
	instanceInit.desc.uByteWidth  = sizeof(INSTANCEDATA) * nrOfInstances;

	initVertexBuffer(device, bufferInit, (int)building);
	//TODO: initIndexBuffer!
	initInstanceBuffer(device, instanceInit, (int)building);
}

void GeometryManager::initEntity(ID3D11Device *device, BUFFER_INIT &bufferInit, BUFFER_INIT &instanceInit, MESH_PUV *mesh, int nrOfVertices, int nrOfInstances , ENTITY_FLAGS building)
{
	bufferInit.desc.uByteWidth    = sizeof(MESH_PUV) * nrOfVertices;
	bufferInit.data.pInitData     = mesh;

	instanceInit.desc.uByteWidth  = sizeof(INSTANCEDATA) * nrOfInstances;

	initVertexBuffer(device, bufferInit, (int)building);
	//TODO: initIndexBuffer!
	initInstanceBuffer(device, instanceInit, (int)building);
}

void GeometryManager::initEntity(ID3D11Device *device, BUFFER_INIT &bufferInit, BUFFER_INIT &instanceInit, MESH_P *mesh, int nrOfVertices, int nrOfInstances , ENTITY_FLAGS building)
{
	bufferInit.desc.uByteWidth    = sizeof(MESH_P) * nrOfVertices;
	bufferInit.data.pInitData     = mesh;
	instanceInit.desc.uByteWidth  = sizeof(INSTANCEDATA) * nrOfInstances;

	initVertexBuffer(device, bufferInit, (int)building);
	//TODO: initIndexBuffer!
	initInstanceBuffer(device, instanceInit, (int)building);
}

void GeometryManager::initFullScreenQuad(ID3D11Device *device, BUFFER_INIT &bufferInit)
{
	MESH_P p[] = {  MESH_P(D3DXVECTOR3(1,-1,0)),
						MESH_P(D3DXVECTOR3(-1,-1,0)), 
						MESH_P(D3DXVECTOR3(1,1,0)),
						MESH_P(D3DXVECTOR3(1,1,0)),  
						MESH_P(D3DXVECTOR3(-1,-1,0)), 
						MESH_P(D3DXVECTOR3(-1,1,0)), 
	};

	bufferInit.desc.uByteWidth = sizeof(MESH_P)*6;
	bufferInit.data.pInitData = p;

	initVertexBuffer(device, bufferInit, this->iNrOfBuffers);
}

void GeometryManager::initParticleBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit)
{
	bufferInit.desc.uByteWidth = sizeof(MESH_PNC) * 1000;
	this->vVertexBuffer.at(this->iNrOfBuffers +1 ) =  this->pBufferObj->initInstance(device, bufferInit);
}
