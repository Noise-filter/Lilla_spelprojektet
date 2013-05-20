#ifndef UPGRADE_H
#define UPGRADE_H

#include "Structure.h"

class Upgrade : public Structure
{
public:
	Upgrade();
	Upgrade(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, int upgradeID, bool fakeBuilding);
	virtual ~Upgrade();

	virtual int update(float dt);

	int getUpgradeID();


private:
	int upgradeID;

};

#endif