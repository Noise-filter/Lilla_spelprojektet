#include "Node.h"

Node::Node() : Entity()
{
	color = 0;
}

Node::Node(Vec3 pos, int meshID, int textureID, float hp, int lightID, int color) 
	: Entity(Vec3(pos.x, pos.y, pos.z), meshID, textureID, hp, lightID)
{
	this->color = color;

	scaleFactor = 0.4f;
	D3DXMatrixScaling(&scale, scaleFactor, scaleFactor, scaleFactor);
	D3DXMatrixTranslation(&pointTranslate, -2.85f, 0, -1.15f);

	rotationSpeed = (float)(rand() % 100 + 1) * 0.001f;
	rotY = 0.0f;

	translateSpeed = 0.1f;

	int temp = rand() % 2;
	if(temp)
		upTranslate = true;
	else
		upTranslate = false;

	this->pos = pos;
	translatePosMax = 0.5f;
}

Node::~Node()
{}

int Node::update(float dt)
{
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