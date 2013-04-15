#ifndef NODE_H
#define NODE_H

#include "Entity.h"

class Node : public Entity
{
public:
	Node();
	virtual ~Node();

	virtual int update(float dt);



private:
	int color;

	/*
		min, max positioner används för animation att de ska åka upp och ner.
	*/

};

#endif