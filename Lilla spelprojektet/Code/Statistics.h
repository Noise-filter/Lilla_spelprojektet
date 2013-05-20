#pragma once
#include "D3DMathUtility.h"
#include "WindowsUtility.h"
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class Statistics
{
public:
	static Statistics* Getinstance();
	bool init();
	bool shutdown();

	bool writeScoreToFile(string fileName);
	int readScoreFromFile(string fileName, string levelName);

public:
	int totalScore;
	int totalRes;
	int totalSupply;
	int totalNrOfBuildings;
	int totalEnemiesKilled;
	int nrOfUpgrades;
	int nrOfMaxLvlTowers;
	
	float totalTime;
	float averageTowerLvl;

	string levelName;

private:
	Statistics(void);
	~Statistics(void);

private:
	static Statistics* stats;
};

