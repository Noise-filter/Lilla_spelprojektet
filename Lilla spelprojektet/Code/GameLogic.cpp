#include "GameLogic.h"

GameLogic::GameLogic(void)
{
	this->level = new Level();
	this->eHandler = new EnemyHandler();
	selectedStructure = 0;
	this->availableSupply = 100;
	this->resource = 100;
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

bool GameLogic::canAfford()
{
		switch(this->selectedStructure)
			{
			case TYPE_TOWER:
				if(availableSupply >= COST_TOWER)
				{
					return true;
				}
				break;
			case TYPE_SUPPLY:
				if(resource >= COST_SUPPLY)
				{
					return true;
				}
				break;
			case TYPE_UPGRADE_HP:
				if(resource >= COST_UPGRADE)
				{
					return true;
				}
				break;
			case TYPE_UPGRADE_ATKSP:
				if(resource >= COST_UPGRADE)
				{
					return true;
				}
				break;
				case TYPE_UPGRADE_DMG:
				if(resource >= COST_UPGRADE)
				{
					return true;
				}
				break;
				case TYPE_UPGRADE_PRJSP:
				if(resource >= COST_UPGRADE)
				{
					return true;
				}
				break;
				case TYPE_UPGRADE_RANGE:
				if(resource >= COST_UPGRADE)
				{
					return true;
				}
				break;
			}
		
		cout << "Cant afford structure" << endl; 
		return false;
}
void GameLogic::structureBuilt()
{
	switch(this->selectedStructure)
			{
			case TYPE_TOWER:
				availableSupply -= COST_TOWER; 
				break;
			case TYPE_SUPPLY:
				availableSupply += COST_TOWER;
				resource -= COST_SUPPLY;
				break;
			case TYPE_UPGRADE_HP:
				resource -= COST_UPGRADE;
				break;
			case TYPE_UPGRADE_ATKSP:
				resource -= COST_UPGRADE;
				break;
			case TYPE_UPGRADE_DMG:
				resource -= COST_UPGRADE;
				break;
			case TYPE_UPGRADE_PRJSP:
				resource -= COST_UPGRADE;
				break;
			case TYPE_UPGRADE_RANGE:
				resource -= COST_UPGRADE;
				break;
			}

}


int GameLogic::update(float dt, MouseState* mState, D3DXMATRIX view, D3DXMATRIX proj, D3DXVECTOR3 cameraPos)
{
	switch(mState->btnState)
	{
		case VK_LBUTTON:
			if(canAfford())
			{
				if(level->buildStructure(getMouseWorldPos(mState, view, proj, cameraPos), this->selectedStructure))
				{
					structureBuilt();
				}
			}
			break;
	}

	level->update(dt, eHandler->getEnemies());

	if(!eHandler->update(dt))
		return 0; //error

	return 1;//all went good
}

bool GameLogic::init(int mapSize, int quadSize)
{
	this->level->init(mapSize,quadSize);
	this->eHandler->init();

	vector<RenderData*> renderData;

	for(int i = 0; i < 5; i++)	//antal olika mesher
		rDataList.push_back(renderData);

	return true;
}

vector<vector<RenderData*>> GameLogic::getRenderData()
{
	for(int i = 0; i < (int)rDataList.size(); i++)
		rDataList.at(i).clear();

	level->getRenderData(rDataList);
	eHandler->getRenderData(rDataList);

	return rDataList;
}

D3DXVECTOR3 GameLogic::getMouseWorldPos(MouseState* mState, D3DXMATRIX view, D3DXMATRIX proj, D3DXVECTOR3 cameraPos)
{
	float pointX, pointY, intersect;
	D3DXMATRIX invView, worldIdentity;
	D3DXVECTOR3 dir, origin, rayOrigin, rayDir, intersectPos, planeNormal;
	D3DXMatrixIdentity(&worldIdentity);

	
	origin = cameraPos;
	pointX = ((2.0f * (float)mState->xPos) / (float)screenWidth) - 1.0f;
	pointY = (((2.0f * (float)mState->yPos) / (float)screenHeight) - 1.0f) * -1.0f;

	pointX = pointX / proj._11;
	pointY = pointY / proj._22;

	D3DXMatrixInverse(&invView, NULL, &view);

	dir.x = (pointX * invView._11) + (pointY * invView._21) + invView._31;
	dir.y = (pointX * invView._12) + (pointY * invView._22) + invView._32;
	dir.z = (pointX * invView._13) + (pointY * invView._23) + invView._33;

	D3DXVec3TransformCoord(&rayOrigin, &origin, &worldIdentity);
	D3DXVec3TransformNormal(&rayDir, &dir, &worldIdentity);

	//Normalize the ray direction.
	D3DXVec3Normalize(&rayDir, &rayDir);
	planeNormal = D3DXVECTOR3(0,1,0);
	intersect = (D3DXVec3Dot(&-planeNormal, &rayOrigin))/(D3DXVec3Dot(&planeNormal,&rayDir));

	return intersectPos = rayOrigin + (intersect*rayDir);
}

