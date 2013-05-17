#include "GameObject.h"

GameObject::GameObject()
{
	this->pVertexBuffer   = NULL;
	this->pIndexBuffer    = NULL;
	this->pInstanceBuffer = NULL;
	this->texArray		  = NULL;
	this->glowArray		  = NULL;


	this->iNrOfinstances  = 0;
	this->iNrOfVertices   = 0;
}
GameObject::~GameObject()
{
	SAFE_RELEASE(this->pVertexBuffer);
	SAFE_RELEASE(this->pIndexBuffer);
	SAFE_RELEASE(this->pInstanceBuffer);
	//SAFE_RELEASE(this->texArray);
	//SAFE_RELEASE(this->glowArray);
}


///////////////////////////////////////////
//methods for updating content in buffers//
///////////////////////////////////////////
void GameObject::mUpdate(ID3D11DeviceContext *dc, std::vector<RenderData*> data)
{
	D3D11_MAPPED_SUBRESOURCE *mappedData = mMap(dc, this->pInstanceBuffer);

	INSTANCEDATA *instance = reinterpret_cast<INSTANCEDATA*>(mappedData->pData);

	for(int i = 0; i < (int)data.size(); i++)
	{
		instance[i].mWorld     = data[i]->worldMat;
		instance[i].iTextureID = data[i]->textureID;
	}

	this->iNrOfinstances = data.size();

	mUnmap(dc , this->pInstanceBuffer);
}
void GameObject::mUpdate(ID3D11DeviceContext *dc, std::vector<std::vector<MESH_PNC>> data)
{
	this->iNrOfVertices = 0;

	D3D11_MAPPED_SUBRESOURCE *mappedData = mMap(dc, this->pVertexBuffer);

	MESH_PNC *mesh = reinterpret_cast<MESH_PNC*>(mappedData->pData);

	for(int j = 0; j < (int)data.size(); j++)
	{
		for(int i = 0; i < (int)data[j].size(); i++)
		{
			mesh[i + this->iNrOfVertices].pos     = data[j][i].pos;
			mesh[i + this->iNrOfVertices].normal  = data[j][i].normal;
			mesh[i + this->iNrOfVertices].color   = data[j][i].color;
		}

		this->iNrOfVertices += data[j].size();
	}

	mUnmap(dc, this->pVertexBuffer);
}
void GameObject::mUpdate(ID3D11DeviceContext *dc , std::vector<HPBarInfo>& data)
{
	D3D11_MAPPED_SUBRESOURCE *mappedData = mMap(dc, this->pInstanceBuffer);

	MatrixInstance *mesh = reinterpret_cast<MatrixInstance*>(mappedData->pData);

	for(int j = 0; j < (int)data.size(); j++)
	{
		D3DXMatrixIdentity(&mesh[j].world);
		D3DXMatrixScaling(&mesh[j].world, data[j].hpPercent * 0.05f, 0.005f, 1.0f);
		mesh[j].world = mesh[j].world * data[j].translate;
	}
	this->iNrOfinstances = data.size();

	mUnmap(dc, this->pVertexBuffer);
}
void GameObject::mUpdate(ID3D11DeviceContext *dc, GUI_Panel* data, int nrOfInstances)
{
	D3D11_MAPPED_SUBRESOURCE *mappedData = mMap(dc, this->pInstanceBuffer);

	INSTANCEDATA *mesh = reinterpret_cast<INSTANCEDATA*>(mappedData->pData);

	for(int j = 0; j < (int)nrOfInstances; j++)
	{
		mesh[j].mWorld = data[j].matrix;
		mesh[j].iTextureID = data[j].textureID;
	}
	this->iNrOfinstances = nrOfInstances;

	mUnmap(dc, this->pVertexBuffer);
}

//////////////////////////////////////////
//methods for applying buffers to shader//
//////////////////////////////////////////
void GameObject::mApply(ID3D11DeviceContext *dc, D3D_PRIMITIVE_TOPOLOGY topology)
{
	ID3D11Buffer *buffers[2];

	UINT strides[2]  = {sizeof(MESH_PNUV) , sizeof(INSTANCEDATA)};
	UINT offset[2]   = {0 , 0};
	buffers[0]       = this->pVertexBuffer;
	buffers[1]       = this->pInstanceBuffer;

	dc->IASetVertexBuffers(0, 2, buffers , strides, offset);
	dc->IASetPrimitiveTopology(topology);
}
void GameObject::mApply(ID3D11DeviceContext *dc, D3D_PRIMITIVE_TOPOLOGY topology, UINT stride)
{
	UINT offset = 0;
	dc->IASetVertexBuffers(0 , 1 , &this->pVertexBuffer, &stride, &offset);
	dc->IASetPrimitiveTopology(topology);
}

void GameObject::mApply(ID3D11DeviceContext *dc, D3D_PRIMITIVE_TOPOLOGY topology, UINT strides[2])
{
	UINT offset[2] = {0, 0};
	ID3D11Buffer* buffers[2] = {pVertexBuffer, pInstanceBuffer};

	dc->IASetVertexBuffers(0, 2, buffers, strides, offset);
	dc->IASetPrimitiveTopology(topology);
}

