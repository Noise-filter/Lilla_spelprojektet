#pragma once
#include <vector>
#include "Trail.h"


using namespace std;
class ParticleSystem
{
public:
	static ParticleSystem* Getinstance();

	void shutdown();

	void update(float dt);
	vector<vector<VertexColor>> getVertexData();

	Trail* addTrail(D3DXVECTOR3 color, D3DXVECTOR3 position , int intensity, float timeToLive, float velocity, float lengthX, float lengthY, float lengthZ); //kompletera med hur ett cloud ska defineras

	bool removePolicy(ParticlePolicy* temp); // kompletera med hur detta ska ske, hur removar man den policy man vill åt?(id? string namn?)

private:
	vector<ParticlePolicy*> particlePolicies;
	
	ParticleSystem();
	~ParticleSystem();
	static ParticleSystem* particleSystem;
};

