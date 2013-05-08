#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "D3DGraphicUtility.h"
#include "WindowsUtility.h"
#include "Buffer.h"

class GameObject
{
	private:
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
		ID3D11Buffer* pInstanceBuffer;

		ID3D11Texture2D **pTextures;
		int iNrOfTextures;

		int iNrOfinstances;
		int iNrOfVertices;

	public:
		GameObject();
		~GameObject();

		void mUpdate(ID3D11DeviceContext *dc ,  std::vector<RenderData*> data);
		void mUpdate(ID3D11DeviceContext *dc , std::vector<std::vector<MESH_PNC>> data);

		void mApply(ID3D11DeviceContext *dc, D3D_PRIMITIVE_TOPOLOGY topology);
		void mApply(ID3D11DeviceContext *dc, D3D_PRIMITIVE_TOPOLOGY topology, UINT stride);

		void mInit(ID3D11Device *device, BUFFER_INIT &bufferInit, BUFFER_INIT &instanceInit, MESH_P    *mesh, int nrOfVertices, int nrOfInstances, Buffer* bufferObj);
		void mInit(ID3D11Device *device, BUFFER_INIT &bufferInit, BUFFER_INIT &instanceInit, MESH_PUV  *mesh, int nrOfVertices, int nrOfInstances, Buffer* bufferObj);
		void mInit(ID3D11Device *device, BUFFER_INIT &bufferInit, BUFFER_INIT &instanceInit, MESH_PNUV *mesh, int nrOfVertices, int nrOfInstances, Buffer* bufferObj);
		void mInit(ID3D11Device *device, BUFFER_INIT &bufferInit, int nrOfVertices, Buffer* bufferObj);

		int mGetNrOfInstances();
		int mGetNrOfVertices();
		
		void mSetNrOfInstances(int value);
		void mSetNrOfVertices(int value);

		D3D11_MAPPED_SUBRESOURCE* mMap(ID3D11DeviceContext *dc, ID3D11Buffer *buffer);
		void mUnmap(ID3D11DeviceContext *dc, ID3D11Buffer *buffer);
		
};
#endif