#include "Headquarter.h"

Headquarter::Headquarter() : Structure()
{
}

Headquarter::Headquarter(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID)
	: Structure(pos, meshID, textureID, hp, lightID)
{

	D3DXMatrixRotationY(&rotation, PI/4);
	//scaleFactor = 2.8;
	//D3DXMatrixScaling(&scale, scaleFactor, scaleFactor, scaleFactor);

}

Headquarter::~Headquarter()
{
}

int Headquarter::update(float dt)
{
	return Structure::update(dt);
}