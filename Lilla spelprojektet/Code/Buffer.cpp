#include "Buffer.h"

Buffer::Buffer()
{
	pBuffer	= NULL;
}

Buffer::~Buffer()
{
	pBuffer = NULL;
	//SAFE_RELEASE(pBuffer);
}

ID3D11Buffer *Buffer::initBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;
	SecureZeroMemory(&bufferDesc, sizeof(bufferDesc));
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
	SecureZeroMemory(&subData, sizeof(subData));
	subData.pSysMem				= bufferInit.data.pInitData;
	subData.SysMemPitch			= bufferInit.data.uSysMemPitch;
	subData.SysMemSlicePitch	= bufferInit.data.uSysMemSlicePitch;

	hr = device->CreateBuffer(&bufferDesc, &subData, &pBuffer);

	if(FAILED(hr))
	{
		MessageBox(NULL, "Unable to create vertex-/instancebuffer.", "Slenda Error", MB_ICONERROR | MB_OK);
	}

	return pBuffer;
}

ID3D11Buffer *Buffer::initInstance(ID3D11Device *device, BUFFER_INIT &bufferInit)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bufferDesc;
	SecureZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth			= bufferInit.desc.uByteWidth;
	bufferDesc.Usage				= bufferInit.desc.eUsage;
	bufferDesc.BindFlags			= bufferInit.desc.uBindFlags;
	bufferDesc.CPUAccessFlags		= bufferInit.desc.uCPUAccessFlags;
	bufferDesc.MiscFlags			= bufferInit.desc.uMiscFlags;
	bufferDesc.StructureByteStride	= bufferInit.desc.uStructureByteStride;

	//Padded so it's atleast 16byte wide
	if(bufferDesc.ByteWidth < 16)
		bufferDesc.ByteWidth = 16;

	hr = device->CreateBuffer(&bufferDesc, NULL, &pBuffer);

	if(FAILED(hr))
	{
		MessageBox(NULL, "Unable to create instance.", "Slenda Error", MB_ICONERROR | MB_OK);
	}

	return pBuffer;
}