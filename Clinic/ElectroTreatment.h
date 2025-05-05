#pragma once
#include "Treatment.h"
#include "Scheduler.h"
class ElectroTreatment : public Treatment
{
public:
	ElectroTreatment(int duration) : Treatment(duration)
	{
	}

	virtual bool canAssign(Scheduler *scheduler);

	virtual void moveToWait(Scheduler *scheduler);
};
