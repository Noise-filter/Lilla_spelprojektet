#include "GameLogic.h"

GameLogic::GameLogic(void)
{
	this->level = new Level();
	this->eHandler = new EnemyHandler();
	this->selectedStructure = 2;
	this->availableSupply = 40;
	this->resource = 20;
	this->maxResCD = 0;
	this->resPerEnemy = 5;
}

GameLogic::~GameLogic(void)
{
	SAFE_DELETE(this->level);
	SAFE_DELETE(this->eHandler);
}

void GameLogic::incrementSelectedStructure(int increment)
{
	if(selectedStructure >= 1 && selectedStructure <= BUILDABLE_UPGRADE_RES)
	{
		this->selectedStructure += increment;
		printSelected();
	}
}
void GameLogic::setSelectedStructure(int select)
{
	if(selectedStructure >= 1 && selectedStructure <= BUILDABLE_UPGRADE_RES)
	{
		this->selectedStructure = select;
		printSelected();
	}
}
int GameLogic::getMapSize()
{
	return this->level->getMapSize();
}

void GameLogic::giveSupply(float dt)
{
	currentResCD += dt;
	if(currentResCD > maxResCD)
	{
		this->availableSupply += resPerTick + level->getNrOfSupplyStructures()/2;
		cout << "gained resources: " << resPerTick + level->getNrOfSupplyStructures()/2 << " you now have: " << availableSupply << endl;
		currentResCD = 0;
	}
}

bool GameLogic::canAfford()
{
		switch(this->selectedStructure)
			{
			case BUILDABLE_TOWER:
				if(availableSupply >= COST_TOWER)
				{
					return true;
				}
				break;
			case BUILDABLE_SUPPLY:
				if(resource >= COST_SUPPLY)
				{
					return true;
				}
				break;
			case BUILDABLE_UPGRADE_OFFENSE:
				if(resource >= COST_UPGRADE)
				{
					return true;
				}
				break;
			case BUILDABLE_UPGRADE_DEFENSE:
				if(resource >= COST_UPGRADE)
				{
					return true;
				}
				break;
			case BUILDABLE_UPGRADE_RES:
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
		case BUILDABLE_TOWER:
			availableSupply -= COST_TOWER; 
			break;
		case BUILDABLE_SUPPLY:
			resource -= COST_SUPPLY;
			break;
		case BUILDABLE_UPGRADE_OFFENSE:
			resource -= COST_UPGRADE;
			break;
		case BUILDABLE_UPGRADE_DEFENSE:
			resource -= COST_UPGRADE;
			break;
		case BUILDABLE_UPGRADE_RES:
			resource -= COST_UPGRADE;
			break;
	}

}


int GameLogic::update(int &gameState, float dt, MouseState* mState, D3DXMATRIX view, D3DXMATRIX proj, Vec3 cameraPos)
{
	if(gameState == STATE_GAMESTART)
	{
		if(mState->btnState == VK_LBUTTON)
		{
			//placera ut main byggnad
			if(level->buildStructure(getMouseWorldPos(mState, view, proj, cameraPos), BUILDABLE_MAINBUILDING))
			{
				gameState = STATE_PLAYING;
				cout << "mainstruct buildt" << endl;
			}
		}
	}

	int ret = level->update(dt, eHandler->getEnemies()); // returnera 4 om vinst 5 om förlust
	if(gameState == STATE_PLAYING)
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

		giveSupply(dt);
		
		if(ret == 4) //win
		{
			gameState = STATE_WIN;
		}
		else if(ret == 5) //lose
		{
			gameState = STATE_LOSE;	
		}

		int nrOfKilledEnemies = eHandler->update(dt);
		if(nrOfKilledEnemies > 0)
		{
			this->resource += resPerEnemy* level->getExtraResPerEnemy()  + resPerEnemy * nrOfKilledEnemies;
		}
	}
	
	if(gameState == STATE_WIN || gameState == STATE_LOSE)
	{
		//ska något göras här?
	}

	return 1;//all went good
}

bool GameLogic::init(int quadSize, GameSettings &settings, string filename, int difficulty)
{
	this->resPerTick = settings.resPerTick;
	this->maxResCD = (float)settings.resCD;
	this->currentResCD = 0;

	this->level->init(quadSize, difficulty);
	this->level->loadLevel(filename);


	this->eHandler->init(level->getStructures(), level->getNodes(), level->getMapSize(), quadSize,settings.enemiesPerMin,settings.difficulty);

	vector<RenderData*> renderData;

	for(int i = 0; i < NROFDIFFERENTMESHES; i++)	//antal olika mesher
		rDataList.push_back(renderData);


	return true;
}

vector<vector<RenderData*>>& GameLogic::getRenderData()
{
	for(int i = 0; i < (int)rDataList.size(); i++)
		rDataList.at(i).clear();


	level->getRenderData(rDataList);
	eHandler->getRenderData(rDataList);

	return rDataList;
}

void GameLogic::printSelected()
{
	switch(this->selectedStructure)
	{
		case BUILDABLE_SUPPLY:
		cout << "SUPPLY SELECTED" << endl;
		break;
		case BUILDABLE_TOWER:
		cout << "TOWER SELECTED" << endl;
		break;
		case BUILDABLE_UPGRADE_OFFENSE:
		cout << "OFFENSE UPGRADE SELECTED" << endl;
		break;
		case BUILDABLE_UPGRADE_DEFENSE:
		cout << "DEFENSE UPGRADE SELECTED" << endl;
		break;
		case BUILDABLE_UPGRADE_RES:
		cout << "RES UPGRADE SELECTED" << endl;
		break;
	}
}
Vec3 GameLogic::getMouseWorldPos(MouseState* mState, D3DXMATRIX view, D3DXMATRIX proj, Vec3 cameraPos)
{
	float pointX, pointY, intersect;
	
	D3DXMATRIX invView, worldIdentity;
	Vec3 dir, origin, rayOrigin, rayDir, intersectPos, planeNormal;
	D3DXMatrixIdentity(&worldIdentity);

	
	origin = cameraPos;
	pointX = ((2.0f * (float)mState->xPos) / (float)SCREEN_WIDTH) - 1.0f;
	pointY = (((2.0f * (float)mState->yPos) / (float)SCREEN_HEIGHT) - 1.0f) * -1.0f;

	pointX = pointX / proj._11;
	pointY = pointY / proj._22;

	D3DXMatrixInverse(&invView, NULL, &view);

	dir.x = (pointX * invView._11) + (pointY * invView._21) + invView._31;
	dir.y = (pointX * invView._12) + (pointY * invView._22) + invView._32;
	dir.z = (pointX * invView._13) + (pointY * invView._23) + invView._33;

	D3DXVec3TransformCoord(&rayOrigin, &origin, &worldIdentity);
	D3DXVec3TransformNormal(&rayDir, &dir, &worldIdentity);

	D3DXVec3Normalize(&rayDir, &rayDir);
	planeNormal = Vec3(0,1,0);
	intersect = (D3DXVec3Dot(&-planeNormal, &rayOrigin))/(D3DXVec3Dot(&planeNormal,&rayDir));

	return intersectPos = rayOrigin + (intersect*rayDir);
}

vector<HPBarInfo> GameLogic::getHPBarInfo()
{
	vector<HPBarInfo> hpBars;
	level->getHPBarInfo(hpBars);
	eHandler->getHPBarInfo(hpBars);

	return hpBars;
}
