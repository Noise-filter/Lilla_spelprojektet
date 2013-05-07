#include "Supply.h"

Supply::Supply() : Structure()
{
}

Supply::Supply(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID)
	: Structure(pos, meshID, textureID, hp, lightID)
{
	topSupply = new RenderData(ENTITY_SUPPLYTOP, 0, renderData.worldMat, 0);

	D3DXMatrixTranslation(&topPointTrans, 0, 0, 0);
	D3DXMatrixTranslation(&topTrans, pos.x, pos.y, pos.z);
	D3DXMatrixIdentity(&topRotation);
}

Supply::~Supply()
{
	SAFE_DELETE(topSupply);
}

int Supply::update(float dt)
{
	topSupply->worldMat = scale * topPointTrans * topRotation * topTrans;

	return Structure::update(dt);
}

vector<RenderData*> Supply::getRenderData()
{
	vector<RenderData*> renderData;
	renderData.reserve(2);

	renderData.push_back(&this->renderData);
	renderData.push_back(topSupply);

	return renderData;
}