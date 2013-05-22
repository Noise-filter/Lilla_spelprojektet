#include "Supply.h"

Supply::Supply() : Structure()
{
}

Supply::Supply(Vec3 pos, int meshID, int textureID, float hp, int lightID, bool fakeBuilding)
	: Structure(pos, meshID, textureID, hp, lightID, fakeBuilding)
{
	topSupply = new RenderData(ENTITY_SUPPLYTOP, textureID, renderData.worldMat, 0);

	D3DXMatrixTranslation(&translate, pos.x+1.9f, pos.y, pos.z);

	D3DXMatrixTranslation(&topPointTrans, 2.5f, 0.0f, -0.1f);
	D3DXMatrixTranslation(&topTrans, pos.x-0.6, pos.y, pos.z);
	D3DXMatrixIdentity(&topRotation);

	rotationSpeed = 2.0f;
	rotY = 0.0f;

	renderData.worldMat = scale * pointTranslate * rotation * translate;
	topSupply->worldMat = scale * topPointTrans * topRotation * topTrans;
}

Supply::~Supply()
{
	SAFE_DELETE(topSupply);
}

int Supply::update(float dt)
{
	rotY += (rotationSpeed * dt);

	D3DXMatrixRotationY(&topRotation, rotY);

	topSupply->worldMat = scale * topPointTrans * topRotation * topTrans;

	return Structure::update(dt);
}

void Supply::setTextureID(int ID)
{
	renderData.textureID = ID;
	topSupply->textureID = ID;
}

vector<RenderData*> Supply::getRenderData()
{
	vector<RenderData*> renderData;
	renderData.reserve(2);

	renderData.push_back(&this->renderData);
	renderData.push_back(topSupply);

	return renderData;
}