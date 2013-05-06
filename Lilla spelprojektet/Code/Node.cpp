#include "Node.h"

Node::Node() : Entity()
{
	color = 0;
}

Node::Node(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, int color) 
	: Entity(pos, meshID, textureID, hp, lightID)
{
	this->color = color;

	renderData.worldMat._11 = 0.3;
	renderData.worldMat._22 = 0.3;
	renderData.worldMat._33 = 0.3;
}

Node::~Node()
{}

int Node::update(float dt)
{
	return 1;
}

int Node::getColor()
{
	return this->color;
}