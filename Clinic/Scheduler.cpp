#include "Scheduler.h"

void Scheduler::readInputFile(string fileName)
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
    exerciseRooms->enqueue(room);
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

void Scheduler::populateDeviceList(int deviceCount, LinkedQueue<Resource *> *deviceList, ResourceType deviceType, int startingIndex)
{
  for (int i = 0; i < deviceCount; ++i)
  {
    Resource *resource = new Resource(startingIndex + i, deviceType);
    deviceList->enqueue(resource);
  }
}

void Scheduler::randomWaitingAssignment(Patient *patient)
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

EU_WaitList *Scheduler::getRandomWaitList()
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

void Scheduler::randomGenerator(int timestep)
{
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
      nextPatient->setFinishTime(timestep);
      finishedPatients->push(nextPatient);
    }
  }
}

int Scheduler::randomNewappointmentTime(int oldAppointmentTime)
{
  int randomNum = rand() % 30 + 5;
  int newAppointmentTime = oldAppointmentTime + randomNum;
  return newAppointmentTime;
}

void Scheduler::handleCurrentPatient(Patient *currentPatient, int timestep)
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

Scheduler::Scheduler()
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

void Scheduler::simulate()
{
  int timestep = 0;
  while (finishedPatients->getSize() != patientCount)
  {
    Patient *currentPatient = nullptr;
    allPatients->peek(currentPatient);
    if (currentPatient != nullptr)
    {
      handleCurrentPatient(currentPatient, timestep);
    }
    randomGenerator(timestep);
    // create clones for each list
    LinkedQueue<Patient *> allPatientsClone = allPatients->clone();
    priQueue<Patient *> latePatientsClone = latePatients->clone();
    LinkedQueue<Resource *> electromagneticDevicesClone = electromagneticDevices->clone();
    LinkedQueue<Resource *> ultrasonicDevicesClone = ultrasonicDevices->clone();
    LinkedQueue<Resource *> exerciseRoomsClone = exerciseRooms->clone();
    priQueue<Patient *> inTreatmentPatientsClone = inTreatmentPatients->clone();
    ArrayStack<Patient *> finishedPatientsClone = finishedPatients->clone();
    priQueue<Patient *> earlyPatientsClone = earlyPatients->clone();
    LinkedQueue<Patient *> electromagneticWaitListClone = electromagneticWaitList->clone();
    LinkedQueue<Patient *> ultrasonicWaitListClone = ultrasonicWaitList->clone();
    LinkedQueue<Patient *> exerciseWaitListClone = exerciseWaitList->clone();

    ui.printData(timestep,
                 allPatientsClone,
                 latePatientsClone,
                 electromagneticDevicesClone,
                 ultrasonicDevicesClone,
                 exerciseRoomsClone,
                 inTreatmentPatientsClone,
                 finishedPatientsClone,
                 earlyPatientsClone,
                 electromagneticWaitListClone,
                 ultrasonicWaitListClone,
                 exerciseWaitListClone);

    timestep++;
  }
}