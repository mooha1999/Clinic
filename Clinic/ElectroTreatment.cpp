#include "ElectroTreatment.h"

bool ElectroTreatment::canAssign(Scheduler *scheduler)
{
    return scheduler->isElectromagneticAvailable();
}

void ElectroTreatment::moveToWait(Scheduler *scheduler)
{
    scheduler->handleFinishedTreatmentPatient(scheduler->getTimestep());
}
