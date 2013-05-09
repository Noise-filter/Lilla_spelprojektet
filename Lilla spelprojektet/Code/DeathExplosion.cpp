#include "DeathExplosion.h"

DeathExplosion::DeathExplosion() : ParticlePolicy()
{

}

DeathExplosion::DeathExplosion(Vec3 color, Vec3 position, int intensity, float timeToLive, float velocity)
	: ParticlePolicy(color, position, intensity, timeToLive, velocity)
{
	emitt();
}

DeathExplosion::~DeathExplosion()
{
}

int DeathExplosion::update(float dt)
{
	if(particles.size() == 0)
	{
		return 2;
	}

	int i = 0;
	for(ListNode<BaseParticle>* walker = particles.getFirst(); walker != NULL; walker = walker->next)
	{
		if(!walker->value.update(dt))
		{
			i++;
		}
	}

	if(i > 0)
		recreateVertices = true;

	particles.remove(i);
	createVertices();
	
	recreateVertices = false;
	return 1;
}

void DeathExplosion::emitt()
{
	BaseParticle Particle = *this->emitter;
	Vec3 origPos = Particle.getPosition();

	float posX = origPos.x;
	float posY = origPos.y;
	float posZ = origPos.z;

	float dirX;
	float dirY;
	float dirZ;

	for(int i = 0; i < intensity; i++)
	{
		dirX = (float)rand() / (float)RAND_MAX -0.5f;
		dirY = (float)rand() / (float)RAND_MAX -0.5f;
		dirZ = (float)rand() / (float)RAND_MAX -0.5f;

		Particle.setDirection(Vec3(dirX,dirY,dirZ));
		Particle.setPosition(Vec3(posX,posY,posZ));

		particles.insertLast(Particle);
	}
	recreateVertices = true;
}