#ifndef ATTACK_PARTICLE_POLICY_H
#define ATTACK_PARTICLE_POLICY_H

#include "ParticlePolicy.h"

class AttackParticlePolicy : public ParticlePolicy
{
public:
	AttackParticlePolicy();
	AttackParticlePolicy(D3DXVECTOR3 color, D3DXVECTOR3 position, int intensity, float timeToLive, float velocity, D3DXVECTOR3 direction);
	~AttackParticlePolicy();

	virtual int update(float dt);

	virtual void emitt();

private:
	D3DXVECTOR3 direction;

};


#endif