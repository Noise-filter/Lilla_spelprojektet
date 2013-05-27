#include "Headquarter.h"

Headquarter::Headquarter() : Structure()
{
}

Headquarter::Headquarter(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, bool fakeBuilding)
	: Structure(pos, meshID, textureID, hp, lightID, fakeBuilding)
{

	D3DXMatrixRotationY(&rotation, PI/4);
	renderData.worldMat = scale * pointTranslate * rotation * translate;
}

Headquarter::~Headquarter()
{
}

int Headquarter::update(float dt)
{
	return Structure::update(dt);
}