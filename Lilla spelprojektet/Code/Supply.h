#ifndef SUPPLY_H
#define SUPPLY_H

#include "Structure.h"

class Supply : public Structure
{
public:
	Supply();
	Supply(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID);
	virtual ~Supply();

	virtual int update(float dt);
	
	vector<RenderData*> getRenderData();

private:
	RenderData* topSupply;
	D3DXMATRIX topPointTrans, topRotation, topTrans;
	float rotationSpeed;
	float rotY;

};

#endif