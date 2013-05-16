#include "Headquarter.h"

Headquarter::Headquarter() : Structure()
{
}

Headquarter::Headquarter(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID)
	: Structure(pos, meshID, textureID, hp, lightID)
{
	D3DXMatrixRotationY(&rotation, 0);
}

Headquarter::~Headquarter()
{
}

int Headquarter::update(float dt)
{
	return Structure::update(dt);
}