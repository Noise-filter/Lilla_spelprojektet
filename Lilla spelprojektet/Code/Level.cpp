#include "Level.h"

Level::Level(void)
{
	this->mapSize = 0;
	this->nodes = NULL;
	this->nrOfSupplyStructures = 0;
	this->extraResPerEnemy = 0;
	this->winPercent = 0;
}

bool Level::init(int quadSize, int difficulty)
{
	this->availibleUpgrades = new UpgradeStats[3];
	this->availibleUpgrades[0] = (UpgradeStats(BUILDABLE_UPGRADE_OFFENSE,0,1,0.1f,1));
	this->availibleUpgrades[1] = (UpgradeStats(BUILDABLE_UPGRADE_DEFENSE,30,0,0,0));
	this->availibleUpgrades[2] = (UpgradeStats(BUILDABLE_UPGRADE_RES,0,0,0,0));

	//l�s in karta fr�n fil h�r
	this->quadSize = quadSize;
	this->winPercent = 0.50f;

	if(difficulty == DIFF_EASY)
		this->winPercent = 0.40f;
	if(difficulty == DIFF_MEDIUM)
		this->winPercent = 0.50f;
	if(difficulty == DIFF_HARD)
		this->winPercent = 0.60f;

	return true;
}

void Level::constructNeutrals()
{
	//kolla igenom nodes och leta efter quads av neutrala noder
	//placera sedan en neutral byggnad
	
	Vec3 pos = Vec3(0,0,0);
	int counter = 0;
	for(int i = 0; i < mapSize-1; i++)
	{
		for(int j = 0; j < mapSize-1; j++)
		{
			counter = 0;

			if(nodes[i][j].getColor() == COLOR_GREY)
			{
				counter++;
			}
			if(nodes[i+1][j].getColor() == COLOR_GREY)
			{
				counter++;		
			}
			if(nodes[i][j+1].getColor() == COLOR_GREY)
			{
				counter++;		
			}
			if(nodes[i+1][j+1].getColor() == COLOR_GREY)
			{
				counter++;			
			}
			if(counter == 4)
			{
				pos = Vec3((float)i*quadSize + (quadSize/2),0,(float)j*quadSize + (quadSize/2));
				neutralStructures.push_back(new Structure(pos,1,0,100,0));
			}
		}
	}
}

