#include "BaseParticle.h"


BaseParticle::BaseParticle(void)
{
}

BaseParticle::BaseParticle(D3DXVECTOR3 position, D3DXVECTOR3 direction, float timeToLive, float velocity)
{
	this->position = position;
	this->timeToLive = timeToLive;
	this->direction = direction;
	this->velocity = velocity;
}


BaseParticle::~BaseParticle(void)
{
}

D3DXVECTOR3 BaseParticle::getPosition()
{
	return this->position;
}

float BaseParticle::getTimeToLive()
{
	return this->timeToLive;
}

void BaseParticle::setPosition(D3DXVECTOR3 position)
{
	this->position = position;
}

void BaseParticle::setDirection(D3DXVECTOR3 direction)
{
	this->direction = direction;	
}

void BaseParticle::setTimeToLive(float timeToLive)
{
	this->timeToLive = timeToLive;
}



bool BaseParticle::update(float dt)
{
	//check if timeToLive is out and the particle is to be killed
	static float srand(time(NULL));

	float random = (float)rand()/(float)RAND_MAX  + 0.01f;

	if(timeToLive <= 0)
	{
		return false; //particle is dead
	}
	else
	{
		timeToLive -= dt; // decreasing particle lifetime
	}
	
	//position += direction * velocity; //new position calculated if the particle is to be alive
	return true; //all went good, do not kill the particle
}
