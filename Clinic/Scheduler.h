#pragma once
#include "Patient.h"
#include "Treatment.h"
#include "ElectroTreatment.h"
#include "UltrasoundTreatment.h"
#include "RoomTreatment.h"
#include "Resource.h"
#include "LinkedQueue.h"
#include "priQueue.h"
#include "ArrayStack.h"
#include "X_WaitList.h"
#include "EarlyPList.h"
#include "UI.h"

#include <string>
#include <fstream>
using namespace std;

class Scheduler
{
private:
	LinkedQueue<Patient *> *allPatients;
	priQueue<Patient *> *latePatients;
	LinkedQueue<Resource *> *electromagneticDevices,
			*ultrasonicDevices, *exerciseRooms;
	priQueue<Patient *> *inTreatmentPatients;
	ArrayStack<Patient *> *finishedPatients;
	EarlyPList *earlyPatients;
	EU_WaitList *electromagneticWaitList,
			*ultrasonicWaitList;
	X_WaitList *exerciseWaitList;

	int pCancel, pReschedule;
	int patientCount;

	void populateDeviceList(int deviceCount, LinkedQueue<Resource *> *deviceList, ResourceType deviceType, int startingIndex = 0);
	void randomWaitingAssignment(Patient *patient);
	EU_WaitList *getRandomWaitList();
	void randomGenerator(int timestep);
	int randomNewappointmentTime(int oldAppointmentTime);
	void handleCurrentPatient(Patient *currentPatient, int timestep);

public:
	Scheduler();
	void readInputFile(string fileName);
	void simulate();
};
