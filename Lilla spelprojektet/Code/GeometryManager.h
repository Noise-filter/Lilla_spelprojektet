#ifndef GEOMETRYMANAGER_H
#define GEOMETRYMANAGER_H

#include "Buffer.h"

class GeometryManager
{
	private:
		void initVertexBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit);
		void initIndexBuffer(ID3D11Device *device, BUFFER_INIT &bufferInit);
		void initInstance(ID3D11Device *device, BUFFER_INIT &bufferInit);
		D3D11_MAPPED_SUBRESOURCE *map(ID3D11DeviceContext *dc, ID3D11Buffer *buffer);
		void unmap(ID3D11DeviceContext *dc, ID3D11Buffer *buffer);



	private:
		Buffer *pBufferObj;
		//Importer *importer, lägg till om/när vi får godkänt på
		//scriptprojektets förslaget
		std::vector<ID3D11Buffer*>	vIndexBuffer;
		std::vector<ID3D11Buffer*>	vVertexBuffer;
		std::vector<ID3D11Buffer*>	vInstanceBuffer;
		std::vector<MESH_PNUV*>		vMesh;

		//Used to send in the appropriate vertex and instance buffer to the Input Assembler
		ID3D11Buffer *buffers[2];

	public:
		GeometryManager();
		virtual~GeometryManager();
		//bool importMesh(); TBD
		
		void init(ID3D11Device *device); //TBA more functionality like calls for importing meshes, also fill out the bufferInit for testing purpose
		void applyBuffer(ID3D11DeviceContext *dc, std::vector<RENDERDATA*> obj, D3D_PRIMITIVE_TOPOLOGY topology, UINT32 misc = 0);
		void updateBuffer(ID3D11DeviceContext *dc, std::vector<RENDERDATA*> data, int index);
		void applyQuadBuffer(ID3D11DeviceContext *dc, int ID);

		int getNrOfInstances(int index);
		int getNrOfVertexPoints(int index);
};

#endif