#include "Trail.h"

Trail::Trail(D3DXVECTOR3 color, D3DXVECTOR3 position, int intensity, float timeToLive, float velocity, float lengthX, float lengthY, float lengthZ)
	: ParticlePolicy(color, position, intensity, timeToLive, velocity)
{
	this->lengthX = lengthX;
	this->lengthY = lengthY;
	this->lengthZ = lengthZ;
}

Trail::~Trail()
{
}

int Trail::update(float dt)
{
	for(int i = 0; i < this->intensity * dt; i++)
	{
		emitt();
	}

	int i = 0;
	for(ListNode<BaseParticle>* walker = particles.getFirst(); walker != NULL; walker = walker->next)
	{
		if(!walker->value.update(dt))
		{
			i++;
		}
	}

	particles.remove(i);
	createVertices();

	return 1;
}

void Trail::emitt()
{
	BaseParticle Particle = *this->emitter;
	float halfLengthX = this->lengthX * 0.5f;
	float halfLengthY = this->lengthY * 0.5f;
	float halfLengthZ = this->lengthZ * 0.5f;

	D3DXVECTOR3 origPos = Particle.getPosition();

	float posX = origPos.x;
	float posY = origPos.y;
	float posZ = origPos.z;

	posX += ((float(rand()) / float(RAND_MAX)) * (halfLengthX - (-halfLengthX))) + (-halfLengthX);
	posY += ((float(rand()) / float(RAND_MAX)) * (halfLengthY - (-halfLengthY))) + (-halfLengthY);
	posZ += ((float(rand()) / float(RAND_MAX)) * (halfLengthZ - (-halfLengthZ))) + (-halfLengthZ);

	float dirX = (float)rand() / (float)RAND_MAX -0.5f;
	float dirY = (float)rand() / (float)RAND_MAX -0.5f;
	float dirZ = (float)rand() / (float)RAND_MAX -0.5f;

	
	Particle.setDirection(D3DXVECTOR3(dirX,dirY,dirZ));
	Particle.setPosition(D3DXVECTOR3(posX,posY,posZ));

	particles.insertLast(Particle);
}