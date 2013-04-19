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
	structures[5][5] = new Tower(D3DXVECTOR3(5*quadSize + (quadSize/2),0,5*quadSize + (quadSize/2)),0,1,0,0, 1, 1, 50, 100);


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
	for(int i = 0; i < mapSize-1; i++)
	{
		for(int j = 0; j < mapSize-1; j++)
		{
			if(structures[i][j] != NULL)
			{
				int id = structures[i][j]->update(dt);
				if(id == 2 && typeid(*structures[i][j]) == typeid(Tower))
				{
					dynamic_cast<Tower*>(structures[i][j])->aquireTarget(&enemies);
				}
			}
		}
	}

	return 1;
}

bool Level::isAdjecent(int xPos, int yPos)
{


	if(structures[xPos+1][yPos] != NULL)
	{
		return true;
	}	
	
	if(xPos > 0 && structures[xPos-1][yPos] != NULL)
	{
		return true;
	}
		
	
	if(structures[xPos][yPos+1] != NULL)
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
				structures[xPos][yPos] = new Tower(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)),0,1,0,0, 1, 1, 50, 100);
				break;
			case TYPE_SUPPLY:
				structures[xPos][yPos] = new Supply(D3DXVECTOR3(xPos*quadSize + (quadSize/2),0,yPos*quadSize + (quadSize/2)), 1,0,0,0);
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
