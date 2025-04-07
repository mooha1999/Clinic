#pragma once
#include "Treatment.h"
class ElectroTreatment :
    public Treatment
{
public:
	ElectroTreatment(int duration) :
		Treatment(duration)
	{
	}

	virtual bool canAssign();

	virtual void moveToWait();
};

