#pragma once
#include <iostream>
#include "LinkedQueue.h"
#include "priQueue.h"
#include "ArrayStack.h"
#include "X_WaitList.h"
#include "EarlyPList.h"
#include "Resource.h"
#include "Patient.h"
#include "Treatment.h"
#include "ElectroTreatment.h"
#include "UltrasoundTreatment.h"
#include "RoomTreatment.h"
#include "Resource.h"
using namespace std;
class UI
{
public:
  void printData(int timestep, LinkedQueue<Patient *> allPatients,
                 priQueue<Patient *> latePatients,
                 LinkedQueue<Resource *> electromagneticDevices,
                 LinkedQueue<Resource *> ultrasonicDevices,
                 LinkedQueue<Resource *> exerciseRooms,
                 priQueue<Patient *> inTreatmentPatients,
                 ArrayStack<Patient *> finishedPatients,
                 EarlyPList earlyPatients,
                 EU_WaitList electromagneticWaitList,
                 EU_WaitList ultrasonicWaitList,
                 X_WaitList exerciseWaitList);
};
