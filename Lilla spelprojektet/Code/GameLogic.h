#pragma once
#include "Level.h"
#include "EnemyHandler.h"
class GameLogic
{
public:
	GameLogic(void);
	~GameLogic(void);

	vector<vector<RenderData*>> getRenderData();
	bool init(int mapSize, int quadSize);
	int update(float dt, MouseState *mState, D3DXMATRIX view, D3DXMATRIX proj);
	D3DXVECTOR3 getMouseWorldPos(MouseState *mState, D3DXMATRIX view, D3DXMATRIX proj);

private:
	vector<vector<RenderData*>> rDataList;
	Level* level;
	EnemyHandler* eHandler;
	int selectedStructure;
};

