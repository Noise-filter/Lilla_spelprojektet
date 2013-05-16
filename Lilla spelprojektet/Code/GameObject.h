#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "D3DGraphicUtility.h"
#include "WindowsUtility.h"
#include "Buffer.h"
#include "GUI.h"

class GameObject
{
	private:
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
		ID3D11Buffer* pInstanceBuffer;

		ID3D11ShaderResourceView *texArray;
		ID3D11ShaderResourceView *glowArray;
		int iNrOfTextures;

		int iNrOfinstances;
		int iNrOfVertices;

	public:
		GameObject();
		~GameObject();

		void mUpdate(ID3D11DeviceContext *dc ,  std::vector<RenderData*> data);
		void mUpdate(ID3D11DeviceContext *dc ,  std::vector<std::vector<RenderData*>> data); //till varje ljus som tillhör en entitet
		void mUpdate(ID3D11DeviceContext *dc , std::vector<std::vector<MESH_PNC>> data);
		void mUpdate(ID3D11DeviceContext *dc , std::vector<HPBarInfo>& data);
		void mUpdate(ID3D11DeviceContext *dc, GUI_Panel* data, int nrOfInstances);	//Används till GUI

		void mApply(ID3D11DeviceContext *dc, D3D_PRIMITIVE_TOPOLOGY topology, UINT stride);
		void mApply(ID3D11DeviceContext *dc, D3D_PRIMITIVE_TOPOLOGY topology, UINT strides[2]);

		void mInit(ID3D11Device *device, BUFFER_INIT &bufferInit, BUFFER_INIT &instanceInit, MESH_P    *mesh, int nrOfVertices, int nrOfInstances, Buffer* bufferObj);
		void mInit(ID3D11Device *device, BUFFER_INIT &bufferInit, BUFFER_INIT &instanceInit, MESH_PUV  *mesh, int nrOfVertices, int nrOfInstances, Buffer* bufferObj , UINT byteWidth[2]);
		void mInit(ID3D11Device *device, BUFFER_INIT &bufferInit, BUFFER_INIT &instanceInit, MESH_PNUV *mesh, int nrOfVertices, int nrOfInstances, Buffer* bufferObj, ID3D11ShaderResourceView *textures, ID3D11ShaderResourceView *glowMaps);
		void mInit(ID3D11Device *device, BUFFER_INIT &bufferInit, int nrOfVertices, Buffer* bufferObj);

		int mGetNrOfInstances() const;
		int mGetNrOfVertices() const;
		ID3D11ShaderResourceView *getTexArray() const;
		ID3D11ShaderResourceView *getGlowArray() const;
		
		void mSetNrOfInstances(int value);
		void mSetNrOfVertices(int value);
		void setTexArray(ID3D11ShaderResourceView *tArray);
		void setGlowArray(ID3D11ShaderResourceView *gArray);

		D3D11_MAPPED_SUBRESOURCE* mMap(ID3D11DeviceContext *dc, ID3D11Buffer *buffer);
		void mUnmap(ID3D11DeviceContext *dc, ID3D11Buffer *buffer);
		
};
#endif