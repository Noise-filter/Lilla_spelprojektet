#include "Buffer.h"

Buffer::Buffer()
{
	pBuffer			= NULL;
}

Buffer::~Buffer()
{
	SAFE_RELEASE(pBuffer);
}

//
//void* Buffer::Map()
//{
//	void* ret = NULL;
//	if(mUsage == BUFFER_CPU_WRITE || mUsage == BUFFER_CPU_READ || mUsage == BUFFER_CPU_WRITE_DISCARD)
//	{
//		D3D11_MAPPED_SUBRESOURCE MappedResource;
//		UINT32 mapType = 0;
//
//		if(mUsage == BUFFER_CPU_READ)	mapType = D3D11_MAP_READ;
//		else if(mUsage == BUFFER_CPU_WRITE) mapType = D3D11_MAP_WRITE;
//		else if(mUsage == BUFFER_CPU_WRITE_DISCARD)	mapType = D3D11_MAP_WRITE_DISCARD;
//
//		HRESULT hr = S_OK;
//		if(FAILED(hr = mImmediateContext->Map(
//			mBuffer,
//			0,
//			(D3D11_MAP)mapType,
//			0,
//			&MappedResource)))
//		{
//			ret = NULL;
//		}
//		else
//		{
//			ret = MappedResource.pData;
//		}
//	}
//
//	return ret;
//
//}
//
//void Buffer::Unmap()
//{
//	deviceContext->Unmap( mBuffer, 0 );
//}


ID3D11Buffer *Buffer::initBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;

	bufferDesc.ByteWidth			= bufferInit.desc.uByteWidth;
	bufferDesc.Usage				= bufferInit.desc.eUsage;
	bufferDesc.BindFlags			= bufferInit.desc.uBindFlags;
	bufferDesc.CPUAccessFlags		= bufferInit.desc.uCPUAccessFlags;
	bufferDesc.MiscFlags			= bufferInit.desc.uMiscFlags;
	bufferDesc.StructureByteStride	= bufferInit.desc.uStructureByteStride;

	//Padded so it's atleast 16byte wide
	if(bufferDesc.ByteWidth < 16)
		bufferDesc.ByteWidth = 16;

	D3D11_SUBRESOURCE_DATA subData;

	subData.pSysMem				= bufferInit.data.pInitData;
	subData.SysMemPitch			= bufferInit.data.uSysMemPitch;
	subData.SysMemSlicePitch	= bufferInit.data.uSysMemSlicePitch;

	hr = device->CreateBuffer(&bufferDesc, &subData, &pBuffer);

	if(FAILED(hr))
	{
		MessageBox(NULL, "Unable to create buffer.", "Slenda Error", MB_ICONERROR | MB_OK);
	}

	return pBuffer;
}