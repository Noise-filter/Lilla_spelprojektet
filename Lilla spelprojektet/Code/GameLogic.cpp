#include "GameLogic.h"



GameLogic::GameLogic(void)
{
	this->level = new Level();
	this->eHandler = new EnemyHandler();
	selectedStructure = 0;
}


GameLogic::~GameLogic(void)
{
	SAFE_DELETE(this->level);
	SAFE_DELETE(this->eHandler);
}

void GameLogic::incrementSelectedStructure(int increment)
{
	this->selectedStructure += increment;
}


int GameLogic::update(float dt, MouseState* mState, D3DXMATRIX view, D3DXMATRIX proj)
{

	switch(mState->btnState)
	{
		case VK_LBUTTON:
			level->buildStructure(getMouseWorldPos(mState, view, proj), this->selectedStructure);	
			break;
	}
	

	if(level->update(dt, eHandler->getEnemies()))
		return 0; //error
	if(eHandler->update(dt))
		return 0; //error


	return 1;//all went good
}

bool GameLogic::init(int mapSize, int quadSize)
{
	this->level->init(mapSize,quadSize);
	this->eHandler->init();

	return true;
}

vector<vector<RenderData*>> GameLogic::getRenderData()
{
	rDataList.clear();

	rDataList.push_back(level->getRenderData());
	rDataList.push_back(eHandler->getRenderData());

	return rDataList;	
}

D3DXVECTOR3 GameLogic::getMouseWorldPos(MouseState* mState, D3DXMATRIX view, D3DXMATRIX proj)
{
	float pointX, pointY, intersect;
	D3DXMATRIX invView, worldIdentity;
	D3DXVECTOR3 dir, origin, rayOrigin, rayDir, intersectPos, planeNormal;
	D3DXMatrixIdentity(&worldIdentity);

	//cout << "X: "<<mState.xPos << " Y: " << mState.yPos << endl;
	

	pointX = ((2.0f * (float)mState->xPos) / (float)screenWidth) - 1.0f;
	pointY = (((2.0f * (float)mState->yPos) / (float)screenHeight) - 1.0f) * -1.0f;

	//cout << "X: "<<pointX << " Y: " << pointY << endl;

	pointX = pointX / proj._11;
	pointY = pointY / proj._22;

	D3DXMatrixInverse(&invView, NULL, &view);

	dir.x = (pointX * invView._11) + (pointY * invView._21) + invView._31;
	dir.y = (pointX * invView._12) + (pointY * invView._22) + invView._32;
	dir.z = (pointX * invView._13) + (pointY * invView._23) + invView._33;

	origin = D3DXVECTOR3(45, 60, 45);

	D3DXVec3TransformCoord(&rayOrigin, &origin, &worldIdentity);
	D3DXVec3TransformNormal(&rayDir, &dir, &worldIdentity);

	// Normalize the ray direction.
	D3DXVec3Normalize(&rayDir, &rayDir);
	planeNormal = D3DXVECTOR3(0,1,0);
	intersect = (D3DXVec3Dot(&-planeNormal, &rayOrigin))/(D3DXVec3Dot(&planeNormal,&rayDir));

	//cout << "intersect: " << intersect << endl;
	return intersectPos = rayOrigin + (intersect*rayDir);
	//cout << "X: "<<intersectPos.x << " Y: " << intersectPos.y << " Z: " << intersectPos.z << endl;
}	
