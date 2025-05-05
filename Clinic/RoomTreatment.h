#pragma once
#include "Treatment.h"
#include "Scheduler.h"
class RoomTreatment : public Treatment
{
public:
	RoomTreatment(int duration) : Treatment(duration)
	{
	}

	virtual bool canAssign(Scheduler *scheduler);

	virtual void moveToWait(Scheduler *scheduler);
};
