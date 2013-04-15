#include "Level.h"


Level::Level(void)
{
	this->mapSize = 0;
	this->nodes = NULL;
}

bool Level::init(int mapSize)
{
	this->mapSize = mapSize;

	nodes = new Node*[mapSize];
	for(int i = 0; i < mapSize; i++)
	{
		nodes[i] = new Node[mapSize];
	}

	for(int i = 0; i < mapSize; i++)
	{
		for(int j = 0; j < mapSize; j++)
		{
			nodes[i][j] = Node(D3DXVECTOR3(i*10,0,j*10),0,0,0,0,0);
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
}

int Level::update(float dt)
{
	return 1;
}

RenderData* Level::getRenderData()
{
	RenderData* rData = new RenderData[mapSize*mapSize];
	int index = 0;

	for(int i = 0; i < mapSize; i++)
	{
		for(int j = 0; j < mapSize; j++)
		{
			rData[index++] = nodes[i][j].getRenderData();
		}
	}

	return rData;
}
