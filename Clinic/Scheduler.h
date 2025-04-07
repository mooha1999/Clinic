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

	void readInputFile(string fileName)
	{
		ifstream inputFile(fileName);
		if (!inputFile)
		{
			cerr << "Error opening file: " << fileName << endl;
			return;
		}

		int eDevices, uDevices, xRooms;
		inputFile >> eDevices >> uDevices >> xRooms;

		populateDeviceList(eDevices, electromagneticDevices, ResourceType::ELECTRO);
		populateDeviceList(uDevices, ultrasonicDevices, ResourceType::ULTRASOUND, eDevices);
		for (int i = 0; i < xRooms; ++i)
		{
			int roomCapacity;
			inputFile >> roomCapacity;
			Resource *room = new Resource(eDevices + uDevices + i, ResourceType::EXERCISE, roomCapacity);
		}

		inputFile >> pCancel >> pReschedule;

		inputFile >> patientCount;

		for (int i = 0; i < patientCount; ++i)
		{
			char patientType;
			int appointmentTime, arrivalTime, treatmentCount;
			inputFile >> patientType >> appointmentTime >> arrivalTime >> treatmentCount;
			LinkedQueue<Treatment *> *treatments = new LinkedQueue<Treatment *>();
			for (int j = 0; j < treatmentCount; ++j)
			{
				char treatmentType;
				int duration;
				inputFile >> treatmentType >> duration;
				Treatment *treatment = nullptr;
				switch (treatmentType)
				{
				case 'E':
					treatment = new RoomTreatment(duration);
					break;
				case 'U':
					treatment = new UltrasoundTreatment(duration);
					break;
				case 'X':
					treatment = new ElectroTreatment(duration);
					break;
				default:
					cerr << "Unknown treatment type: " << treatmentType << endl;
					continue;
				}
				treatments->enqueue(treatment);
			}
			Patient::Type type = (patientType == 'N') ? Patient::NORMAL : Patient::RECOVERING;
			Patient *patient = new Patient(i, appointmentTime, arrivalTime, type, treatments);
			allPatients->enqueue(patient);
		}

		inputFile.close();
	}

	void populateDeviceList(int deviceCount, LinkedQueue<Resource *> *deviceList, ResourceType deviceType, int startingIndex = 0)
	{
		for (int i = 0; i < deviceCount; ++i)
		{
			Resource *resource = new Resource(startingIndex + i, deviceType);
			deviceList->enqueue(resource);
		}
	}

	void randomWaitingAssignment(Patient *patient)
	{
		int randomNum = rand() % 100 + 1;
		if (randomNum < 33)
		{
			electromagneticWaitList->enqueue(patient);
		}
		else if (randomNum < 66)
		{
			ultrasonicWaitList->enqueue(patient);
		}
		else
		{
			exerciseWaitList->enqueue(patient);
		}
	}

	EU_WaitList *getRandomWaitList()
	{
		int randomNum = rand() % 100 + 1;
		if (randomNum < 33)
		{
			return electromagneticWaitList;
		}
		else if (randomNum < 66)
		{
			return ultrasonicWaitList;
		}
		else
		{
			return exerciseWaitList;
		}
	}

	void randomGenerator(int timestep)
	{
		/*
		Generate a random number X from 0 to 100 then:

		i. If X < 10, move next patient from Early to a RandomWaiting list.
		ii. If 10 ≤ X < 20, move next patient from Late to a RandomWaiting list and insert him using PT+penalty time.
		iii. If 20 ≤ X < 40, move 2 next patients from a RandomWaiting to In-treatment list.
		iv. If 40 ≤ X < 50, move next patient from In-treatment to a RandomWaiting list.
		v. If 50 ≤ X < 60, move next patient from In-treatment to Finish.
		vi. If 60 ≤ X < 70, move random patient from X-Waiting to Finish, to simulate the cancel process.
		vii. If 70 ≤ X < 80, choose random patient from Early list to the appropriate list to perform an accepted reschedule process.
		*/
		int randomNum = rand() % 100 + 1;
		Patient *nextPatient = nullptr;
		int priority;
		if (randomNum < 10)
		{
			earlyPatients->dequeue(nextPatient, priority);
			if (nextPatient != nullptr)
			{
				randomWaitingAssignment(nextPatient);
			}
		}
		else if (randomNum < 20)
		{
			latePatients->dequeue(nextPatient, priority);
			if (nextPatient != nullptr)
			{
				priority *= -1;
				priority = (priority - nextPatient->getAppointmentTime()) + nextPatient->getArrivalTime();
				randomWaitingAssignment(nextPatient);
			}
		}
		else if (randomNum < 40)
		{
			EU_WaitList *waitList = getRandomWaitList();
			waitList->dequeue(nextPatient);
			if (nextPatient != nullptr)
			{
				inTreatmentPatients->enqueue(nextPatient, nextPatient->getAppointmentTime());
				nextPatient->setAssignmentTime(timestep);
				nextPatient->setStatus(Patient::SERV);

				waitList->dequeue(nextPatient);
				if (nextPatient != nullptr)
				{
					inTreatmentPatients->enqueue(nextPatient, nextPatient->getAppointmentTime());
					nextPatient->setAssignmentTime(timestep);
					nextPatient->setStatus(Patient::SERV);
				}
			}
		}
		else if (randomNum < 50)
		{
			inTreatmentPatients->dequeue(nextPatient, priority);
			if (nextPatient != nullptr)
			{
				randomWaitingAssignment(nextPatient);
			}
		}
		else if (randomNum < 60)
		{
			inTreatmentPatients->dequeue(nextPatient, priority);
			if (nextPatient != nullptr)
			{
				nextPatient->setStatus(Patient::FNSH);
				nextPatient->setFinishTime(timestep);
				finishedPatients->push(nextPatient);
			}
		}
		else if (randomNum < 70)
		{
			exerciseWaitList->dequeue(nextPatient);
			if (nextPatient != nullptr)
			{
				nextPatient->setStatus(Patient::FNSH);
				nextPatient->setFinishTime(timestep);
				finishedPatients->push(nextPatient);
			}
		}
		else if (randomNum < 80)
		{
			earlyPatients->dequeue(nextPatient, priority);
			if (nextPatient != nullptr)
			{
				int newAppointmentTime = randomNewappointmentTime(nextPatient->getAppointmentTime());
				nextPatient->setAppointmentTime(newAppointmentTime);
				nextPatient->setStatus(Patient::IDLE);
				//! This is not correct, we need to add the patient to an appropriate list
				nextPatient->setFinishTime(timestep);
				finishedPatients->push(nextPatient);
			}
		}
	}

	int randomNewappointmentTime(int oldAppointmentTime)
	{
		int randomNum = rand() % 30 + 5;
		int newAppointmentTime = oldAppointmentTime + randomNum;
		return newAppointmentTime;
	}

	void handleCurrentPatient(Patient *currentPatient, int timestep)
	{
		if (currentPatient->getArrivalTime() != timestep)
		{
			return;
		}
		allPatients->dequeue(currentPatient);
		if (currentPatient->getAppointmentTime() < timestep)
		{
			int penalty = timestep + (timestep - currentPatient->getAppointmentTime()) / 2;
			latePatients->enqueue(currentPatient, penalty * -1);
		}
		else if (currentPatient->getAppointmentTime() > timestep)
		{
			earlyPatients->enqueue(currentPatient, currentPatient->getAppointmentTime() * -1);
		}
		else
		{
			randomWaitingAssignment(currentPatient);
		}
	}

public:
	Scheduler()
	{
		allPatients = new LinkedQueue<Patient *>();
		latePatients = new priQueue<Patient *>();
		inTreatmentPatients = new priQueue<Patient *>();
		finishedPatients = new ArrayStack<Patient *>();
		earlyPatients = new EarlyPList();
		electromagneticDevices = new LinkedQueue<Resource *>();
		ultrasonicDevices = new LinkedQueue<Resource *>();
		exerciseRooms = new LinkedQueue<Resource *>();
		electromagneticWaitList = new EU_WaitList();
		ultrasonicWaitList = new EU_WaitList();
		exerciseWaitList = new X_WaitList();
		pCancel = pReschedule = patientCount = -1;
	}

	void simulate()
	{
		int timestep = 0;
		while (!finishedPatients->getSize() == patientCount)
		{
			Patient *currentPatient = nullptr;
			allPatients->peek(currentPatient);
			if (currentPatient != nullptr)
			{
				handleCurrentPatient(currentPatient, timestep);
				allPatients->dequeue(currentPatient);
			}
			while (!allPatients->isEmpty())
				timestep++;
		}
	}
};
