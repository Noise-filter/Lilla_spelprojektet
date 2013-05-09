#include "GeometryManager.h"

GeometryManager::GeometryManager()
{
	this->pBufferObj = new Buffer();
	this->importer = NULL;
	
	this->vEntities.resize(NROFDIFFERENTMESHES);

	for(int i = 0; i < (int)this->vEntities.size(); i++)
	{
		this->vEntities[i] = new GameObject();
	}

	this->Particles = new GameObject();
	this->FullScreenQuad = new GameObject();

	hpBars = new GameObject();

}
GeometryManager::~GeometryManager()
{
	for(int i = 0; i < (int)this->vEntities.size(); i++)
	{
		SAFE_DELETE(this->vEntities[i]);
	}

	SAFE_DELETE(this->Particles);
	SAFE_DELETE(this->FullScreenQuad);
	SAFE_DELETE(hpBars);

	SAFE_DELETE(pBufferObj);
	SAFE_DELETE(this->importer);
}

void GeometryManager::init(ID3D11Device *device,int mapSize)
{
	importer = new OBJReader();
	BUFFER_INIT bufferInit		= BUFFER_INIT();
	BUFFER_INIT instanceInit	= BUFFER_INIT();
	BUFFER_INIT indexInit		= BUFFER_INIT();
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
	
	MESH_PNUV supply[] ={
		MESH_PNUV(Vec3(1.0,0,0), Vec3(1,0,0), Vec2(1,1)),
		MESH_PNUV(Vec3(-1.0,0,0), Vec3(0,0,1), Vec2(-1,0)),
		MESH_PNUV(Vec3(0,1.0,0), Vec3(0,1,0), Vec2(1,0)),

		MESH_PNUV(Vec3(1,0,0), Vec3(0,0,1), Vec2(0,0)),
		MESH_PNUV(Vec3(-1,0,0), Vec3(0,1,0), Vec2(0,-1)),
		MESH_PNUV(Vec3(0,-1.0,0), Vec3(1,0,0), Vec2(1,1)),
	};

	MESH_PNUV node[] = {

		MESH_PNUV(Vec3(0,0.5,0), Vec3(1,1,0), Vec2(0,0)),
		MESH_PNUV(Vec3(0,0,0.5), Vec3(1,0,1), Vec2(0,0)),
		MESH_PNUV(Vec3(0,-0.5,0), Vec3(1,0,0), Vec2(0,0)),
	};

	MESH_P quad[] = {  MESH_P(D3DXVECTOR3(1,-1,0)),
						MESH_P(D3DXVECTOR3(-1,-1,0)), 
						MESH_P(D3DXVECTOR3(1,1,0)),
						MESH_P(D3DXVECTOR3(1,1,0)),  
						MESH_P(D3DXVECTOR3(-1,-1,0)), 
						MESH_P(D3DXVECTOR3(-1,1,0)), 
	};


	//import and initilaze buffers
	string fileName = "Meshar/Main Building.obj";
	importMesh(device, this->vEntities.at(ENTITY_MAINBUILDING), fileName, bufferInit, instanceInit, 1, this->pBufferObj);

	fileName = "Meshar/Power Building part 1.obj";
	importMesh(device, this->vEntities.at(ENTITY_SUPPLYBASE), fileName, bufferInit, instanceInit, 100, this->pBufferObj);
	
	fileName = "Meshar/Power Building part 2.obj";
	importMesh(device, this->vEntities.at(ENTITY_SUPPLYTOP), fileName, bufferInit, instanceInit, 100, this->pBufferObj);

	fileName = "Meshar/Tower Part 1.obj";
	importMesh(device, this->vEntities.at(ENTITY_TOWERTOP), fileName, bufferInit, instanceInit, 100, this->pBufferObj);

	fileName = "Meshar/Tower Part 2.obj";
	importMesh(device, this->vEntities.at(ENTITY_TOWERBASE), fileName, bufferInit, instanceInit, 100, this->pBufferObj);
	
	fileName = "Meshar/Green node.obj";
	importMesh(device, this->vEntities.at(ENTITY_NODE_GREEN), fileName, bufferInit, instanceInit, 400, this->pBufferObj);

	fileName = "Meshar/Enemy (broken node).obj";
	importMesh(device, this->vEntities.at(ENTITY_NODE_RED), fileName, bufferInit, instanceInit, 400, this->pBufferObj);

	fileName = "Meshar/Enemy (broken node).obj";
	importMesh(device, this->vEntities.at(ENTITY_ENEMY), fileName, bufferInit, instanceInit, 400, this->pBufferObj);

	fileName = "Meshar/Very basic disc (projectile).obj";
	importMesh(device, this->vEntities.at(ENTITY_PROJECTILE), fileName, bufferInit, instanceInit, 400, this->pBufferObj);


	//temp buffer init
	
	this->vEntities.at(ENTITY_UPGRADE_HP)->mInit(device, bufferInit, instanceInit, supply, 6, 100, this->pBufferObj);
	this->vEntities.at(ENTITY_UPGRADE_ATKSP)->mInit(device, bufferInit, instanceInit, supply, 6, 100, this->pBufferObj);
	this->vEntities.at(ENTITY_UPGRADE_DMG)->mInit(device, bufferInit, instanceInit, supply, 6, 100, this->pBufferObj);
	this->vEntities.at(ENTITY_UPGRADE_PRJSP)->mInit(device, bufferInit, instanceInit, supply, 6, 100, this->pBufferObj);
	this->vEntities.at(ENTITY_UPGRADE_RANGE)->mInit(device, bufferInit, instanceInit, supply, 6, 100, this->pBufferObj);



	this->Particles->mInit(device, instanceInit, 1000, this->pBufferObj);

	MESH_P p[] = {  MESH_P(D3DXVECTOR3(1,-1,0)),
						MESH_P(D3DXVECTOR3(-1,-1,0)), 
						MESH_P(D3DXVECTOR3(1,1,0)),
						MESH_P(D3DXVECTOR3(1,1,0)),  
						MESH_P(D3DXVECTOR3(-1,-1,0)), 
						MESH_P(D3DXVECTOR3(-1,1,0)), 
	};

	this->FullScreenQuad->mInit(device, bufferInit, instanceInit, p, 6, 0 , this->pBufferObj);

	MESH_PUV puv[] = {  MESH_PUV(D3DXVECTOR3(1,-1,0), D3DXVECTOR2(0, 0)),
						MESH_PUV(D3DXVECTOR3(-1,-1,0), D3DXVECTOR2(0, 0)),
						MESH_PUV(D3DXVECTOR3(1,1,0), D3DXVECTOR2(0, 0)),
						MESH_PUV(D3DXVECTOR3(1,1,0), D3DXVECTOR2(0, 0)),
						MESH_PUV(D3DXVECTOR3(-1,-1,0), D3DXVECTOR2(0, 0)),
						MESH_PUV(D3DXVECTOR3(-1,1,0), D3DXVECTOR2(0, 0))
	};


	hpBars->mInit(device, bufferInit, instanceInit, puv, 6, 1000, pBufferObj, true);
}

