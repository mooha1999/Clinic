#pragma once
#include "priQueue.h"
#include "Patient.h"
class EarlyPList :
	public priQueue<Patient*>
{
public:
	void reschedule(int time);
};

