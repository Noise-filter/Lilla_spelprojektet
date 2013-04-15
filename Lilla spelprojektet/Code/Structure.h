#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "Entity.h"

class Structure : public Entity
{
public:
	Structure();
	virtual ~Structure();

	virtual int update(float dt);


private:


};

#endif