bool Level::loadLevel(string fileName)
{

	//rensa level och structures om n�got fanns d�r innan
	if(mapSize > 0)
	{
		for(int i = 0; i < this->mapSize; i++)
		{
			SAFE_DELETE_ARRAY(nodes[i]);
		}
		SAFE_DELETE_ARRAY(nodes);

		for(int i = 0; i < this->mapSize-1; i++)
		{
			for(int j = 0; j < mapSize-1; j++)
			{
				SAFE_DELETE(structures[i][j]);
			}

			SAFE_DELETE_ARRAY(structures[i]);
		}
		SAFE_DELETE_ARRAY(structures);
		neutralStructures.clear();
	}
	

	//l�s in fil med niv�
	ifstream fin;
	fin.open(fileName);
	string attribute;
	int value;
	int entityFlag;

	if(fin.fail() == true)
	{
		cout << "FAILED TO READ LEVEL FROM FILE" << endl;
	}

	//l�s in mapSize
	fin >> value;
	this->mapSize = value;

	//initiera
	nodes = new Node*[mapSize];
	for(int i = 0; i < mapSize; i++)
	{
		nodes[i] = new Node[mapSize];
	}
	structures = new Structure**[mapSize-1];
	for(int i = 0; i < mapSize-1; i++)
	{
		structures[i] = new Structure*[mapSize-1];
	}
	for(int i = 0; i < mapSize-1; i++)
	{
		for(int j = 0; j < mapSize-1; j++)
		{
			structures[i][j] = NULL;
		}
	}
	//initiera

	char val;
	fin.ignore();
	for(int i = 0; i < mapSize; i++)
	{
		for(int j = 0; j < mapSize; j++)
		{
			fin.get(val);
			value = ((int)val-48); // konverterar till int
			if(value == COLOR_GREEN)
				nodes[i][j] = Node(Vec3((float)i*quadSize,0,(float)j*quadSize),ENTITY_NODE_GREEN,0,0,0,value);//entityFlag = ENTITY_NODE_GREEN;
			else if(value == COLOR_RED)
				nodes[i][j] = Node(Vec3((float)i*quadSize,0,(float)j*quadSize),ENTITY_NODE_GREEN,1,0,0,value);//entityFlag = ENTITY_NODE_RED;
			else if(value == COLOR_GREY)
				nodes[i][j] = Node(Vec3((float)i*quadSize,0,(float)j*quadSize),ENTITY_NODE_GREEN,2,0,0,value);//entityFlag = ENTITY_NODE_GREEN;
					
			//l�gg till kollar f�r texturer

			//nodes[i][j] = Node(Vec3((float)i*quadSize,0,(float)j*quadSize),entityFlag,0,0,0,value);
			cout << value << " , ";
		}
		fin.ignore();
		cout << endl;
	}
	
	//l�s in ytterligare info efter spelplanen
	//fin >> line; // detta �r n�sta rad av v�rde

	fin.close();

	constructNeutrals();

	//skapa planet
	plane = new Entity(Vec3((mapSize-1) * quadSize * 0.5f, 0, (mapSize-1) * quadSize * 0.5f), ENTITY_PLANE, 0, 0, 0);
	plane->setScale((float)(mapSize-1)*quadSize);

	return true;
}
Level::~Level(void)
{
	for(int i = 0; i < this->mapSize; i++)
	{
		SAFE_DELETE_ARRAY(nodes[i]);
	}
	SAFE_DELETE_ARRAY(nodes);

	for(int i = 0; i < this->mapSize-1; i++)
	{
		for(int j = 0; j < mapSize-1; j++)
		{
			SAFE_DELETE(structures[i][j]);
		}

		SAFE_DELETE_ARRAY(structures[i]);
	}
	SAFE_DELETE_ARRAY(structures);
	for(int i = 0; i < (int)neutralStructures.size(); i++)
	{
		SAFE_DELETE(neutralStructures.at(i));
	}

	SAFE_DELETE_ARRAY(availibleUpgrades);

	SAFE_DELETE(plane);
}
int Level::getExtraResPerEnemy()
{
	return this->extraResPerEnemy;
}

int Level::update(float dt, vector<Enemy*>& enemies)
{
	bool buildingDestroyed = false;

	for(int i = 0; i < mapSize-1; i++)
	{
		for(int j = 0; j < mapSize-1; j++)
		{
			if(structures[i][j] != NULL)
			{
				if(structures[i][j]->isDead())
				{
					//En byggnad f�rst�rs
					if(typeid(*structures[i][j]) == typeid(Tower))
					{
						
					}
					else if(typeid(*structures[i][j]) == typeid(Supply))
					{
						
					}
					else if(typeid(*structures[i][j]) == typeid(Upgrade))
					{
						if(dynamic_cast<Upgrade*>(structures[i][j])->getUpgradeID() == BUILDABLE_UPGRADE_RES)
						{
							this->extraResPerEnemy -= 2;
						}
						else
						{
							removeUpgrade(dynamic_cast<Upgrade*>(structures[i][j])->getUpgradeID());
						}
					}
					else if(typeid(*structures[i][j]) == typeid(Headquarter))
					{
						return 5; // mainbuilding died, you lose
					}

					//Ta bort byggnaden
					SAFE_DELETE(structures[i][j]);

					buildingDestroyed = true;
				}
				else
				{
					if(structures[i][j])
					{
						if(!structures[i][j]->isDead())
						{
							int id = structures[i][j]->update(dt);

							if(id == 2 && typeid(*structures[i][j]) == typeid(Tower))
							{
								dynamic_cast<Tower*>(structures[i][j])->aquireTarget(&enemies);
							}
						}
						else
						{
							cout << "FAIL!!!!!!!!!!!!!" << endl;
							cout << "FAIL!!!!!!!!!!!!!" << endl;
							cout << "FAIL!!!!!!!!!!!!!" << endl;
							cout << "FAIL!!!!!!!!!!!!!" << endl;
							cout << "FAIL!!!!!!!!!!!!!" << endl;
						}
					}
				}
			}
		}
	}
	int nrOfStructures = 0;
	for(int i = 0; i < mapSize-1; i++)
	{
		for(int j = 0; j  < mapSize-1; j++)
		{
			if(structures[i][j] != NULL)
			{
				nrOfStructures++;
			}
		}
	}

	for(int i = 0; i < mapSize; i++)
	{
		for(int j = 0; j < mapSize; j++)
		{
			nodes[i][j].update(dt);
		}
	}

	if((float)nrOfStructures/((mapSize-1) * (mapSize-1)) > winPercent)
	{
		return 4; // win
	}

	if(buildingDestroyed)
	{
		//Skapa m�ngder och hitta de byggnader som inte l�ngre sitter ihop med main byggnaden
		//r�kna ut vilka byggnader som kommer f�rst�ras
		sets.initSets(structures, mapSize-1);
		destroyBuildings();
	}

	return 1;
}

