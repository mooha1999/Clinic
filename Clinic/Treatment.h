#pragma once
#include "Resource.h"

class Treatment
{
protected:
	Resource* resource;
	int duration, assignmentTime;
public:
	Treatment(int duration) : duration(duration), assignmentTime(-1), resource(nullptr) {}
	void setResource(Resource* resource);
	void setAssignmentTime(int time);
	int getAssignmentTime();
	int getDuration();
	virtual bool canAssign() = 0;
	virtual void moveToWait() = 0;
};

