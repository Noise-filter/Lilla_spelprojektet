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
			nodes[i][j] = Node(D3DXVECTOR3(i*quadSize,0,j*quadSize),0,0,0,0,0);
		}
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

	structures[0][0] = new Headquarter(D3DXVECTOR3(5, 0, 5), 2, 0, 2, 0);

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

bool Level::buildStructure(D3DXVECTOR3 mouseClickPos, int selectedStructure)
{
	int xPos = mouseClickPos.x/quadSize;
	int yPos = mouseClickPos.z/quadSize;

	if(xPos >= 0 && xPos < mapSize-1 && yPos >= 0 && yPos < mapSize-1)
	{
		if(structures[xPos][yPos] == NULL)
		{
			switch(selectedStructure)
			{
			case TOWER:
				structures[xPos][yPos] = new Tower(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)),0,1,100,0, 1, 1, 50, 100);
				break;
			case SUPPLY:
				structures[xPos][yPos] = new Supply(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)), 1,0,100,0);
				break;
			}
		}
	}

	cout << "a structure has been built on the location X:"<< xPos << " Y:" << yPos << endl; 
	return true;
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
					for(int k = 1; k < (int)rD.size(); k++)
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
					mainBuilding = j + (i * mapSize-1);
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
						supply += 20;
					else if(typeid(*structures[i][j]) == typeid(Supply))
						supply -= 20;
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