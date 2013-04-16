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

	/*structures = new Structure*[mapSize-1];
	for(int i = 0; i < mapSize-1; i++)
	{
		structures[i] = new Structure[mapSize-1];
	}*/
	/*for(int i = 0; i < mapSize-1; i++)
	{
		for(int j = 0; j < mapSize-1; j++)
		{
			structures[i][j] = structure(D3DXVECTOR3(i*quadSize + (quadSize/2),0,j*quadSize + (quadSize/2)),0,0,0,0,0);
		}	
	}*/


	return true;
}

Level::~Level(void)
{
	for(int i = 0; i < this->mapSize; i++)
	{
		SAFE_DELETE_ARRAY(nodes[i]);
	}
	SAFE_DELETE_ARRAY(nodes);
}

int Level::update(float dt)
{
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

	return renderData;
}
