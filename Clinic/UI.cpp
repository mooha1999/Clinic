#include "UI.h"

void UI::printData(
    int timestep,
    LinkedQueue<Patient *> allPatients,
    priQueue<Patient *> latePatients,
    LinkedQueue<Resource *> electromagneticDevices,
    LinkedQueue<Resource *> ultrasonicDevices,
    LinkedQueue<Resource *> exerciseRooms,
    priQueue<Patient *> inTreatmentPatients,
    ArrayStack<Patient *> finishedPatients,
    EarlyPList earlyPatients,
    EU_WaitList electromagneticWaitList,
    EU_WaitList ultrasonicWaitList,
    X_WaitList exerciseWaitList)
{

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
  cout << electromagneticWaitList.getSize() << " E-therapy patients: ";
  // print all of their ids
  for (int i = 0; i < electromagneticWaitList.getSize(); i++)
  {
    Patient *p = nullptr;
    electromagneticWaitList.dequeue(p);
    cout << p->getId() << ", ";
  }
  cout << endl;
  cout << ultrasonicWaitList.getSize() << " U-therapy patients: ";
  // print all of their ids
  for (int i = 0; i < ultrasonicWaitList.getSize(); i++)
  {
    Patient *p = nullptr;
    ultrasonicWaitList.dequeue(p);
    cout << p->getId() << ", ";
  }
  cout << endl;
  cout << exerciseWaitList.getSize() << " X-therapy patients: ";
  // print all of their ids
  for (int i = 0; i < exerciseWaitList.getSize(); i++)
  {
    Patient *p = nullptr;
    exerciseWaitList.dequeue(p);
    cout << p->getId() << ", ";
  }
  cout << endl;

  cout << "============================Early Patients============================" << endl;
  cout << earlyPatients.getSize() << " patients: ";
  // print all of their ids
  for (int i = 0; i < earlyPatients.getSize(); i++)
  {
    Patient *p = nullptr;
    int priority;
    earlyPatients.dequeue(p, priority);

    cout << p->getId() << ", ";
  }
  cout << endl;

  cout << "============================Late Patients============================" << endl;
  cout << latePatients.getSize() << " patients: ";
  // print all of their ids
  for (int i = 0; i < latePatients.getSize(); i++)
  {
    Patient *p = nullptr;
    int priority;
    latePatients.dequeue(p, priority);
    cout << p->getId() << ", ";
  }
  cout << endl;

  cout << "============================Available E-Devices============================" << endl;
  cout << electromagneticDevices.getSize() << " Electromagnetic devices: ";
  // print all of their ids
  for (int i = 0; i < electromagneticDevices.getSize(); i++)
  {
    Resource *r = nullptr;
    electromagneticDevices.dequeue(r);
    cout << r->getId() << ", ";
  }
  cout << endl;
  cout << "============================Available U-Devices============================" << endl;
  cout << ultrasonicDevices.getSize() << " Ultrasonic devices: ";
  // print all of their ids
  for (int i = 0; i < ultrasonicDevices.getSize(); i++)
  {
    Resource *r = nullptr;
    ultrasonicDevices.dequeue(r);
    cout << r->getId() << ", ";
  }
  cout << endl;
  cout << "============================Available X-Rooms============================" << endl;
  cout << exerciseRooms.getSize() << " rooms: ";
  // print all of their ids
  for (int i = 0; i < exerciseRooms.getSize(); i++)
  {
    Resource *r = nullptr;
    exerciseRooms.dequeue(r);
    cout << r->getId() << ", ";
  }
  cout << endl;

  cout << "============================In Treatment Patients============================" << endl;
  cout << inTreatmentPatients.getSize() << " patients: ";
  // print all of their ids
  for (int i = 0; i < inTreatmentPatients.getSize(); i++)
  {
    Patient *p = nullptr;
    int priority;
    inTreatmentPatients.dequeue(p, priority);
    cout << p->getId() << ", ";
  }
  cout << endl;

  cout << "============================Finished Patients============================" << endl;
  cout << finishedPatients.getSize() << " patients: ";
  // print all of their ids
  for (int i = 0; i < finishedPatients.getSize(); i++)
  {
    Patient *p = nullptr;
    finishedPatients.pop(p);
    cout << p->getId() << ", ";
  }
  cout << endl;

  // Allow the user to press enter to continue
  cout << "Press enter to continue..." << endl;
  cin.ignore();
  cin.get();
  cout << endl;
}
