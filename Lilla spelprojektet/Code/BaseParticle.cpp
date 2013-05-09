#include "BaseParticle.h"


BaseParticle::BaseParticle(void)
{
}

BaseParticle::BaseParticle(Vec3 position, Vec3 direction, float timeToLive, float velocity)
{
	this->position = position;
	this->timeToLive = timeToLive;
	this->direction = direction;
	this->velocity = velocity;
}


BaseParticle::~BaseParticle(void)
{
}

Vec3 BaseParticle::getPosition()
{
	return this->position;
}

float BaseParticle::getTimeToLive()
{
	return this->timeToLive;
}

void BaseParticle::setPosition(Vec3 position)
{
	this->position = position;
}

void BaseParticle::setDirection(Vec3 direction)
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

	//float random = (float)rand()/(float)RAND_MAX  + 0.01f;

	if(timeToLive <= 0)
	{
		return false; //particle is dead
	}
	else
	{
		timeToLive -= dt; // decreasing particle lifetime
	}
	
	position += (direction * velocity * dt); //new position calculated if the particle is to be alive
	return true; //all went good, do not kill the particle
}
