#pragma once
#include "Treatment.h"
class RoomTreatment :
    public Treatment
{
public:
	RoomTreatment(int duration) :
		Treatment(duration)
	{
	}

	virtual bool canAssign();

	virtual void moveToWait();
};

