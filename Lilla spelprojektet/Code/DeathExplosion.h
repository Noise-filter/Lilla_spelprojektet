#ifndef DEATH_EXPLOSION_H
#define DEATH_EXPLOSION_H

#include "ParticlePolicy.h"

class DeathExplosion : public ParticlePolicy
{
public:
	DeathExplosion();
	DeathExplosion(D3DXVECTOR3 color, D3DXVECTOR3 position, int intensity, float timeToLive, float velocity);
	~DeathExplosion();

	virtual int update(float dt);

	virtual void emitt();

private:
	

};

#endif