#ifndef GEOMETRYMANAGER_H
#define GEOMETRYMANAGER_H

#include "Buffer.h"
#include "OBJReader.h"
#include "GameObject.h"
#include "GUI.h"

class GeometryManager
{
	private:
		Buffer *pBufferObj;
		OBJReader *importer;
		std::vector<GameObject*> vEntities;
		std::vector<GameObject*> vLightTypes;
		GameObject* Particles;
		GameObject* FullScreenQuad;

		GameObject* hpBars;
		GameObject* GUI;
		ID3D11ShaderResourceView *texGUI;

	private:
		ID3D11ShaderResourceView *createTextureArray(ID3D11Device *device,
													 ID3D11DeviceContext *dc,
													 std::vector<std::string> &filenames,
													 DXGI_FORMAT format,
													 UINT filter,
													 UINT mipFilter);

	public:
		GeometryManager();
		virtual~GeometryManager();
		

		void init(ID3D11Device *device, ID3D11DeviceContext *dc); //TBA more functionality like calls for importing meshes, also fill out the bufferInit for testing purpose
		void initMeshes(ID3D11Device *device, ID3D11DeviceContext *dc, int mapSize);
		void updateEntityBuffer(ID3D11DeviceContext *dc, std::vector<std::vector<RenderData*>> data);
		void updateParticles(ID3D11DeviceContext *dc, std::vector<std::vector<MESH_PNC>> data);
		void updateHPBars(ID3D11DeviceContext *dc, std::vector<HPBarInfo>& data);

		void updateGUI(ID3D11DeviceContext *dc, GUI_Panel* data, int nrOfObjects);

		void applyLightBuffer(ID3D11DeviceContext *dc, D3D_PRIMITIVE_TOPOLOGY topology);
		void applyEntityBuffer(ID3D11DeviceContext *dc, int ID, D3D_PRIMITIVE_TOPOLOGY topology);
		void applyQuadBuffer(ID3D11DeviceContext *dc , D3D_PRIMITIVE_TOPOLOGY topology);
		void applyParticleBuffer(ID3D11DeviceContext *dc , D3D_PRIMITIVE_TOPOLOGY topology);
		void applyHpBarBuffer(ID3D11DeviceContext *dc , D3D_PRIMITIVE_TOPOLOGY topology);

		void applyGUIBuffer(ID3D11DeviceContext *dc, D3D_PRIMITIVE_TOPOLOGY topology);

		int getNrOfInstances(int ID);
		int getNrOfVertexPoints(int ID);
		int getNrOfEntities();
		int getNrOfParticles();
		int getNrOfHPBars();
		int getNrOfGUIObjects();

		ID3D11ShaderResourceView *getTextures(int ID);
		ID3D11ShaderResourceView *getGlowMaps(int ID);
		ID3D11ShaderResourceView *getGUIPanels();

		//imports the mesh and init the buffer for it
		void importMesh(ID3D11Device *device,
						ID3D11DeviceContext *dc,
						GameObject *object,
						string fileName,
						BUFFER_INIT &bufferInit,
						BUFFER_INIT &instanceInit,
						int nrOfInstances,
						Buffer* bufferObj,
						vector<string> tex,
						vector<string> glow);

};

#endif