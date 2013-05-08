#include "Level.h"

Level::Level(void)
{
	this->mapSize = 0;
	this->nodes = NULL;
	this->nrOfSupplyStructures = 0;
}

bool Level::init(int quadSize)
{
	this->availibleUpgrades = new UpgradeStats[5];
	this->availibleUpgrades[0] = (UpgradeStats(BUILDABLE_UPGRADE_HP,10,0,0,0,0));
	this->availibleUpgrades[1] = (UpgradeStats(BUILDABLE_UPGRADE_ATKSP,0,10,0,0,0));
	this->availibleUpgrades[2] = (UpgradeStats(BUILDABLE_UPGRADE_DMG,0,0,10,0,0));
	this->availibleUpgrades[3] = (UpgradeStats(BUILDABLE_UPGRADE_PRJSP,0,0,0,10,0));
	this->availibleUpgrades[4] = (UpgradeStats(BUILDABLE_UPGRADE_RANGE,0,0,0,0,10));

	//läs in karta från fil här
	this->quadSize = quadSize;

	return true;
}

void Level::constructNeutrals()
{
	//kolla igenom nodes och leta efter quads av neutrala noder
	//placera sedan en neutral byggnad 
	D3DXVECTOR3 pos = D3DXVECTOR3(0,0,0);
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
				pos = D3DXVECTOR3((float)i*quadSize + (quadSize/2),0,(float)j*quadSize + (quadSize/2));

				neutralStructures.push_back(Structure(pos,0,0,100,0));
			}
		}
	}
}

bool Level::loadLevel(string fileName)
{

	//rensa level och structures om något fanns där innan
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
	}
	

	//läs in fil med nivå
	ifstream fin;
	fin.open(fileName);
	string attribute;
	int value;
	int entityFlag;

	if(fin.fail() == true)
	{
		cout << "FAILED TO READ LEVEL FROM FILE" << endl;
	}

	//läs in mapSize
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
				entityFlag = ENTITY_NODE_GREEN;
			else if(value == COLOR_RED)
				entityFlag = ENTITY_NODE_RED;
					
			//lägg till kollar för texturer

			nodes[i][j] = Node(D3DXVECTOR3((float)i*quadSize,0,(float)j*quadSize),entityFlag,0,0,0,value);
			cout << value << " , ";
		}
		fin.ignore();
		cout << endl;
	}
	
	//läs in ytterligare info efter spelplanen
	//fin >> line; // detta är nästa rad av värde

	fin.close();

	constructNeutrals();

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

	SAFE_DELETE_ARRAY(availibleUpgrades);
}

