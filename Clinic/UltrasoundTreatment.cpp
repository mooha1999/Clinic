#include "UltrasoundTreatment.h"

bool UltrasoundTreatment::canAssign(Scheduler *scheduler)
{
	return scheduler->isUltrasonicAvailable();
}

void UltrasoundTreatment::moveToWait(Scheduler *scheduler)
{
	scheduler->handleFinishedTreatmentPatient(scheduler->getTimestep());
}
