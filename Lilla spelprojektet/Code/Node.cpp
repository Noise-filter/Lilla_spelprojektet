#include "Node.h"

Node::Node() : Entity()
{
	color = 0;
}

Node::Node(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, int color) 
	: Entity(pos, meshID, textureID, hp, lightID)
{
	this->color = color;

	//this->renderData.worldMat = scale * this->renderData.worldMat;
	/*renderData.worldMat._11 = scale._11;
	renderData.worldMat._22 = scale._22;
	renderData.worldMat._33 = scale._33;
	*/
	//D3DXMatrixTranslation(&renderData.worldMat, pos.x, pos.y, pos.z);
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