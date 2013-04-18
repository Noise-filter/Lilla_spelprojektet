#ifndef BUFFER__H
#define BUFFER__H

#include "D3DGraphicUtility.h"
#include "D3DMathUtility.h"
#include "WindowsUtility.h"

class Buffer
{
	private:
		ID3D11Buffer *pBuffer;
		
	public:
		Buffer();
		virtual ~Buffer();

		ID3D11Buffer *initBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit);

};

#endif
