#pragma once
#include "Level.h"
#include "EnemyHandler.h"
#include "WindowsUtility.h"

class GameLogic
{
public:
	GameLogic(void);
	~GameLogic(void);


	bool init(int quadSize, GameSettings &settings, string filename, int difficulty);
	int update(int &gameState, float dt, MouseState *mState, D3DXMATRIX view, D3DXMATRIX proj, D3DXVECTOR3 cameraPos);
	void incrementSelectedStructure(int increment);
	
	vector<vector<RenderData*>>& getRenderData();
	vector<HPBarInfo> getHPBarInfo();
	int getMapSize();

private:
	vector<vector<RenderData*>> rDataList;
	Level* level;
	EnemyHandler* eHandler;
	
	bool canAfford();
	void structureBuilt();
	void structureDestroyed();
	void giveSupply(float dt);
	void printSelected();

	D3DXVECTOR3 getMouseWorldPos(MouseState *mState, D3DXMATRIX view, D3DXMATRIX proj, D3DXVECTOR3 cameraPos);

	int selectedStructure;
	int availableSupply;
	int resource;
	int resPerTick;
	int resPerEnemy;
	float maxResCD;
	float currentResCD;

	Vec3 mouseWorldPos;
	Structure* selectedStructureRenderData;
};