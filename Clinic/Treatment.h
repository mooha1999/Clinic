#pragma once
#include "Resource.h"

class Scheduler; // Forward declaration of Scheduler

class Treatment
{
protected:
	Resource *resource;
	int duration, assignmentTime;

public:
	Treatment(int duration) : duration(duration), assignmentTime(-1), resource(nullptr) {}
	void setResource(Resource *resource);
	void setAssignmentTime(int time);
	int getAssignmentTime();
	int getDuration();
	Resource *getResource();
	virtual bool canAssign(Scheduler *scheduler) = 0;
	virtual void moveToWait(Scheduler *scheduler) = 0;
};
