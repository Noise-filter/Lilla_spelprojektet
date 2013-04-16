#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "Entity.h"

class Structure : public Entity
{
public:
	Structure();
	Structure(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID);
	virtual ~Structure();

	virtual int update(float dt);


private:


};

#endif