#ifndef UPGRADE_H
#define UPGRADE_H

#include "Structure.h"

class Upgrade : public Structure
{
public:
	Upgrade();
	virtual ~Upgrade();

	virtual int update(float dt);


private:
	int upgradeID;

};

#endif