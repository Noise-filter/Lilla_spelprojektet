#ifndef HEADQUARTER_H
#define HEADQUARTER_H

#include "Structure.h"

class Headquarter : public Structure
{
public:
	Headquarter();
	Headquarter(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, bool fakeBuilding);
	virtual ~Headquarter();

	virtual int update(float dt);

private:


};

#endif