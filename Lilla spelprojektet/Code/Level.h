#pragma once
#include "D3DMathUtility.h"
#include "Node.h"
#include "Structure.h"
#include "Enemy.h"
#include "Tower.h"
#include "Headquarter.h"
#include "Supply.h"
#include "Set.h"
#include "Upgrade.h"
#include <vector>
using namespace std;

class Level
{
public:
	Level(void);
	~Level(void);

	bool init(int mapSize,int quadSize);

	void getRenderData(vector<vector<RenderData*>>& rData);
	int update(float dt, vector<Enemy*>& enemies);
	
	bool buildStructure(D3DXVECTOR3 mouseClickPos, int selectedStructure);
	bool isAdjecent(int xPos, int yPos);
	bool isLocationBuildable(int xPos, int yPos);
	int getNrOfSupplyStructures();

	Structure*** getStructures();
	Node** getNodes();
	int getMapSize();
	void getHPBarInfo(vector<HPBarInfo>& hpBars);

private:
	int destroyBuildings();
	void makeSet(int x, int z);
	void upgradeStructures(int selectedUpgrade);
	void removeUpgrade(int selectedUpgrade);
	void constructNeutrals();

private:
	Node** nodes;
	Structure*** structures;
	int nrOfSupplyStructures;
	vector<Structure> neutralStructures;

	

	UpgradeStats* availibleUpgrades;
	vector<UpgradeStats> upgradesInUse;

	Set<Structure*> sets;

	int quadSize;
	int mapSize;
};

