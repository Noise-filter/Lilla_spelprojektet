#include "Upgrade.h"

Upgrade::Upgrade()
{
	this->upgradeID = 0;
}

Upgrade::Upgrade(Vec3 pos, int meshID, int textureID, float hp, int lightID, int upgradeID, bool fakeBuilding)
	: Structure(pos, meshID, textureID, hp, lightID, fakeBuilding)
{
	this->upgradeID = upgradeID;
	D3DXMatrixTranslation(&translate, pos.x+2, pos.y, pos.z);
	renderData.worldMat = scale * pointTranslate * rotation * translate;
}

Upgrade::~Upgrade()
{
}

int Upgrade::update(float dt)
{
	return Structure::update(dt);
}

int Upgrade::getUpgradeID()
{
	return this->upgradeID;
}