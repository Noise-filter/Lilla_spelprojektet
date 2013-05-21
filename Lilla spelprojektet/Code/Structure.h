#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "Entity.h"
#include "ParticleSystem.h"

class Structure : public Entity
{
public:
	Structure();
	Structure(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, bool fakeBuilding);
	virtual ~Structure();

	virtual int update(float dt);

	HPBarInfo getHPBarInfo();

	/*
		HP/MAXHP
		TRANSLATE MATRIX
	*/


protected:
	bool fake;

};

#endif