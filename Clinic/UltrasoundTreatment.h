#pragma once
#include "Treatment.h"
class UltrasoundTreatment :
    public Treatment
{
public:
	UltrasoundTreatment(int duration) :
		Treatment(duration)
	{
	}
	virtual bool canAssign();

	virtual void moveToWait();
};

