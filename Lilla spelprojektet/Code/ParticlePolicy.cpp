#include "ParticlePolicy.h"

ParticlePolicy::ParticlePolicy()
{
	this->nrOfVertsPerParticle = 1;
	this->intensity = 0;
	
	this->color = Vec3(1, 1, 1);
	this->emitter = new BaseParticle(Vec3(0, 0, 0), Vec3(0, 1, 0), 0, 0);
	recreateVertices = true;
}

ParticlePolicy::ParticlePolicy(Vec3 color, Vec3 position, int intensity, float timeToLive, float velocity)
{
	this->nrOfVertsPerParticle = 1;
	this->intensity = intensity;

	this->color = color;
	this->emitter = new BaseParticle(position, Vec3(0,1,0), timeToLive, velocity);
	recreateVertices = true;
}

ParticlePolicy::~ParticlePolicy()
{
	SAFE_DELETE(emitter);
}

vector<MESH_PNC>& ParticlePolicy::getVertexData()
{
	return vertices;
}

void ParticlePolicy::createVertices()
{
	if(recreateVertices)
	{
		this->vertices.clear();

		for(ListNode<BaseParticle>* walker = particles.getFirst(); walker != NULL; walker = walker->next)
		{
			MESH_PNC vert1;

			vert1.normal = Vec3(1,1,1);
			vert1.pos = walker->value.getPosition();
			vert1.color = this->color;
		
			vertices.push_back(vert1);
		}
	}
	else
	{
		int i = 0;
		for(ListNode<BaseParticle>* walker = particles.getFirst(); walker != NULL; walker = walker->next, i++)
		{
			vertices.at(i).pos = walker->value.getPosition();
		}
	}
}

void ParticlePolicy::updatePosition(Vec3 pos)
{
	emitter->setPosition(pos);
}