void Level::upgradeStructures(int selectedUpgrade)
{
	for(int i = 0; i < this->mapSize-1; i++)
	{
		for(int j = 0; j < this->mapSize-1; j++)
		{
			if(structures[i][j] != NULL && typeid(*structures[i][j]) == typeid(Tower))
			{
				//cout << "hp before: "<< dynamic_cast<Tower*>(structures[i][j])->getHp() << endl;
				dynamic_cast<Tower*>(structures[i][j])->giveUpgrade(availibleUpgrades[selectedUpgrade-3]);
				//cout << "hp after: "<< dynamic_cast<Tower*>(structures[i][j])->getHp() << endl;
			}
		}	
	}
}

void Level::removeUpgrade(int selectedUpgrade)
{
	for(int i = 0; i < this->mapSize-1; i++)
	{
		for(int j = 0; j < this->mapSize-1; j++)
		{
			if(structures[i][j] != NULL && typeid(*structures[i][j]) == typeid(Tower))
			{
				dynamic_cast<Tower*>(structures[i][j])->removeUpgrade(availibleUpgrades[selectedUpgrade-3]);
			}
		}	
	}
}

int Level::getNrOfSupplyStructures()
{
	return this->nrOfSupplyStructures;
}

bool Level::isAdjecent(int xPos, int yPos)
{
	if(xPos < mapSize-2 && structures[xPos+1][yPos] != NULL)
	{
		return true;
	}

	if(xPos > 0 && structures[xPos-1][yPos] != NULL)
	{
		return true;
	}

	if(yPos < mapSize-2 && structures[xPos][yPos+1] != NULL)
	{
		return true;
	}

	if(yPos > 0 && structures[xPos][yPos-1] != NULL)
	{
		return true;
	}	

	return false;
}

