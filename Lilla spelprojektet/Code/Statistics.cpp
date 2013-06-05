#include "Statistics.h"


Statistics::Statistics(void)
{
	totalScore = 0;
	totalRes = 0;
	totalSupply = 0;
	totalNrOfBuildings = 0;
	totalEnemiesKilled = 0;
	totalNrOfUpgrades = 0;
	totalNrOfMaxLvlTowers = 0;
	
	totalTime = 0;
	averageTowerLvl = 0;

	levelName = "";

}


Statistics::~Statistics(void)
{

}

Statistics* Statistics::stats = NULL;
Statistics* Statistics::Getinstance()
{
	if(stats==NULL)
	{
		stats = new Statistics();
	}
	return stats;
}

bool Statistics::init()
{

	totalScore = 0;
	totalRes = 0;
	totalSupply = 0;
	totalNrOfBuildings = 0;
	totalEnemiesKilled = 0;
	totalNrOfUpgrades = 0;
	totalNrOfMaxLvlTowers = 0;
	
	totalTime = 0;
	averageTowerLvl = 0;

	levelName = "";
	ifstream fin;
	fin.open("highscore.txt");
	if(fin.fail())
	{
		ofstream fout;
		fout.open("highscore.txt");

		
		fout << 0;
		
		fout.close();
	}
	fin.close();
	
	return true;
}

bool Statistics::shutdown()
{
	SAFE_DELETE(stats);	
	return true;
}

bool Statistics::writeScoreToFile(string fileName)
{
	//write the totalTime that it took for player to win if the time is higher than the current written time
	ifstream fin(fileName);
	
	vector<string> levelNames;
	vector<float> totalTimes;
	int nrOfEntries = 0;
	bool writeNewScore = false;
	bool levelExists = false;
	

	if(!fin)
	{
		return false;
	}
	fin >> nrOfEntries;
	levelNames.resize(nrOfEntries);
	totalTimes.resize(nrOfEntries);

	for(int i = 0; i < nrOfEntries; i++)
	{
		string tempLevelName = "";
		float tempTotalTime = 10000.0f;

		fin >> tempLevelName >> tempTotalTime;
		
		totalTimes[i] = tempTotalTime;
		levelNames[i] = tempLevelName;

		if(tempLevelName == this->levelName)
		{
			levelExists = true;
			
			if(this->totalTime < tempTotalTime)
			{
				totalTimes[i] = totalTime;
				writeNewScore = true;
			}
		}

	}
	if(!levelExists)
	{
		
		writeNewScore = true;
		nrOfEntries++;

		levelNames.resize(nrOfEntries);
		totalTimes.resize(nrOfEntries);

		levelNames[nrOfEntries-1] = this->levelName;
		totalTimes[nrOfEntries-1] = this->totalTime;
	}
	fin.close();

	if(writeNewScore)
	{
		//skriv in den nya scoren på vald plats
		
		ofstream fout;

		fout.open(fileName);
		
		fout << nrOfEntries << endl;
		cout << "writing score" << endl;
		for(int i = 0; i < nrOfEntries; i++)
		{
			fout << levelNames[i] << " " << totalTimes[i] << endl;
			cout << "name: " << levelNames[i]  << " score: " << totalTimes[i] << endl;
		}

		

		fout.close();
	}

	return true;
}

int Statistics::readScoreFromFile(string fileName,string levelName)
{
	//search for the given levelname in the file and then read back the totalTime score and return it
	ifstream fin;
	vector<string> levelNames;
	vector<float> totalTimes;
	int nrOfEntries = 0;

	
	
	fin.open(fileName);

	if(fin.fail() == true)
	{
		return false;
	}
	fin >> nrOfEntries;
	levelNames.resize(nrOfEntries);
	totalTimes.resize(nrOfEntries);

	for(int i = 0; i < nrOfEntries; i++)
	{
		string tempLevelName = "";
		float tempTotalTime = 0.0f;

		fin >> tempLevelName >> tempTotalTime;
		
		if(tempLevelName == levelName)
		{
			return tempTotalTime;
		}

	}
	fin.close();

	return 0;	
}