#ifndef PARTICLEPOLICY_H
#define PARTICLEPOLICY_H

#include "Buffer.h"
#include "Shader.h"
#include "BaseParticle.h"
#include "List.h"
#include <vector>
using namespace std;

class ParticlePolicy
{
	protected:
		int nrOfVertsPerParticle;
		int intensity;
		D3DXVECTOR3 color;
		
		BaseParticle *emitter;
		List<BaseParticle> particles;
		std::vector<VertexColor> vertices;

		bool recreateVertices;

		virtual void emitt() = 0;
		virtual void createVertices();

	public:
		ParticlePolicy();
		ParticlePolicy(D3DXVECTOR3 color, D3DXVECTOR3 position, int intensity, float timeToLive, float velocity);

		virtual int update(float dt) = 0;

		virtual ~ParticlePolicy();

		virtual vector<VertexColor>& getVertexData();
		virtual void updatePosition(D3DXVECTOR3 pos);
};

#endif