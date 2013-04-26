#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(void)
{
}


ParticleSystem::~ParticleSystem(void)
{
	for(int i = 0; i < this->particlePolicies.size(); i++)
	{
		delete particlePolicies.at(i);
	}
}

ParticleSystem* ParticleSystem::particleSystem = NULL;
ParticleSystem* ParticleSystem::Getinstance()
{
	if(particleSystem==NULL)
	{
		particleSystem = new ParticleSystem();
	}
	return particleSystem;
}

void ParticleSystem::update()
{
	//update all policies
	for(int i = 0; i < particlePolicies.size(); i++)
	{
		if(particlePolicies.at(i) == NULL)
		{
			particlePolicies.erase(particlePolicies.begin() + i);
			i--;
		}
		else
		{
			particlePolicies.at(i)->update();
		}
	}
}

vector<vector<VertexColor>> ParticleSystem::getVertexData()
{
	vector<vector<VertexColor>> vertexData;

	//fyll med all vertexdata fr�n particlePolicies

	return vertexData;
}


Trail* ParticleSystem::addTrail(D3DXVECTOR3 color, D3DXVECTOR3 position , int intensity, int timeToLive, float velocity, float lengthX, float lengthY, float lengthZ)
{
	Trail* temp = new Trail(color, position, intensity, timeToLive, velocity, lengthX, lengthY, lengthZ);
	particlePolicies.push_back(temp);

	return temp;
}

bool ParticleSystem::removePolicy() // kompletera med hur detta ska ske, hur removar man den policy man vill �t?(id? string namn?)
{
	return true;
}