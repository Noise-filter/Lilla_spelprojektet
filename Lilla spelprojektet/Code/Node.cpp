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
	D3DXMatrixTranslation(&pointTranslate, 1.4f, 0, -0.2);

	rotationSpeed = (float)(rand() % 100 + 1) * 0.0005;
}

Node::~Node()
{}

int Node::update(float dt)
{
	static float rotY = 0.0f;
	rotY += (rotationSpeed * dt);
	D3DXMatrixRotationY(&rotation, rotY);

	renderData.worldMat = scale * pointTranslate * rotation * translate;
	return 1;
}

int Node::getColor()
{
	return this->color;
}