#include "UI.h"

void UI::chooseMode()
{
  cout << "Choose mode: 1 for interactive, 2 for silent" << endl;
  cin >> mode;
  if (mode != 1 && mode != 2)
  {
    cout << "Invalid mode, defaulting to interactive" << endl;
    mode = 1;
  }
}

void UI::printData(
    int timestep,
    LinkedQueue<Patient *> allPatients,
    priQueue<Patient *> latePatients,
    LinkedQueue<Resource *> electromagneticDevices,
    LinkedQueue<Resource *> ultrasonicDevices,
    LinkedQueue<Resource *> exerciseRooms,
    priQueue<Patient *> inTreatmentPatients,
    ArrayStack<Patient *> finishedPatients,
    priQueue<Patient *> earlyPatients,
    LinkedQueue<Patient *> electromagneticWaitList,
    LinkedQueue<Patient *> ultrasonicWaitList,
    LinkedQueue<Patient *> exerciseWaitList)
{
  if (mode == 2)
  {
    return; // Silent mode, do not print anything
  }

  cout << "Current timestep: " << timestep << endl;

  cout << "============================All Patients============================" << endl;
  // print only the ids of 10 patients
  cout << allPatients.getSize() << " patients remaining: ";
  int mini = allPatients.getSize() < 10 ? allPatients.getSize() : 10;
  for (int i = 0; i < mini; i++)
  {
    Patient *p = nullptr;
    allPatients.dequeue(p);
    cout << 'P' << p->getId() << '_' << p->getArrivalTime() << ", ";
  }
  cout << endl;

  cout << "============================Waiting Patients============================" << endl;
  int electromagneticWaitListSize = electromagneticWaitList.getSize();
  cout << electromagneticWaitListSize << " E-therapy patients: ";
  // print all of their ids
  for (int i = 0; i < electromagneticWaitListSize; i++)
  {
    Patient *p = nullptr;
    electromagneticWaitList.dequeue(p);
    cout << p->getId() << ", ";
  }
  cout << endl;

  int ultrasonicWaitListSize = ultrasonicWaitList.getSize();
  cout << ultrasonicWaitListSize << " U-therapy patients: ";
  // print all of their ids
  for (int i = 0; i < ultrasonicWaitListSize; i++)
  {
    Patient *p = nullptr;
    ultrasonicWaitList.dequeue(p);
    cout << p->getId() << ", ";
  }
  cout << endl;

  int exerciseWaitListSize = exerciseWaitList.getSize();
  cout << exerciseWaitListSize << " X-therapy patients: ";
  // print all of their ids
  for (int i = 0; i < exerciseWaitListSize; i++)
  {
    Patient *p = nullptr;
    exerciseWaitList.dequeue(p);
    cout << p->getId() << ", ";
  }
  cout << endl;

  cout << "============================Early Patients============================" << endl;
  int earlyPatientsSize = earlyPatients.getSize();
  cout << earlyPatientsSize << " patients: ";
  // print all of their ids
  for (int i = 0; i < earlyPatientsSize; i++)
  {
    Patient *p = nullptr;
    int priority;
    earlyPatients.dequeue(p, priority);
    cout << p->getId() << ", ";
  }
  cout << endl;

  cout << "============================Late Patients============================" << endl;
  int latePatientsSize = latePatients.getSize();
  cout << latePatientsSize << " patients: ";
  // print all of their ids
  for (int i = 0; i < latePatientsSize; i++)
  {
    Patient *p = nullptr;
    int priority;
    latePatients.dequeue(p, priority);
    cout << p->getId() << ", ";
  }
  cout << endl;

  cout << "============================Available E-Devices============================" << endl;
  int electromagneticDevicesSize = electromagneticDevices.getSize();
  cout << electromagneticDevicesSize << " Electromagnetic devices: ";
  // print all of their ids
  for (int i = 0; i < electromagneticDevicesSize; i++)
  {
    Resource *r = nullptr;
    electromagneticDevices.dequeue(r);
    cout << r->getId() << ", ";
  }
  cout << endl;

  cout << "============================Available U-Devices============================" << endl;
  int ultrasonicDevicesSize = ultrasonicDevices.getSize();
  cout << ultrasonicDevicesSize << " Ultrasonic devices: ";
  // print all of their ids
  for (int i = 0; i < ultrasonicDevicesSize; i++)
  {
    Resource *r = nullptr;
    ultrasonicDevices.dequeue(r);
    cout << r->getId() << ", ";
  }
  cout << endl;

  cout << "============================Available X-Rooms============================" << endl;
  int exerciseRoomsSize = exerciseRooms.getSize();
  cout << exerciseRoomsSize << " rooms: ";
  // print all of their ids
  for (int i = 0; i < exerciseRoomsSize; i++)
  {
    Resource *r = nullptr;
    exerciseRooms.dequeue(r);
    cout << 'R' << r->getId() << '[' << r->getCurrentCapacity() << ", " << r->getCapacity() << "], ";
  }
  cout << endl;

  cout << "============================In Treatment Patients============================" << endl;
  int inTreatmentPatientsSize = inTreatmentPatients.getSize();
  cout << inTreatmentPatientsSize << " patients: ";
  // print all of their ids
  for (int i = 0; i < inTreatmentPatientsSize; i++)
  {
    Patient *p = nullptr;
    int priority;
    inTreatmentPatients.dequeue(p, priority);
    cout << p->getId() << ", ";
  }
  cout << endl;

  cout << "============================Finished Patients============================" << endl;
  int finishedPatientsSize = finishedPatients.getSize();
  cout << finishedPatientsSize << " patients: ";
  // print all of their ids
  for (int i = 0; i < finishedPatientsSize; i++)
  {
    Patient *p = nullptr;
    finishedPatients.pop(p);
    cout << p->getId() << ", ";
  }
  cout << endl;

  // Allow the user to press enter to continue
  cout << "Press enter to continue..." << endl;
  // cin.ignore();
  cin.get();
  cout << endl;
}

void UI::printEndMessage()
{
  cout << "Simulation ends, Output file created" << endl;
}
