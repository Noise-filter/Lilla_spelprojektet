#ifndef TRAIL_H
#define TRAIL_H

#include "ParticlePolicy.h"

class Trail : public ParticlePolicy
{
	private:
		float lengthX;
		float lengthY;
		float lengthZ;
		void emitt();

	public:
		Trail(D3DXVECTOR3 color, D3DXVECTOR3 position , int intensity, float timeToLive, float velocity, float lengthX, float lengthY, float lengthZ);
		~Trail();

		int update(float dt);
};

#endif