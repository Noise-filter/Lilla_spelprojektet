#include "Node.h"

Node::Node() : Entity()
{
	color = 0;
}

Node::Node(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, int color) 
	: Entity(D3DXVECTOR3(pos.x, pos.y, pos.z), meshID, textureID, hp, lightID)
{
	this->color = color;

	scaleFactor = 0.4f;
	D3DXMatrixScaling(&scale, scaleFactor, scaleFactor, scaleFactor);
	D3DXMatrixTranslation(&pointTranslate, 1.8f, 0, -0.2f);

	rotationSpeed = (float)(rand() % 100 + 1) * 0.0005f;
	translateSpeed = 0.1f;
	upTranslate = rand() % 2;
	this->pos = pos;
	translatePosMax = 0.5;
}

Node::~Node()
{}

int Node::update(float dt)
{
	static float rotY = 0.0f;
	rotY += (rotationSpeed * dt);
	D3DXMatrixRotationY(&rotation, rotY);

	if(upTranslate)
	{
		pos.y += (translateSpeed * dt);
		D3DXMatrixTranslation(&translate, pos.x, pos.y, pos.z);
		if(pos.y >= translatePosMax)
			upTranslate = !upTranslate;
	}
	else
	{
		pos.y -= (translateSpeed * dt);
		D3DXMatrixTranslation(&translate, pos.x, pos.y, pos.z);
		if(pos.y <= -translatePosMax)
			upTranslate = !upTranslate;
	}

	renderData.worldMat = scale * pointTranslate * rotation * translate;
	return 1;
}

int Node::getColor()
{
	return this->color;
}