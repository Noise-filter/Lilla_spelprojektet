#ifndef GEOMETRYMANAGER_H
#define GEOMETRYMANAGER_H

#include "Buffer.h"
#include "OBJReader.h"
#include "GameObject.h"

class GeometryManager
{
	private:
		Buffer *pBufferObj;
		OBJReader *importer;
		std::vector<GameObject*> vEntities;
		GameObject* Particles;
		GameObject* FullScreenQuad;

		GameObject* hpBars;

	public:
		GeometryManager();
		virtual~GeometryManager();
		
		void init(ID3D11Device *device); //TBA more functionality like calls for importing meshes, also fill out the bufferInit for testing purpose
		void applyEntityBuffer(ID3D11DeviceContext *dc, int ID, D3D_PRIMITIVE_TOPOLOGY topology);
		void updateEntityBuffer(ID3D11DeviceContext *dc, std::vector<RenderData*> data, int ID);
		void updateParticles(ID3D11DeviceContext *dc, std::vector<std::vector<MESH_PNC>> data);
		void updateHPBars(ID3D11DeviceContext *dc, std::vector<HPBarInfo>& data);
		void applyQuadBuffer(ID3D11DeviceContext *dc , D3D_PRIMITIVE_TOPOLOGY topology);
		void applyParticleBuffer(ID3D11DeviceContext *dc , D3D_PRIMITIVE_TOPOLOGY topology);
		void applyHpBarBuffer(ID3D11DeviceContext *dc , D3D_PRIMITIVE_TOPOLOGY topology);

		int getNrOfInstances(int ID);
		int getNrOfVertexPoints(int ID);
		int getNrOfEntities();
		int getNrOfParticles();
		int getNrOfHPBars();

		//imports the mesh and init the buffer for it
		void importMesh(ID3D11Device *device,
						GameObject *object,
						string fileName,
						BUFFER_INIT &bufferInit,
						BUFFER_INIT &instanceInit,
						int nrOfInstances,
						Buffer* bufferObj);

};

#endif