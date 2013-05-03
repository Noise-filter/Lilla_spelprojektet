#pragma once
#include <vector>
#include "Trail.h"
#include "DeathExplosion.h"
#include "AttackParticlePolicy.h"

using namespace std;
class ParticleSystem
{
public:
	static ParticleSystem* Getinstance();

	void shutdown();

	void update(float dt);
	vector<vector<VertexColor>> getVertexData();

	Trail* addTrail(D3DXVECTOR3 color, D3DXVECTOR3 position , int intensity, float timeToLive, float velocity, float lengthX, float lengthY, float lengthZ); //kompletera med hur ett cloud ska defineras
	void addDeathExplosion(D3DXVECTOR3 color, D3DXVECTOR3 position , int intensity, float timeToLive, float velocity);
	void addAttackParticlePolicy(D3DXVECTOR3 color, D3DXVECTOR3 position , int intensity, float timeToLive, float velocity, D3DXVECTOR3 direction);

	bool removePolicy(ParticlePolicy* temp); // kompletera med hur detta ska ske, hur removar man den policy man vill åt?(id? string namn?)

private:
	vector<ParticlePolicy*> particlePolicies;

	ParticleSystem();
	~ParticleSystem();
	static ParticleSystem* particleSystem;
};

