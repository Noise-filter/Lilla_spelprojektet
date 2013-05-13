#pragma once
#include "Level.h"
#include "EnemyHandler.h"
#include "WindowsUtility.h"

class GameLogic
{
public:
	GameLogic(void);
	~GameLogic(void);

	vector<vector<RenderData*>>& getRenderData();
	bool init(int quadSize, GameSettings &settings);
	int update(int &gameState, float dt, MouseState *mState, D3DXMATRIX view, D3DXMATRIX proj, D3DXVECTOR3 cameraPos);
	void incrementSelectedStructure(int increment);
	vector<HPBarInfo> getHPBarInfo();
	int getMapSize();

private:
	vector<vector<RenderData*>> rDataList;
	Level* level;
	EnemyHandler* eHandler;
	int selectedStructure;
	bool canAfford();
	void structureBuilt();
	void structureDestroyed();
	void giveResource(float dt);
	

	void printSelected();

	D3DXVECTOR3 getMouseWorldPos(MouseState *mState, D3DXMATRIX view, D3DXMATRIX proj, D3DXVECTOR3 cameraPos);

	//resurser
	int availableSupply;
	int resource;
	int nrOfSupplyStructures;
	int resPerTick;
	float maxResCD;
	float currentResCD;
	//resurser
};

