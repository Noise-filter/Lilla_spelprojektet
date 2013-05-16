#pragma once
#include <ctime>
#include "D3DMathUtility.h"

class BaseParticle
{
public:
	BaseParticle(D3DXVECTOR3 position, D3DXVECTOR3 direction, float timeToLive, float velocity);
	BaseParticle(void);
	~BaseParticle(void);

	bool update(float dt);

	D3DXVECTOR3 getPosition();
	float getTimeToLive();

	void setPosition(D3DXVECTOR3 position);
	void setDirection(D3DXVECTOR3 direction);
	void setTimeToLive(float timeToLive);

private:
	D3DXVECTOR3 position; //position of emitter or of real particle
	D3DXVECTOR3 direction; //  if emitter then this is the general direction that particles are fired in, if particle then this is the actual direction
	float velocity;
	float timeToLive; // this is the number of frames that the particle is to be alive
};

