#pragma once
#include <vector>
#include "stdafx.h"
#include "Trail.h"


using namespace std;
class ParticleSystem
{
public:
	static ParticleSystem* Getinstance();

	void shutdown();

	void update();
	vector<vector<VertexColor>> getVertexData();

	Trail* addTrail(D3DXVECTOR3 color, D3DXVECTOR3 position , int intensity, int timeToLive, float velocity, float lengthX, float lengthY, float lengthZ); //kompletera med hur ett cloud ska defineras

	bool removePolicy(); // kompletera med hur detta ska ske, hur removar man den policy man vill åt?(id? string namn?)

private:
	vector<ParticlePolicy*> particlePolicies;
	
	ParticleSystem();
	~ParticleSystem();
	static ParticleSystem* particleSystem;
};

