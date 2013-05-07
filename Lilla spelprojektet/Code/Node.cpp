#include "Node.h"

Node::Node() : Entity()
{
	color = 0;
}

Node::Node(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, int color) 
	: Entity(D3DXVECTOR3(pos.x, pos.y, pos.z), meshID, textureID, hp, lightID)
{
	this->color = color;

	scaleFactor = 0.3;
	D3DXMatrixScaling(&scale, scaleFactor, scaleFactor, scaleFactor);
}

Node::~Node()
{}

int Node::update(float dt)
{
	renderData.worldMat = scale * pointTranslate * rotation * translate;
	return 1;
}

int Node::getColor()
{
	return this->color;
}