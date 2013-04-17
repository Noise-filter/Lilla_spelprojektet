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
			structures[i][j] = new Tower(D3DXVECTOR3(i*quadSize + (quadSize/2),0,j*quadSize + (quadSize/2)),1,1,0,0, 1, 1, 100, 100);
		}	
	}


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
			int id = structures[i][j]->update(dt);
			if(id == 2 && typeid(*structures[i][j]) == typeid(Tower))
			{
				dynamic_cast<Tower*>(structures[i][j])->aquireTarget(enemies);
			}
		}
	}

	return 1;
}

vector<RenderData*> Level::getRenderData()
{
	renderData.clear();

	for(int i = 0; i < mapSize; i++)
	{
		for(int j = 0; j < mapSize; j++)
		{
			renderData.push_back(&nodes[i][j].getRenderData());
		}
	}

	for(int i = 0; i < mapSize-1; i++)
	{
		for(int j = 0; j < mapSize-1; j++)
		{
			if(typeid(*structures[i][j]) == typeid(Tower))
			{
				vector<RenderData*> rData = dynamic_cast<Tower*>(structures[i][j])->getRenderData();
				
				//Lägg till tornets övre del
				renderData.push_back(rData.at(0));

				//lägg till tornets undre del och alla projektiler
				renderData.insert(renderData.begin(), rData.begin(), rData.end());
			}
			else
			{
				renderData.push_back(&structures[i][j]->getRenderData());
			}
		}
	}

	return renderData;
}