bool Level::isLocationBuildable(int xPos, int yPos)
{
	int greenCounter = 0;
	int greyCounter = 0;

	if(nodes[xPos][yPos].getColor() == COLOR_GREEN)
	{
		greenCounter++;
	}
	else if(nodes[xPos][yPos].getColor() == COLOR_GREY)
	{
		greyCounter++;
	}

	if(nodes[xPos+1][yPos].getColor() == COLOR_GREEN)
	{
		greenCounter++;
	}
	else if(nodes[xPos+1][yPos].getColor() == COLOR_GREY)
	{
		greyCounter++;
	}

	if(nodes[xPos][yPos+1].getColor() == COLOR_GREEN)
	{
		greenCounter++;
	}
	else if(nodes[xPos][yPos+1].getColor() == COLOR_GREY)
	{
		greyCounter++;
	}

	if(nodes[xPos+1][yPos+1].getColor() == COLOR_GREEN)
	{
		greenCounter++;
	}
	else if(nodes[xPos+1][yPos+1].getColor() == COLOR_GREY)
	{
		greyCounter++;
	}
	
	if(greenCounter == 4)
	{
		return true;
	}
	else if((greenCounter + greyCounter) == 4 && greenCounter > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Level::buildStructure(Vec3 mouseClickPos, int selectedStructure)
{
	int xPos = (int)(mouseClickPos.x/quadSize);
	int yPos = (int)(mouseClickPos.z/quadSize);

	if(xPos >= 0 && xPos < mapSize-1 && yPos >= 0 && yPos < mapSize-1)
	{
		if(selectedStructure == BUILDABLE_MAINBUILDING && structures[xPos][yPos] == NULL && isLocationBuildable(xPos, yPos))
		{
			structures[xPos][yPos] = new Headquarter(Vec3((float)xPos*quadSize + (quadSize/2),0,(float)yPos*quadSize + (quadSize/2)), ENTITY_MAINBUILDING, 0, 50, 0);
			return true;
		}
		else if(structures[xPos][yPos] == NULL && isAdjecent(xPos,yPos) && isLocationBuildable(xPos, yPos))
		{
			bool builtUpgrade = false;
			switch(selectedStructure)
			{
				case BUILDABLE_TOWER:
					structures[xPos][yPos] = new Tower(Vec3((float)xPos*quadSize + (quadSize/2),0,(float)yPos*quadSize + (quadSize/2)),ENTITY_TOWERBASE,0,100,0,20, 1, 25, 100);
					for(int i = 0; i < (int)this->upgradesInUse.size();i++)
					{
						dynamic_cast<Tower*>(structures[xPos][yPos])->giveUpgrade(upgradesInUse[i]);
					}
					break;
				case BUILDABLE_SUPPLY:
					structures[xPos][yPos] = new Supply(Vec3((float)xPos*quadSize + (quadSize/2),0,(float)yPos*quadSize + (quadSize/2)), ENTITY_SUPPLYBASE,0,100,0);
					this->nrOfSupplyStructures++;
					break;
				case BUILDABLE_UPGRADE_OFFENSE:
					structures[xPos][yPos] = new Upgrade(Vec3((float)xPos*quadSize + (quadSize/2),0,(float)yPos*quadSize + (quadSize/2)),
						ENTITY_SUPPLYBASE,0,100,0,BUILDABLE_UPGRADE_OFFENSE);
					upgradesInUse.push_back(availibleUpgrades[(BUILDABLE_UPGRADE_OFFENSE)-3]);
					builtUpgrade = true;
					break;
				case BUILDABLE_UPGRADE_DEFENSE:
					structures[xPos][yPos] = new Upgrade(Vec3((float)xPos*quadSize + (quadSize/2),0,(float)yPos*quadSize + (quadSize/2)),
						ENTITY_SUPPLYBASE,0,100,0,BUILDABLE_UPGRADE_DEFENSE);
					upgradesInUse.push_back(availibleUpgrades[(BUILDABLE_UPGRADE_DEFENSE)-3]);
					builtUpgrade = true;
					break;
				case BUILDABLE_UPGRADE_RES:
					structures[xPos][yPos] = new Upgrade(Vec3((float)xPos*quadSize + (quadSize/2),0,(float)yPos*quadSize + (quadSize/2)),
						ENTITY_SUPPLYBASE,0,100,0,BUILDABLE_UPGRADE_RES);
					this->extraResPerEnemy += 2;
					break;
				cout << "a structure has been built on the location X:"<< xPos << " Y:" << yPos << endl;

				if(builtUpgrade)
				{
					upgradeStructures(selectedStructure);
					cout << "the structure was an upgrade: " << selectedStructure <<endl;
				}
			
			}
			return true;
		}
	}
	return false;
}

void Level::getRenderData(vector<vector<RenderData*>>& rData)
{
	//L�gg till alla noder i renderData
	for(int i = 0; i < mapSize; i++)
	{
		for(int j = 0; j < mapSize; j++)
		{
			rData[nodes[i][j].getRenderData().meshID].push_back(&nodes[i][j].getRenderData());
		}
	}
	
	for(int i = 0; i < (int)neutralStructures.size(); i++)
	{
		rData.at(2).push_back(&neutralStructures.at(i)->getRenderData());
	}

	//L�gg till alla byggnader i renderData
	for(int i = 0; i < mapSize-1; i++)
	{
		for(int j = 0; j < mapSize-1; j++)
		{	
			if(structures[i][j] != NULL)
			{
				//Om det �r ett torn l�gg till �vre delen och alla projektiler
				if(typeid(*structures[i][j]) == typeid(Tower))
				{
					vector<RenderData*> rD = dynamic_cast<Tower*>(structures[i][j])->getRenderData();

					//l�gg till tornets undre del, �vre del och alla projektiler
					for(int k = 0; k < (int)rD.size(); k++)
						rData.at(rD.at(k)->meshID).push_back(rD.at(k));
				}
				else if(typeid(*structures[i][j]) == typeid(Supply))
				{
					vector<RenderData*> rD = dynamic_cast<Supply*>(structures[i][j])->getRenderData();

					for(int k = 0; k < (int)rD.size(); k++)
						rData.at(rD.at(k)->meshID).push_back(rD.at(k));
				}
				else
				{
					rData.at(structures[i][j]->getRenderData().meshID).push_back(&structures[i][j]->getRenderData());
				}
			}
		}
	}

	rData.at(plane->getRenderData().meshID).push_back(&plane->getRenderData());

}

int Level::destroyBuildings()
{
	int mainBuilding = -1;

	for(int i = 0; i < mapSize-1; i++)
	{
		for(int j = 0; j < mapSize-1; j++)
		{
			if(structures[i][j])
			{
				makeSet(i, j);
				if(typeid(*structures[i][j]) == typeid(Headquarter))
					mainBuilding = j + (i * (mapSize-1));
			}
		}
	}

	for(int i = 0; i < mapSize-1; i++)
	{
		for(int j = 0; j < mapSize-1; j++)
		{
			if(structures[i][j])
			{
				if(sets.findSet(mainBuilding) != sets.findSet(j + (i * (mapSize-1))))
				{
					if(typeid(*structures[i][j]) == typeid(Supply))
					{
						nrOfSupplyStructures--;
					}

					else if(typeid(*structures[i][j]) == typeid(Upgrade))
					{
						//remove this upgrade from all towers on the map
						removeUpgrade(dynamic_cast<Upgrade*>(structures[i][j])->getUpgradeID());
					}

					structures[i][j]->setDead(true);
				}
			}
		}
	}

	return 1;
}

void Level::makeSet(int x, int z)
{
	if(x > 0 && structures[x-1][z])
		sets.Union(z + (x*(mapSize-1)), z + ((x-1)*(mapSize-1)));

	if(x < mapSize-2 && structures[x+1][z])
		sets.Union(z + (x*(mapSize-1)), z + ((x+1)*(mapSize-1)));

	if(z > 0 && structures[x][z-1])
		sets.Union(z + (x*(mapSize-1)), z-1 + (x*(mapSize-1)));

	if(z < mapSize-2 && structures[x][z+1])
		sets.Union(z + (x*(mapSize-1)), z+1 + (x*(mapSize-1)));
}

Structure*** Level::getStructures()
{
	return structures;
}

Node** Level::getNodes()
{
	return nodes;
}

int Level::getMapSize()
{
	return mapSize;
}

void Level::getHPBarInfo(vector<HPBarInfo>& hpBars)
{
	for(int i = 0; i < mapSize-1; i++)
	{
		for(int j = 0; j < mapSize-1; j++)
		{
			if(structures[i][j])
			{
				hpBars.push_back(structures[i][j]->getHPBarInfo());
			}
		}
	}
}