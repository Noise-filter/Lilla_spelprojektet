#include "ParticlePolicy.h"

ParticlePolicy::ParticlePolicy()
{
	this->nrOfVertsPerParticle = 1;
	this->intensity = 0;
	
	this->color = D3DXVECTOR3(1, 1, 1);
	this->emitter = new BaseParticle(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0), 0, 0);
	recreateVertices = true;
}

ParticlePolicy::ParticlePolicy(D3DXVECTOR3 color, D3DXVECTOR3 position, int intensity, float timeToLive, float velocity)
{
	this->nrOfVertsPerParticle = 1;
	this->intensity = intensity;

	this->color = color;
	this->emitter = new BaseParticle(position, D3DXVECTOR3(0,1,0), timeToLive, velocity);
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
	//if(recreateVertices)
	{
		this->vertices.clear();

		for(ListNode<BaseParticle>* walker = particles.getFirst(); walker != NULL; walker = walker->next)
		{
			MESH_PNC vert1;

			vert1.normal = D3DXVECTOR3(0,0,-1);
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

void ParticlePolicy::updatePosition(D3DXVECTOR3 pos)
{
	emitter->setPosition(pos);
}