#ifndef SUPPLY_H
#define SUPPLY_H

#include "Structure.h"

class Supply : public Structure
{
public:
	Supply();
	virtual ~Supply();

	virtual int update(float dt);


private:


};

#endif