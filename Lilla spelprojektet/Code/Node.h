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

private:
	int color;

	/*
		min, max positioner anv�nds f�r animation att de ska �ka upp och ner.
	*/

};

#endif