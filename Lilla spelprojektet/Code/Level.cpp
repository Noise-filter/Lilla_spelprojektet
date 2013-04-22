#include "Level.h"

Level::Level(void)
{
	this->mapSize = 0;
	this->nodes = NULL;
	this->towerUpgrades = NULL;
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

	structures[0][0] = new Headquarter(D3DXVECTOR3(5, 0, 5), 2, 0, 2, 0);

	towerUpgrades = new TowerUpgrade[5];
	towerUpgrades[0] = TowerUpgrade(100,0,0,0,0);
	towerUpgrades[1] = TowerUpgrade(0,100,0,0,0);
	towerUpgrades[2] = TowerUpgrade(0,0,100,0,0);
	towerUpgrades[3] = TowerUpgrade(0,0,0,100,0);
	towerUpgrades[4] = TowerUpgrade(0,0,0,0,100);

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

	SAFE_DELETE_ARRAY(towerUpgrades);
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
					//En byggnad f�rst�rs
					if(typeid(structures[i][j]) == typeid(Tower*))
						supply += 20;
					else if(typeid(structures[i][j]) == typeid(Supply*))
						supply -= 20;

					//Ta bort byggnaden
					SAFE_DELETE(structures[i][j]);

					//Skapa m�ngder och hitta de byggnader som inte l�ngre sitter ihop med main byggnaden
					//r�kna ut vilka byggnader som kommer f�rst�ras
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
	if(nodes[xPos][yPos].getColor() == COLOR_RED)
	{
		return false;
	}
	if(nodes[xPos+1][yPos].getColor() == COLOR_RED)
	{
		return false;
	}
	if(nodes[xPos][yPos+1].getColor() == COLOR_RED)
	{
		return false;
	}
	if(nodes[xPos+1][yPos+1].getColor() == COLOR_RED)
	{
		return false;
	}
	return true;
}

bool Level::buildStructure(D3DXVECTOR3 mouseClickPos, int selectedStructure)
{
	int xPos = mouseClickPos.x/quadSize;
	int yPos = mouseClickPos.z/quadSize;

	if(xPos >= 0 && xPos < mapSize-1 && yPos >= 0 && yPos < mapSize-1)
	{
		if(structures[xPos][yPos] == NULL && isAdjecent(xPos,yPos) && isLocationBuildable(xPos, yPos))
		{
			switch(selectedStructure)
			{
			case TYPE_TOWER:
				structures[xPos][yPos] = new Tower(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)),0,0,10,0, 1, 1, 50, 100);
				break;
			case TYPE_SUPPLY:
				structures[xPos][yPos] = new Supply(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)), 1,0,100,0);
				break;
			case TYPE_UPGRADE_HP:
				structures[xPos][yPos] = new Upgrade(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)),1,0,100,0,0);
				break;
			case TYPE_UPGRADE_ATKSP:
				structures[xPos][yPos] = new Upgrade(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)),1,0,100,0,1);
				break;
			case TYPE_UPGRADE_DMG:
				structures[xPos][yPos] = new Upgrade(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)),1,0,100,0,2);
				break;
			case TYPE_UPGRADE_PRJSP:
				structures[xPos][yPos] = new Upgrade(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)),1,0,100,0,3);
				break;
			case TYPE_UPGRADE_RANGE:
				structures[xPos][yPos] = new Upgrade(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)),1,0,100,0,4);
				break;
			}
			cout << "a structure has been built on the location X:"<< xPos << " Y:" << yPos << endl;

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
			rData.at(nodes[i][j].getRenderData().meshID).push_back(&nodes[i][j].getRenderData());
		}
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

					//L�gg till tornets �vre del
					rData.at(4).push_back(rD.at(0));

					//l�gg till tornets undre del och alla projektiler
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
				if(sets.findSet(mainBuilding, false) != sets.findSet(j + (i * (mapSize-1)), false))
				{
					if(typeid(*structures[i][j]) == typeid(Tower))
						supply += COST_TOWER;
					else if(typeid(*structures[i][j]) == typeid(Supply))
						supply -= COST_SUPPLY;
					SAFE_DELETE(structures[i][j]);
				}
			}
		}
	}

	return supply;
}

void Level::makeSet(int x, int z)
{
	bool compress = true;
	bool rank = true;

	if(x > 0 && structures[x-1][z])
		sets.Union(z + (x*(mapSize-1)), z + ((x-1)*(mapSize-1)), compress, rank);

	if(x < mapSize-2 && structures[x+1][z])
		sets.Union(z + (x*(mapSize-1)), z + ((x+1)*(mapSize-1)), compress, rank);

	if(z > 0 && structures[x][z-1])
		sets.Union(z + (x*(mapSize-1)), z-1 + (x*(mapSize-1)), compress, rank);

	if(z < mapSize-2 && structures[x][z+1])
		sets.Union(z + (x*(mapSize-1)), z+1 + (x*(mapSize-1)), compress, rank);
}
