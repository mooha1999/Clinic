#include "Treatment.h"

void Treatment::setResource(Resource *resource)
{
	this->resource = resource;
}

void Treatment::setAssignmentTime(int time)
{
	assignmentTime = time;
}

int Treatment::getAssignmentTime()
{
	return assignmentTime;
}

int Treatment::getDuration()
{
	return duration;
}

Resource *Treatment::getResource()
{
	return resource;
}
