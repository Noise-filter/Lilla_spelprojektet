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
		
		BaseParticle *emitter;
		List<BaseParticle> particles;

		virtual void emitt() = 0;
		virtual void createVertices() = 0;

	public:
		virtual void update(float dt) = 0;

		virtual ~ParticlePolicy(){};
		
		virtual vector<VertexColor> getVertexData() = 0;
};

#endif