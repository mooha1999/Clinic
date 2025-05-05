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
// #include "UI.h"

#include <string>
#include <fstream>

class UI;
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
	UI *ui;

	int pCancel, pReschedule;
	int patientCount;

	int timestep;

	void populateDeviceList(int deviceCount, LinkedQueue<Resource *> *deviceList, ResourceType deviceType, int startingIndex = 0);
	void handleNormalPatientToWaitingList(Treatment *treatment, Patient *&patient);
	// Checks if there are patients in the early/late lists and moves them to wait lists
	void moveToWaitLists(int timestep);
	void waitingToTreatment(LinkedQueue<Patient *> *waitList, int timestep);
	void handleCurrentPatient(Patient *currentPatient, int timestep);
	void callTreatmentsMoveToWait(int timestep);
	void releaseResource(Treatment *treatment);
	LinkedQueue<Patient *> *getLeastLatencyWaitList();

public:
	Scheduler();
	void readInputFile(string fileName);
	void simulate();
	bool isElectromagneticAvailable();
	bool isUltrasonicAvailable();
	bool isExerciseRoomAvailable();
	void moveToFinishedPatients(Patient *patient, int timestep);
	void movePateintToWaitList(Patient *patient, int timestep);
	void moveToWaitingOrFinished(Patient *patient, int timestep);
	int getTimestep();
	void handleFinishedTreatmentPatient(int timestep);
};
