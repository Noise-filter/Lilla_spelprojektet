#include "Supply.h"

Supply::Supply() : Structure()
{
}

Supply::Supply(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID)
	: Structure(pos, meshID, textureID, hp, lightID)
{
	topSupply = new RenderData(ENTITY_SUPPLYTOP, 0, renderData.worldMat, 0);

	D3DXMatrixTranslation(&topPointTrans, 1.9, 0, -0.1);
	D3DXMatrixTranslation(&topTrans, pos.x-2, pos.y, pos.z);
	D3DXMatrixIdentity(&topRotation);

	rotationSpeed = (rand() % 10 + 1) * 0.05;
}

Supply::~Supply()
{
	SAFE_DELETE(topSupply);
}

int Supply::update(float dt)
{
	static float rotY = 0.0;
	rotY += (rotationSpeed * dt);

	D3DXMatrixRotationY(&topRotation, rotY);

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