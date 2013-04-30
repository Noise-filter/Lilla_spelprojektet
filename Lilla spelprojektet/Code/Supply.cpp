#include "Supply.h"

Supply::Supply() : Structure()
{
}

Supply::Supply(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID)
	: Structure(pos, meshID, textureID, hp, lightID)
{
}

Supply::~Supply()
{
}

int Supply::update(float dt)
{
	return Structure::update(dt);
}