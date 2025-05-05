#pragma once
#include <iostream>
#include "LinkedQueue.h"
#include "priQueue.h"
#include "ArrayStack.h"
#include "X_WaitList.h"
#include "EarlyPList.h"
#include "Patient.h"
#include "ElectroTreatment.h"
#include "UltrasoundTreatment.h"
#include "RoomTreatment.h"
#include "Resource.h"

class Scheduler; // Forward declaration of Scheduler class

using namespace std;
class UI
{
  int mode;

public:
  void chooseMode();
  void printData(int timestep, LinkedQueue<Patient *> allPatients,
                 priQueue<Patient *> latePatients,
                 LinkedQueue<Resource *> electromagneticDevices,
                 LinkedQueue<Resource *> ultrasonicDevices,
                 LinkedQueue<Resource *> exerciseRooms,
                 priQueue<Patient *> inTreatmentPatients,
                 ArrayStack<Patient *> finishedPatients,
                 priQueue<Patient *> earlyPatients,
                 LinkedQueue<Patient *> electromagneticWaitList,
                 LinkedQueue<Patient *> ultrasonicWaitList,
                 LinkedQueue<Patient *> exerciseWaitList);

  void printEndMessage();
};
