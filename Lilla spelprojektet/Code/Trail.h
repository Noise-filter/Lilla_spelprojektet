#ifndef TRAIL_H
#define TRAIL_H

#include "ParticlePolicy.h"
#include <ctime>

class Trail : public ParticlePolicy
{
	private:
		std::vector<VertexColor> vertices;

		D3DXVECTOR3 color;
		float lengthX;
		float lengthY;
		float lengthZ;
		void emitt();
		void createVertices();

		void updatePosition(D3DXVECTOR3 pos);

	public:
		Trail(D3DXVECTOR3 color, D3DXVECTOR3 position , int intensity, int timeToLive, float velocity, float lengthX, float lengthY, float lengthZ);
		~Trail();

		void update();
		vector<VertexColor> getVertexData();
};

#endif