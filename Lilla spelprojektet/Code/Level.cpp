#include "Level.h"

Level::Level(void)
{
	this->mapSize = 0;
	this->nodes = NULL;
}

bool Level::init(int mapSize, int quadSize)
{
	this->mapSize = mapSize;
	this->quadSize = quadSize;

	nodes = new Node*[mapSize];
	for(int i = 0; i < mapSize; i++)
	{
		nodes[i] = new Node[mapSize];
	}

	for(int i = 0; i < mapSize; i++)
	{
		for(int j = 0; j < mapSize; j++)
		{
			nodes[i][j] = Node(D3DXVECTOR3(i*quadSize,0,j*quadSize),0,0,0,0,COLOR_GREEN);
		}
	}
	nodes[3][3] = Node(D3DXVECTOR3(3*quadSize,0,3*quadSize),0,0,0,0,COLOR_RED);
	nodes[3][4] = Node(D3DXVECTOR3(3*quadSize,0,4*quadSize),0,0,0,0,COLOR_RED);
	nodes[3][5] = Node(D3DXVECTOR3(3*quadSize,0,5*quadSize),0,0,0,0,COLOR_RED);
	nodes[3][6] = Node(D3DXVECTOR3(3*quadSize,0,6*quadSize),0,0,0,0,COLOR_RED);
	nodes[3][7] = Node(D3DXVECTOR3(3*quadSize,0,7*quadSize),0,0,0,0,COLOR_RED);

	nodes[4][4] = Node(D3DXVECTOR3(4*quadSize,0,4*quadSize),0,0,0,0,COLOR_GREY);
	nodes[4][5] = Node(D3DXVECTOR3(4*quadSize,0,5*quadSize),0,0,0,0,COLOR_GREY);
	nodes[5][4] = Node(D3DXVECTOR3(5*quadSize,0,4*quadSize),0,0,0,0,COLOR_GREY);
	nodes[5][5] = Node(D3DXVECTOR3(5*quadSize,0,5*quadSize),0,0,0,0,COLOR_GREY);
	 
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

	structures[5][5] = new Headquarter(D3DXVECTOR3(55, 0, 55), 2, 0, 2, 0);

	this->availibleUpgrades = new UpgradeStats[5];
	this->availibleUpgrades[0] = (UpgradeStats(TYPE_UPGRADE_HP,10,0,0,0,0));
	this->availibleUpgrades[1] = (UpgradeStats(TYPE_UPGRADE_ATKSP,0,10,0,0,0));
	this->availibleUpgrades[2] = (UpgradeStats(TYPE_UPGRADE_DMG,0,0,10,0,0));
	this->availibleUpgrades[3] = (UpgradeStats(TYPE_UPGRADE_PRJSP,0,0,0,10,0));
	this->availibleUpgrades[4] = (UpgradeStats(TYPE_UPGRADE_RANGE,0,0,0,0,10));

	constructNeutrals();

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
				pos = D3DXVECTOR3(i*quadSize + (quadSize/2),0,j*quadSize + (quadSize/2));

				neutralStructures.push_back(Structure(pos,1,0,100,0));
			}
		}
	}
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
	for(int i = 0; i < mapSize-1; i++)
	{
		for(int j = 0; j < mapSize-1; j++)
		{
			if(structures[i][j] != NULL)
			{
				int id = structures[i][j]->update(dt);
				if(id == 0)
				{
					//En byggnad förstörs
					if(typeid(structures[i][j]) == typeid(Tower*))
						supply += 20;
					else if(typeid(structures[i][j]) == typeid(Supply*))
						supply -= 20;
					else if(typeid(structures[i][j]) == typeid(Upgrade*))
					{
						//remove this upgrade from all towers on the map
						removeUpgrade(dynamic_cast<Upgrade*>(structures[i][j])->getUpgradeID());
					}

					//Ta bort byggnaden
					SAFE_DELETE(structures[i][j]);

					//Skapa mängder och hitta de byggnader som inte längre sitter ihop med main byggnaden
					//räkna ut vilka byggnader som kommer förstöras
					sets.initSets(structures, mapSize-1);
					supply += destroyBuildings();
				}
				else if(id == 2 && typeid(*structures[i][j]) == typeid(Tower))
				{
					dynamic_cast<Tower*>(structures[i][j])->aquireTarget(&enemies);
				}
			}
		}
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
	int xPos = mouseClickPos.x/quadSize;
	int yPos = mouseClickPos.z/quadSize;

	if(xPos >= 0 && xPos < mapSize-1 && yPos >= 0 && yPos < mapSize-1)
	{
		if(structures[xPos][yPos] == NULL && isAdjecent(xPos,yPos) && isLocationBuildable(xPos, yPos))
		{
			bool builtUpgrade = false;
			switch(selectedStructure)
			{
			case TYPE_TOWER:
				structures[xPos][yPos] = new Tower(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)),0,0,10,0, 1, 1, 50, 100);
				for(int i = 0; i < (int)this->upgradesInUse.size();i++)
				{
					dynamic_cast<Tower*>(structures[xPos][yPos])->giveUpgrade(upgradesInUse[i]);
				}
				break;
			case TYPE_SUPPLY:
				structures[xPos][yPos] = new Supply(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)), 1,0,100,0);
				break;
			case TYPE_UPGRADE_HP:
				structures[xPos][yPos] = new Upgrade(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)),
					1,0,100,0,TYPE_UPGRADE_HP);
				upgradesInUse.push_back(availibleUpgrades[(TYPE_UPGRADE_HP)-2]);
				builtUpgrade = true;
				break;
			case TYPE_UPGRADE_ATKSP:
				structures[xPos][yPos] = new Upgrade(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)),
					1,0,100,0,TYPE_UPGRADE_ATKSP);
				upgradesInUse.push_back(availibleUpgrades[(TYPE_UPGRADE_ATKSP)-2]);
				builtUpgrade = true;
				break;
			case TYPE_UPGRADE_DMG:
				structures[xPos][yPos] = new Upgrade(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)),
					1,0,100,0,TYPE_UPGRADE_DMG);
				upgradesInUse.push_back(availibleUpgrades[(TYPE_UPGRADE_DMG)-2]);
				builtUpgrade = true;
				break;
			case TYPE_UPGRADE_PRJSP:
				structures[xPos][yPos] = new Upgrade(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)),
					1,0,100,0,TYPE_UPGRADE_PRJSP);
				upgradesInUse.push_back(availibleUpgrades[(TYPE_UPGRADE_PRJSP)-2]);
				builtUpgrade = true;
				break;
			case TYPE_UPGRADE_RANGE:
				structures[xPos][yPos] = new Upgrade(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)),
					1,0,100,0,TYPE_UPGRADE_RANGE);
				upgradesInUse.push_back(availibleUpgrades[(TYPE_UPGRADE_RANGE)-2]);
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
			rData.at(nodes[i][j].getRenderData().meshID).push_back(&nodes[i][j].getRenderData());
		}
	}
	
	for(int i = 0; i < neutralStructures.size(); i++)
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

					//Lägg till tornets övre del
					rData.at(4).push_back(rD.at(0));

					//lägg till tornets undre del och alla projektiler
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
	int mainBuilding = 0;

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
						supply -= COST_SUPPLY;
					else if(typeid(structures[i][j]) == typeid(Upgrade*))
					{
						//remove this upgrade from all towers on the map
						removeUpgrade(dynamic_cast<Upgrade*>(structures[i][j])->getUpgradeID());
					}

					SAFE_DELETE(structures[i][j]);
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