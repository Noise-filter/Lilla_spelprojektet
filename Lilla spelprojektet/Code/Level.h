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
#include "Statistics.h"
#include <random>
using namespace std;

class Level
{
public:
	Level(void);
	~Level(void);

	
	bool init(int quadSize, int difficulty);
	int update(float dt, vector<Enemy*>& enemies);


	int getExtraResPerEnemy();
	void getRenderData(vector<vector<RenderData*>>& rData);
	int getNrOfSupplyStructures();
	Structure*** getStructures();
	Node** getNodes();
	int getMapSize();
	int getQuadSize();
	void getHPBarInfo(vector<HPBarInfo>& hpBars);

	bool buildStructure(Vec3 mouseClickPos, int selectedStructure);
	bool isAdjecent(int xPos, int yPos);
	bool isLocationBuildable(int xPos, int yPos);
	bool isEmpty(int xPos, int yPos);
	bool isGrey(int xPos, int yPos);

	bool loadLevel(string fileName);

	int getNrOfBuildings()const;
	int getGoal()const;

private:
	int destroyBuildings();
	void makeSet(int x, int z);
	void upgradeStructures(int selectedUpgrade);
	void removeUpgrade(int selectedUpgrade);
	void constructNeutrals();

private:
	Node** nodes;
	Structure*** structures;
	vector<Structure*> neutralStructures;

	Entity* plane;
	UpgradeStats* availibleUpgrades;
	vector<UpgradeStats> upgradesInUse;
	Set<Structure*> sets;

	float winPercent;
	float currPercent;
	int nrOfSupplyStructures;
	int quadSize;
	int mapSize;
	int extraResPerEnemy;
	int nrOfStructures;
	int goal;
};

