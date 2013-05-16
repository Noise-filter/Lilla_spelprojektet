#include "GeometryManager.h"
#include "TexAndGlow.h"

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
	GUI = new GameObject();
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

void GeometryManager::init(ID3D11Device *device, ID3D11DeviceContext *dc, int mapSize)
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

	MESH_PNUV plane[] ={
		MESH_PNUV(Vec3(0.5f,0,-0.5f), Vec3(0,1,0), Vec2((float)mapSize,0)),
		MESH_PNUV(Vec3(-0.5f,0,-0.5f), Vec3(0,1,0), Vec2(0,0)),
		MESH_PNUV(Vec3(0.5f,0,0.5f), Vec3(0,1,0), Vec2((float)mapSize,(float)mapSize)),

		MESH_PNUV(Vec3(0.5f,0,0.5f), Vec3(0,1,0), Vec2((float)mapSize,(float)mapSize)),
		MESH_PNUV(Vec3(-0.5f,0,-0.5f), Vec3(0,1,0), Vec2(0,0)),
		MESH_PNUV(Vec3(-0.5f,0,0.5f), Vec3(0,1,0), Vec2(0,(float)mapSize)),
	};

	//import and initilaze buffers
	vector<string> tex, glow;
	string fileName = "Meshar/Main Building.obj";
	tex = TexAndGlow::loadMainBuildingTex();
	glow = TexAndGlow::loadMainBuildingGlow();
	importMesh(device, dc, this->vEntities.at(ENTITY_MAINBUILDING), fileName, bufferInit, instanceInit, 1, this->pBufferObj, tex, glow);

	fileName = "Meshar/Power Building part 1.obj";
	tex = TexAndGlow::loadSupplyLowerTex();
	glow = TexAndGlow::loadSupplyLowerGlow();
	importMesh(device, dc, this->vEntities.at(ENTITY_SUPPLYBASE), fileName, bufferInit, instanceInit, 100, this->pBufferObj, tex, glow);
	
	fileName = "Meshar/Power Building part 2.obj";
	tex = TexAndGlow::loadSupplyUpperTex();
	glow = TexAndGlow::loadSupplyUpperGlow();
	importMesh(device, dc, this->vEntities.at(ENTITY_SUPPLYTOP), fileName, bufferInit, instanceInit, 100, this->pBufferObj, tex, glow);

	fileName = "Meshar/Tower Part 1.obj";
	tex = TexAndGlow::loadTowerLowerTex();
	glow = TexAndGlow::loadTowerLowerGlow();
	importMesh(device, dc, this->vEntities.at(ENTITY_TOWERTOP), fileName, bufferInit, instanceInit, 100, this->pBufferObj, tex, glow);

	fileName = "Meshar/Tower Part 2.obj";
	tex = TexAndGlow::loadTowerUpperTex();
	glow = TexAndGlow::loadTowerUpperGlow();
	importMesh(device, dc, this->vEntities.at(ENTITY_TOWERBASE), fileName, bufferInit, instanceInit, 100, this->pBufferObj, tex, glow);
	
	fileName = "Meshar/Green node.obj";
	tex = TexAndGlow::loadFriendlyNodeTex();
	glow = TexAndGlow::loadFriendlyNodeGlow();
	importMesh(device, dc, this->vEntities.at(ENTITY_NODE_GREEN), fileName, bufferInit, instanceInit, 400, this->pBufferObj, tex, glow);

	fileName = "Meshar/Enemy (broken node).obj";
	tex = TexAndGlow::loadHostileNodeTex();
	glow = TexAndGlow::loadHostileNodeGlow();
	importMesh(device, dc, this->vEntities.at(ENTITY_NODE_RED), fileName, bufferInit, instanceInit, 400, this->pBufferObj, tex, glow);

	fileName = "Meshar/Enemy (broken node).obj";
	tex = TexAndGlow::loadHostileNodeTex();
	glow = TexAndGlow::loadHostileNodeGlow();
	importMesh(device, dc, this->vEntities.at(ENTITY_ENEMY), fileName, bufferInit, instanceInit, 400, this->pBufferObj, tex, glow);

	fileName = "Meshar/Very basic disc (projectile).obj";
	tex = TexAndGlow::loadHostileNodeTex();
	glow = TexAndGlow::loadHostileNodeGlow();
	importMesh(device, dc, this->vEntities.at(ENTITY_PROJECTILE), fileName, bufferInit, instanceInit, 400, this->pBufferObj, tex, glow);


	//temp buffer init
	ID3D11ShaderResourceView *nulls = NULL;
	this->vEntities.at(ENTITY_UPGRADE_OFFENSE)->mInit(device, bufferInit, instanceInit, supply, 6, 100, this->pBufferObj, nulls, nulls);
	this->vEntities.at(ENTITY_UPGRADE_DEFENSE)->mInit(device, bufferInit, instanceInit, supply, 6, 100, this->pBufferObj, nulls, nulls);
	this->vEntities.at(ENTITY_UPGRADE_RES)->mInit(device, bufferInit, instanceInit, supply, 6, 100, this->pBufferObj, nulls, nulls);
	this->vEntities.at(ENTITY_PLANE)->mInit(device, bufferInit, instanceInit, plane, 6, 1, this->pBufferObj, nulls, nulls);

	this->Particles->mInit(device, instanceInit, 1000, this->pBufferObj);

	MESH_P p[] = {  MESH_P(D3DXVECTOR3(1,-1,0)),
						MESH_P(D3DXVECTOR3(-1,-1,0)), 
						MESH_P(D3DXVECTOR3(1,1,0)),
						MESH_P(D3DXVECTOR3(1,1,0)),  
						MESH_P(D3DXVECTOR3(-1,-1,0)), 
						MESH_P(D3DXVECTOR3(-1,1,0)), 
	};

	this->FullScreenQuad->mInit(device, bufferInit, instanceInit, p, 6, 0 , this->pBufferObj);

	MESH_PUV puv[] = {  MESH_PUV(D3DXVECTOR3(1,-1,0), D3DXVECTOR2(1, 1)),
						MESH_PUV(D3DXVECTOR3(-1,-1,0), D3DXVECTOR2(0, 1)),
						MESH_PUV(D3DXVECTOR3(1,1,0), D3DXVECTOR2(1, 0)),
						MESH_PUV(D3DXVECTOR3(1,1,0), D3DXVECTOR2(1, 0)),
						MESH_PUV(D3DXVECTOR3(-1,-1,0), D3DXVECTOR2(0, 1)),
						MESH_PUV(D3DXVECTOR3(-1,1,0), D3DXVECTOR2(0, 0))
	};


	hpBars->mInit(device, bufferInit, instanceInit, puv, 6, 1000, pBufferObj, true);

	GUI->mInit(device, bufferInit, instanceInit, puv, 6, 1000, pBufferObj);
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
void GeometryManager::applyGUIBuffer(ID3D11DeviceContext *dc, D3D_PRIMITIVE_TOPOLOGY topology)
{
	UINT stride[2] = {sizeof(MESH_PUV), sizeof(INSTANCEDATA)};
	this->GUI->mApply(dc, topology, stride);
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
void GeometryManager::updateGUI(ID3D11DeviceContext *dc, GUI_Panel* data, int nrOfObjects)
{
	GUI->mUpdate(dc, data, nrOfObjects);
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
int GeometryManager::getNrOfGUIObjects()
{
	return this->GUI->mGetNrOfInstances();
}

ID3D11ShaderResourceView *GeometryManager::getTextures(int ID)
{
	return this->vEntities[ID]->getTexArray();
}

ID3D11ShaderResourceView *GeometryManager::getGlowMaps(int ID)
{
	return this->vEntities[ID]->getGlowArray();
}

void GeometryManager::importMesh(ID3D11Device *device,
								 ID3D11DeviceContext *dc,
								 GameObject *object,
								 string fileName,
								 BUFFER_INIT &bufferInit,
								 BUFFER_INIT &instanceInit,
								 int nrOfInstances,
								 Buffer* bufferObj,
								 vector<string> tex,
								 vector<string> glow)
{
	int nrOfVertices = 0;
	ID3D11ShaderResourceView *texTemp = NULL;
	ID3D11ShaderResourceView *glowTemp = NULL;

	MESH_PNUV *temp = this->importer->getOBJfromFile(fileName, nrOfVertices, tex, glow);
	
	if(glow.size() > 0)	glowTemp = createTextureArray(device, dc, glow, DXGI_FORMAT_R8G8B8A8_UNORM, D3DX11_FILTER_NONE, D3DX11_FILTER_NONE);
	if(tex.size() > 0) texTemp = createTextureArray(device, dc, tex, DXGI_FORMAT_R8G8B8A8_UNORM, D3DX11_FILTER_NONE, D3DX11_FILTER_NONE);
	object->mInit(device, bufferInit, instanceInit, temp, nrOfVertices, nrOfInstances, bufferObj, texTemp, glowTemp);

	delete temp;
}


/*###############################
### Private helper functions. ###
###############################*/
ID3D11ShaderResourceView *GeometryManager::createTextureArray(ID3D11Device *device, ID3D11DeviceContext *dc, std::vector<std::string> &filenames, DXGI_FORMAT format, UINT filter, UINT mipFilter)
{
	HRESULT hr = S_OK;
	/*####################################################################################
	### Load the textures individually from file, GPU won't use them (bind flags = 0), ###
	### they're only used to load the image data from file.							   ###
	### STAGING used so the CPU can read the resource.								   ###
	####################################################################################*/
	UINT size = filenames.size();
	std::vector<ID3D11Texture2D*> srcTex(size);

	D3DX11_IMAGE_LOAD_INFO loadInfo;
	loadInfo.Width			= D3DX11_FROM_FILE;
	loadInfo.Height			= D3DX11_FROM_FILE;
	loadInfo.Depth			= D3DX11_FROM_FILE;
	loadInfo.FirstMipLevel	= 0;
	loadInfo.MipLevels		= D3DX11_FROM_FILE;
	loadInfo.Usage			= D3D11_USAGE_STAGING;
	loadInfo.BindFlags		= 0;
	loadInfo.CpuAccessFlags	= D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	loadInfo.MiscFlags		= 0;
	loadInfo.Format			= format;
	loadInfo.Filter			= filter;
	loadInfo.MipFilter		= mipFilter;
	loadInfo.pSrcInfo		= 0;

	for(UINT i = 0; i < size; i++)
	{
		hr = D3DX11CreateTextureFromFile(device, filenames[i].c_str(), &loadInfo, 0, (ID3D11Resource**)&srcTex[i], 0);

		if(FAILED(hr))
		{
			MessageBox(NULL, "Failed to load texture from file", filenames[i].c_str(), MB_ICONERROR | MB_OK);
		}
	}

	/*############################################################################
	### Create the texture array, each element got the same format/dimensions. ###
	############################################################################*/
	D3D11_TEXTURE2D_DESC texElementDesc;
	srcTex[0]->GetDesc(&texElementDesc);

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width				= texElementDesc.Width;
	texArrayDesc.Height				= texElementDesc.Height;
	texArrayDesc.MipLevels			= texElementDesc.MipLevels;
	texArrayDesc.ArraySize			= size;
	texArrayDesc.Format				= texElementDesc.Format;
	texArrayDesc.SampleDesc.Count	= 1;
	texArrayDesc.SampleDesc.Quality	= 0;;
	texArrayDesc.Usage				= D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags		= 0;
	texArrayDesc.MiscFlags			= 0;

	ID3D11Texture2D *texArray = NULL;
	hr = device->CreateTexture2D(&texArrayDesc, NULL, &texArray);

	if(FAILED(hr))
	{
		MessageBox(NULL, "Failed to create texture array", "Not good :(", MB_ICONERROR | MB_OK);
	}

	/*##############################################################
	### Copy individual texture elements into the texture array. ###
	##############################################################*/
	D3D11_MAPPED_SUBRESOURCE mappedTex2D;

	for(UINT texElement = 0; texElement < size; texElement++)
	{
		for(UINT mipLevel = 0; mipLevel < texElementDesc.MipLevels; mipLevel++)
		{
			hr = dc->Map(srcTex[texElement], mipLevel, D3D11_MAP_READ, 0, &mappedTex2D);
			
			if(FAILED(hr))
			{
				MessageBox(NULL, "Map failed", "Not good :(", MB_ICONERROR | MB_OK);
			}

			dc->UpdateSubresource(texArray, D3D11CalcSubresource(mipLevel, texElement, texElementDesc.MipLevels),
								  0, mappedTex2D.pData, mappedTex2D.RowPitch, mappedTex2D.DepthPitch);

			dc->Unmap(srcTex[texElement], mipLevel);
		}
	}

	/*##################################################
	### Create a resource view to the texture array. ###
	##################################################*/
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format							= texArrayDesc.Format;
	srvDesc.ViewDimension					= D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MostDetailedMip	= 0;
	srvDesc.Texture2DArray.MipLevels		= texArrayDesc.MipLevels;
	srvDesc.Texture2DArray.FirstArraySlice	= 0;
	srvDesc.Texture2DArray.ArraySize		= size;

	ID3D11ShaderResourceView *texArraySRV = NULL;
	hr = device->CreateShaderResourceView(texArray, &srvDesc, &texArraySRV);
	if(FAILED(hr))
	{
		MessageBox(NULL, "Failed to create the Shader Resource View array", "Not good :(", MB_ICONERROR | MB_OK);
	}

	/*####################################################
	### Releasing everything except the resource view, ### 
	### since it's the only thing we need.             ###
	####################################################*/
	SAFE_RELEASE(texArray);

	for(UINT i = 0; i < size; i++)
	{
		SAFE_RELEASE(srcTex[i]);
	}

	return texArraySRV;
}


