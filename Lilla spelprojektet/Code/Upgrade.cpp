#include "Upgrade.h"

Upgrade::Upgrade()
{
	this->upgradeID = 0;
}

Upgrade::Upgrade(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, int upgradeID)
	: Structure(pos, meshID, textureID, hp, lightID)
{
	this->upgradeID = upgradeID;
}

Upgrade::~Upgrade()
{
}

int Upgrade::update(float dt)
{

	return 1;
}