#ifndef GEOMETRYMANAGER_H
#define GEOMETRYMANAGER_H

#include "Buffer.h"

class GeometryManager
{
	private:
		Buffer *pBufferObj;
		//Importer *importer, lägg till om/när vi får godkänt på
		//scriptprojektets förslaget
		std::vector<ID3D11Buffer*>	vIndexBuffer;
		std::vector<ID3D11Buffer*>	vVertexBuffer;
		std::vector<MESH_PNUV>		vMesh;

	public:
		GeometryManager();
		virtual~GeometryManager();
		//bool importMesh(); TBD
		
		void initVertexBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit);
		void initIndexBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit);

		void ApplyBuffer(ID3D11DeviceContext *dc, RENDERDATA obj, D3D_PRIMITIVE_TOPOLOGY topology, UINT32 misc = 0);
};

#endif