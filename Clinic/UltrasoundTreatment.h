#pragma once
#include "Treatment.h"
#include "Scheduler.h"
class UltrasoundTreatment : public Treatment
{
public:
	UltrasoundTreatment(int duration) : Treatment(duration)
	{
	}
	virtual bool canAssign(Scheduler *scheduler);

	virtual void moveToWait(Scheduler *scheduler);
};
