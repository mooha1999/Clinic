#include "RoomTreatment.h"

bool RoomTreatment::canAssign(Scheduler *scheduler)
{
    return scheduler->isExerciseRoomAvailable();
}

void RoomTreatment::moveToWait(Scheduler *scheduler)
{
    scheduler->handleFinishedTreatmentPatient(scheduler->getTimestep());
}