////////////////////////////////////
//methods for initializing buffers//
////////////////////////////////////
void GameObject::mInit(ID3D11Device *device, BUFFER_INIT &bufferInit, BUFFER_INIT &instanceInit, MESH_P *mesh, int nrOfVertices, int nrOfInstances, Buffer* bufferObj)
{
	bufferInit.desc.uByteWidth    = sizeof(MESH_P) * nrOfVertices;
	bufferInit.data.pInitData     = mesh;

	if(nrOfInstances > 0)
	{
		instanceInit.desc.uByteWidth = sizeof(INSTANCEDATA) * nrOfInstances;
		this->pInstanceBuffer = bufferObj->initInstance(device, instanceInit);
	}
	
	this->pVertexBuffer = bufferObj->initBuffer(device, bufferInit);
	this->iNrOfVertices = nrOfVertices;

}
void GameObject::mInit(ID3D11Device *device, BUFFER_INIT &bufferInit, BUFFER_INIT &instanceInit, MESH_PUV *mesh, int nrOfVertices, int nrOfInstances,
					   Buffer* bufferObj)
{
	bufferInit.desc.uByteWidth    = sizeof(MESH_PUV) * nrOfVertices;
	bufferInit.data.pInitData     = mesh;

	if(nrOfInstances > 0)
	{
		instanceInit.desc.uByteWidth = sizeof(INSTANCEDATA) * nrOfInstances;
		this->pInstanceBuffer = bufferObj->initInstance(device, instanceInit);
	}

	this->pVertexBuffer = bufferObj->initBuffer(device, bufferInit);
	this->iNrOfVertices = nrOfVertices;
}

void GameObject::mInit(ID3D11Device *device, BUFFER_INIT &bufferInit, BUFFER_INIT &instanceInit, MESH_PUV *mesh, int nrOfVertices, int nrOfInstances,
					   Buffer* bufferObj, bool asd)
{
	bufferInit.desc.uByteWidth    = sizeof(MESH_PUV) * nrOfVertices;
	bufferInit.data.pInitData     = mesh;

	if(nrOfInstances > 0)
	{
		instanceInit.desc.uByteWidth = sizeof(MatrixInstance) * nrOfInstances;
		this->pInstanceBuffer = bufferObj->initInstance(device, instanceInit);
	}

	this->pVertexBuffer = bufferObj->initBuffer(device, bufferInit);
	this->iNrOfVertices = nrOfVertices;
}
void GameObject::mInit(ID3D11Device *device, BUFFER_INIT &bufferInit, BUFFER_INIT &instanceInit, MESH_PNUV *mesh, int nrOfVertices, int nrOfInstances,
					   Buffer* bufferObj, ID3D11ShaderResourceView *textures, ID3D11ShaderResourceView *glowMaps, float glowPower, float blurScalar)
{
	bufferInit.desc.uByteWidth    = sizeof(MESH_PNUV) * nrOfVertices;
	bufferInit.data.pInitData     = mesh;

	if(nrOfInstances > 0)
	{
		instanceInit.desc.uByteWidth = sizeof(INSTANCEDATA) * nrOfInstances;
		this->pInstanceBuffer = bufferObj->initInstance(device, instanceInit);
	}

	this->pVertexBuffer = bufferObj->initBuffer(device, bufferInit);
	this->iNrOfVertices = nrOfVertices;
	this->texArray = textures;
	this->glowArray = glowMaps;
	this->fGlowPower = glowPower;
	this->fBlurScalar = blurScalar;
}
void GameObject::mInit(ID3D11Device *device, BUFFER_INIT &bufferInit, int nrOfVertices, Buffer* bufferObj)
{
	bufferInit.desc.uByteWidth = sizeof(MESH_PNC) * nrOfVertices;
	this->pVertexBuffer = bufferObj->initInstance(device, bufferInit);
	this->iNrOfVertices = nrOfVertices;
}

///////////////////////
//Set and Get methods//
///////////////////////
int GameObject::mGetNrOfInstances() const
{
	return this->iNrOfinstances;
}
int GameObject::mGetNrOfVertices() const
{
	return this->iNrOfVertices;
}

float GameObject::getGlowPower() const
{
	return this->fGlowPower;
}

float GameObject::getBlurScalar() const
{
	return this->fBlurScalar;
}

ID3D11ShaderResourceView *GameObject::getTexArray() const
{
	return this->texArray;
}

ID3D11ShaderResourceView *GameObject::getGlowArray() const
{
	return this->glowArray;
}

void GameObject::mSetNrOfInstances(int value)
{
	this->iNrOfinstances = value;
}
void GameObject::mSetNrOfVertices(int value)
{
	this->iNrOfVertices = value;
}

void GameObject::setTexArray(ID3D11ShaderResourceView *tArray)
{
	this->texArray = tArray;
}

void GameObject::setGlowArray(ID3D11ShaderResourceView *gArray)
{
	this->glowArray = gArray;
}

//////////////////////////////////////////////////
//Map and Unmap methods, for updating the buffer//
//////////////////////////////////////////////////'
D3D11_MAPPED_SUBRESOURCE* GameObject::mMap(ID3D11DeviceContext *dc, ID3D11Buffer *buffer)
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
void GameObject::mUnmap(ID3D11DeviceContext *dc, ID3D11Buffer *buffer)
{
	dc->Unmap(buffer, 0);
}