int Level::update(float dt, vector<Enemy*>& enemies)
{
	int supply = 0;
	bool buildingDestroyed = false;

	for(int i = 0; i < mapSize-1; i++)
	{
		for(int j = 0; j < mapSize-1; j++)
		{
			if(structures[i][j] != NULL)
			{
				if(structures[i][j]->isDead())
				{
					//En byggnad förstörs
					if(typeid(*structures[i][j]) == typeid(Tower))
						supply += 20;
					else if(typeid(*structures[i][j]) == typeid(Supply))
						supply -= 20;
					else if(typeid(*structures[i][j]) == typeid(Upgrade))
					{
						//remove this upgrade from all towers on the map
						removeUpgrade(dynamic_cast<Upgrade*>(structures[i][j])->getUpgradeID());
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
						int id = structures[i][j]->update(dt);

						if(id == 2 && typeid(*structures[i][j]) == typeid(Tower))
						{
							dynamic_cast<Tower*>(structures[i][j])->aquireTarget(&enemies);
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

	if((float)nrOfStructures/((mapSize-1) * (mapSize-1)) > 0.40f)
	{
		return 4; // win
	}

	if(buildingDestroyed)
	{
		//Skapa mängder och hitta de byggnader som inte längre sitter ihop med main byggnaden
		//räkna ut vilka byggnader som kommer förstöras
		sets.initSets(structures, mapSize-1);
		supply += destroyBuildings();
	}

	return supply;
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
				dynamic_cast<Tower*>(structures[i][j])->giveUpgrade(availibleUpgrades[selectedUpgrade-2]);
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
				dynamic_cast<Tower*>(structures[i][j])->removeUpgrade(availibleUpgrades[selectedUpgrade-2]);
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
	int counter = 0;

	if(nodes[xPos][yPos].getColor() == COLOR_GREEN)
	{
		counter++;
	}
	if(nodes[xPos+1][yPos].getColor() == COLOR_GREEN)
	{
		counter++;
	}
	if(nodes[xPos][yPos+1].getColor() == COLOR_GREEN)
	{
		counter++;
	}
	if(nodes[xPos+1][yPos+1].getColor() == COLOR_GREEN)
	{
		counter++;
	}
	
	if(counter == 4)
	{
		return true;
	}
	else
	{
		return false;
	}
		

}

bool Level::buildStructure(D3DXVECTOR3 mouseClickPos, int selectedStructure)
{
	int xPos = (int)(mouseClickPos.x/quadSize);
	int yPos = (int)(mouseClickPos.z/quadSize);

	if(xPos >= 0 && xPos < mapSize-1 && yPos >= 0 && yPos < mapSize-1)
	{	

		if(selectedStructure == BUILDABLE_MAINBUILDING && structures[xPos][yPos] == NULL && isLocationBuildable(xPos, yPos))
		{ 
			structures[xPos][yPos] = new Headquarter(D3DXVECTOR3((float)xPos*quadSize + (quadSize/2),0,(float)yPos*quadSize + (quadSize/2)), ENTITY_MAINBUILDING, 0, 30, 0);
			return true;
		
		}
		else if(structures[xPos][yPos] == NULL && isAdjecent(xPos,yPos) && isLocationBuildable(xPos, yPos))
		{
			bool builtUpgrade = false;
			switch(selectedStructure)
			{
			case BUILDABLE_TOWER:
				structures[xPos][yPos] = new Tower(D3DXVECTOR3((float)xPos*quadSize + (quadSize/2),0,(float)yPos*quadSize + (quadSize/2)),ENTITY_TOWERBASE,0,100,0, 10, 1, 50, 100);
				for(int i = 0; i < (int)this->upgradesInUse.size();i++)
				{
					dynamic_cast<Tower*>(structures[xPos][yPos])->giveUpgrade(upgradesInUse[i]);
				}
				break;
			case BUILDABLE_SUPPLY:
				structures[xPos][yPos] = new Supply(D3DXVECTOR3((float)xPos*quadSize + (quadSize/2),0,(float)yPos*quadSize + (quadSize/2)), ENTITY_SUPPLYBASE,0,100,0);
				this->nrOfSupplyStructures++;
				break;
			case BUILDABLE_UPGRADE_HP:
				structures[xPos][yPos] = new Upgrade(D3DXVECTOR3((float)xPos*quadSize + (quadSize/2),0,(float)yPos*quadSize + (quadSize/2)),
					ENTITY_SUPPLYBASE,0,100,0,BUILDABLE_UPGRADE_HP);
				upgradesInUse.push_back(availibleUpgrades[(BUILDABLE_UPGRADE_HP)-2]);
				builtUpgrade = true;
				break;
			case BUILDABLE_UPGRADE_ATKSP:
				structures[xPos][yPos] = new Upgrade(D3DXVECTOR3((float)xPos*quadSize + (quadSize/2),0,(float)yPos*quadSize + (quadSize/2)),
					ENTITY_SUPPLYBASE,0,100,0,BUILDABLE_UPGRADE_ATKSP);
				upgradesInUse.push_back(availibleUpgrades[(BUILDABLE_UPGRADE_ATKSP)-2]);
				builtUpgrade = true;
				break;
			case BUILDABLE_UPGRADE_DMG:
				structures[xPos][yPos] = new Upgrade(D3DXVECTOR3((float)xPos*quadSize + (quadSize/2),0,(float)yPos*quadSize + (quadSize/2)),
					ENTITY_SUPPLYBASE,0,100,0,BUILDABLE_UPGRADE_DMG);
				upgradesInUse.push_back(availibleUpgrades[(BUILDABLE_UPGRADE_DMG)-2]);
				builtUpgrade = true;
				break;
			case BUILDABLE_UPGRADE_PRJSP:
				structures[xPos][yPos] = new Upgrade(D3DXVECTOR3((float)xPos*quadSize + (quadSize/2),0,(float)yPos*quadSize + (quadSize/2)),
					ENTITY_SUPPLYBASE,0,100,0,BUILDABLE_UPGRADE_PRJSP);
				upgradesInUse.push_back(availibleUpgrades[(BUILDABLE_UPGRADE_PRJSP)-2]);
				builtUpgrade = true;
				break;
			case BUILDABLE_UPGRADE_RANGE:
				structures[xPos][yPos] = new Upgrade(D3DXVECTOR3((float)xPos*quadSize + (quadSize/2),0,(float)yPos*quadSize + (quadSize/2)),
					ENTITY_SUPPLYBASE,0,100,0,BUILDABLE_UPGRADE_RANGE);
				upgradesInUse.push_back(availibleUpgrades[(BUILDABLE_UPGRADE_RANGE)-2]);
				builtUpgrade = true;
				break;
			}
			cout << "a structure has been built on the location X:"<< xPos << " Y:" << yPos << endl;

			if(builtUpgrade)
			{
				upgradeStructures(selectedStructure);
				cout << "the structure was an upgrade: " << selectedStructure <<endl;
			}

			return true;
		}
	}
	return false;
}

void Level::getRenderData(vector<vector<RenderData*>>& rData)
{
	//Lägg till alla noder i renderData
	for(int i = 0; i < mapSize; i++)
	{
		for(int j = 0; j < mapSize; j++)
		{
			rData[nodes[i][j].getRenderData().meshID].push_back(&nodes[i][j].getRenderData());
		}
	}
	
	for(int i = 0; i < (int)neutralStructures.size(); i++)
	{
		rData.at(1).push_back(&neutralStructures.at(i).getRenderData());
	}

	//Lägg till alla byggnader i renderData
	for(int i = 0; i < mapSize-1; i++)
	{
		for(int j = 0; j < mapSize-1; j++)
		{
			if(structures[i][j] != NULL)
			{
				//Om det är ett torn lägg till övre delen och alla projektiler
				if(typeid(*structures[i][j]) == typeid(Tower))
				{
					vector<RenderData*> rD = dynamic_cast<Tower*>(structures[i][j])->getRenderData();

					//lägg till tornets undre del, övre del och alla projektiler
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
}

int Level::destroyBuildings()
{
	int supply = 0;
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
					if(typeid(*structures[i][j]) == typeid(Tower))
						supply += COST_TOWER;
					else if(typeid(*structures[i][j]) == typeid(Supply))
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

	return supply;
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
	for(int i = 0; i < mapSize; i++)
	{
		for(int j = 0; j < mapSize; j++)
		{
			if(structures[i][j])
			{
				hpBars.push_back(structures[i][j]->getHPBarInfo());
			}
		}
	}
}