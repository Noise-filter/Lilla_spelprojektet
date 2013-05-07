#ifndef NODE_H
#define NODE_H

#include "Entity.h"

class Node : public Entity
{
public:
	Node();
	Node(D3DXVECTOR3 pos, int meshID, int textureID, float hp, int lightID, int color);
	virtual ~Node();

	virtual int update(float dt);

	int getColor();

private:
	int color;
	float rotationSpeed;
	float translateSpeed;

	/*
		min, max positioner används för animation att de ska åka upp och ner.
	*/

};

#endif