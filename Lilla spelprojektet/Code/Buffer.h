#ifndef BUFFER__H
#define BUFFER__H

#include "D3DGraphicUtility.h"
#include "D3DMathUtility.h"
#include "WindowsUtility.h"



class Buffer
{
	private:
		HRESULT initBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit);

	private:
		ID3D11Buffer		*pBuffer;
		ID3D11Buffer		*pIndexBuffer;

		std::vector<ID3D11Buffer*> vIndexBuffer;
		std::vector<ID3D11Buffer*> vVertexBuffer;
		
	public:

		Buffer();
		virtual ~Buffer();

		HRESULT initVertexBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit);
		HRESULT initIndexBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit);

		//void* Map();
		//void Unmap();

		void Apply(ID3D11DeviceContext *dc, RENDERDATA obj, D3D_PRIMITIVE_TOPOLOGY topology, UINT32 misc = 0);

};

#endif
