#pragma once
#include "Treatment.h"
#include "LinkedQueue.h"
class Patient
{
public:
	enum Type
	{
		NORMAL,
		RECOVERING,
	};
	enum Status
	{
		IDLE,
		ERLY,
		WAIT,
		SERV,
		FNSH
	};

private:
	int id, appointmentTime, arrivalTime, assignmentTime;
	int finishTime;
	int waitingTime;
	int treatmentTime;
	Type type;
	Status status;
	LinkedQueue<Treatment *> *treatments;

public:
	Patient(int id, int appointmentTime, int arrivalTime, Type type, LinkedQueue<Treatment *> *treatments);
	int getId();
	int getAppointmentTime();
	int getArrivalTime();
	int getAssignmentTime();
	int getWaitingTime();
	int getTreatmentTime();
	Type getType();
	Status getStatus();
	LinkedQueue<Treatment *> *getTreatments();
	int getFinishTime();
	void setFinishTime(int finishTime);
	void setStatus(Status status);
	void setAssignmentTime(int assignmentTime);
	void setAppointmentTime(int appointmentTime);
	// remove the first treatment if it exists
	void removeTreatment();
	bool hasTreatment();
	bool isEarly();
	bool isWaiting();
	bool isServing();
	bool isFinished();
	void addToWaitingTime(int amount);
};
