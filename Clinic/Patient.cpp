#include "Patient.h"

Patient::Patient(int id, int appointmentTime, int arrivalTime, Type type, LinkedQueue<Treatment *> *treatments)
{
	this->treatments = treatments;
	this->id = id;
	this->appointmentTime = appointmentTime;
	this->arrivalTime = arrivalTime;
	this->type = type;
	this->status = IDLE;
	this->assignmentTime = 0;
}

int Patient::getId()
{
	return id;
}

int Patient::getAppointmentTime()
{
	return appointmentTime;
}

int Patient::getArrivalTime()
{
	return arrivalTime;
}

int Patient::getAssignmentTime()
{
	return assignmentTime;
}

Patient::Type Patient::getType()
{
	return type;
}

void Patient::setStatus(Status status)
{
	this->status = status;
}

void Patient::setAssignmentTime(int assignmentTime)
{
	this->assignmentTime = assignmentTime;
}

void Patient::setAppointmentTime(int appointmentTime)
{
	this->appointmentTime = appointmentTime;
	this->status = IDLE;
}

Patient::Status Patient::getStatus()
{
	return status;
}

LinkedQueue<Treatment *> *Patient::getTreatments()
{
	return treatments;
}

int Patient::getFinishTime()
{
	return finishTime;
}

void Patient::setFinishTime(int finishTime)
{
	this->finishTime = finishTime;
}

void Patient::removeTreatment()
{
	// remove the last treatment if it exists
}

bool Patient::hasTreatment()
{
	return true;
}

bool Patient::isEarly()
{
	return status == ERLY;
}

bool Patient::isWaiting()
{
	return status == WAIT;
}

bool Patient::isServing()
{
	return status == SERV;
}

bool Patient::isFinished()
{
	return status == FNSH;
}