void GeometryManager::applyEntityBuffer(ID3D11DeviceContext *dc, int ID, D3D_PRIMITIVE_TOPOLOGY topology)
{
	this->vEntities.at(ID)->mApply(dc, topology);
}
void GeometryManager::applyQuadBuffer(ID3D11DeviceContext *dc, D3D_PRIMITIVE_TOPOLOGY topology)
{
	UINT stride = sizeof(MESH_P);
	this->FullScreenQuad->mApply(dc, topology, stride);
}
void GeometryManager::applyParticleBuffer(ID3D11DeviceContext *dc , D3D_PRIMITIVE_TOPOLOGY topology)
{
	UINT stride = sizeof(MESH_PNC);
	this->Particles->mApply(dc, topology, stride);
}
void GeometryManager::applyHpBarBuffer(ID3D11DeviceContext *dc , D3D_PRIMITIVE_TOPOLOGY topology)
{
	UINT stride[2] = {sizeof(MESH_PUV), sizeof(MatrixInstance)};
	this->hpBars->mApply(dc, topology, stride);
}

void GeometryManager::updateEntityBuffer(ID3D11DeviceContext *dc, std::vector<RenderData*> data, int ID)
{
	this->vEntities.at(ID)->mUpdate(dc, data);
}
void GeometryManager::updateParticles(ID3D11DeviceContext *dc, std::vector<std::vector<MESH_PNC>> data)
{
	this->Particles->mUpdate(dc, data);
}
void GeometryManager::updateHPBars(ID3D11DeviceContext *dc, std::vector<HPBarInfo>& data)
{
	this->hpBars->mUpdate(dc, data);
}

int GeometryManager::getNrOfInstances(int ID)
{
	return vEntities.at(ID)->mGetNrOfInstances();
}
int GeometryManager::getNrOfVertexPoints(int ID)
{
	return this->vEntities.at(ID)->mGetNrOfVertices();
}
int GeometryManager::getNrOfEntities()
{
	return this->vEntities.size();
}
int GeometryManager::getNrOfParticles()
{
	return this->Particles->mGetNrOfVertices();
}
int GeometryManager::getNrOfHPBars()
{
	return this->hpBars->mGetNrOfInstances();
}

void GeometryManager::importMesh(ID3D11Device *device,
								 GameObject *object,
								 string fileName,
								 BUFFER_INIT &bufferInit,
								 BUFFER_INIT &instanceInit,
								 int nrOfInstances,
								 Buffer* bufferObj)
{
	int nrOfVertices = 0;
	std::vector<string> textures;
	std::vector<string> glowMaps;

	MESH_PNUV *temp = this->importer->getOBJfromFile(fileName, nrOfVertices, textures, glowMaps);

	object->mInit(device, bufferInit, instanceInit, temp, nrOfVertices, nrOfInstances, bufferObj);

	delete temp;
}
