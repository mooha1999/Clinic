#include "Scheduler.h"
#include "UI.h"

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

void Scheduler::handleCurrentPatient(Patient *currentPatient, int timestep)
{
  if (currentPatient->getArrivalTime() != timestep)
  {
    return;
  }
  allPatients->dequeue(currentPatient);
  if (currentPatient->getAppointmentTime() < timestep)
  {
    int penalty = timestep + (timestep - currentPatient->getAppointmentTime());
    latePatients->enqueue(currentPatient, penalty * -1);
  }
  else if (currentPatient->getAppointmentTime() > timestep)
  {
    earlyPatients->enqueue(currentPatient, currentPatient->getAppointmentTime() * -1);
  }
  else
  {
    movePateintToWaitList(currentPatient, timestep);
  }
}
void Scheduler::callTreatmentsMoveToWait(int timestep)
{
  // loop through the inTreatmentPatients queue and check if any patients have finished their treatment using the priority as the time to finish
  // if so, check if they have any treatments left
  // if they do, move them to the waiting list
  // if they don't, move them to the finished patients stack
  Patient *patient = nullptr;
  int timeToFinish = 0;
  bool isPatient = inTreatmentPatients->peek(patient, timeToFinish);
  timeToFinish *= -1;
  while (isPatient && timeToFinish == timestep)
  {
    inTreatmentPatients->peek(patient, timeToFinish);
    Treatment *treatment = nullptr;
    patient->getTreatments()->peek(treatment);
    patient->removeTreatment();
    treatment->moveToWait(this);

    releaseResource(treatment);

    isPatient = inTreatmentPatients->peek(patient, timeToFinish);
    timeToFinish *= -1;
  }
}

void Scheduler::releaseResource(Treatment *treatment)
{
  Resource *resource = treatment->getResource();
  if (dynamic_cast<ElectroTreatment *>(treatment) != nullptr)
  {
    electromagneticDevices->enqueue(resource);
  }
  else if (dynamic_cast<UltrasoundTreatment *>(treatment) != nullptr)
  {
    ultrasonicDevices->enqueue(resource);
  }
  else if (dynamic_cast<RoomTreatment *>(treatment) != nullptr)
  {
    if (!resource->isAvailable()) // check if the room was full
    {
      resource->decrementCurrentCapacity(); // decrement the current capacity as the room is not full anymore
      exerciseRooms->enqueue(resource);     // add the room back to the list
    }
    else
    { // if the room was not full, then it is already in the list
      resource->decrementCurrentCapacity();
    }
    treatment->setResource(nullptr); // set the resource to null as it is not in use anymore
  }
}

void Scheduler::handleFinishedTreatmentPatient(int timestep)
{
  // Check if there are any patients in treatment and if their treatment is finished
  // If so, move them to the waiting list or finished patients list
  Patient *patient = nullptr;
  int timeToFinish = 0;
  bool isPatient = inTreatmentPatients->peek(patient, timeToFinish);
  timeToFinish *= -1;
  if (isPatient && timeToFinish == timestep)
  {
    inTreatmentPatients->dequeue(patient, timeToFinish);
    moveToWaitingOrFinished(patient, timestep);
  }
}

LinkedQueue<Patient *> *Scheduler::getLeastLatencyWaitList()
{
  LinkedQueue<Patient *> *leastLatencyWaitList = nullptr;
  int minLatency = INT_MAX;
  if (electromagneticWaitList->getSize() < minLatency)
  {
    minLatency = electromagneticWaitList->getSize();
    leastLatencyWaitList = electromagneticWaitList;
  }
  if (ultrasonicWaitList->getSize() < minLatency)
  {
    minLatency = ultrasonicWaitList->getSize();
    leastLatencyWaitList = ultrasonicWaitList;
  }
  if (exerciseWaitList->getSize() < minLatency)
  {
    minLatency = exerciseWaitList->getSize();
    leastLatencyWaitList = exerciseWaitList;
  }
  return leastLatencyWaitList;
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
  ui = new UI();
  pCancel = pReschedule = patientCount = -1;
  timestep = 0;
}

void Scheduler::simulate()
{
  timestep = 0;
  while (finishedPatients->getSize() != patientCount)
  {
    Patient *currentPatient = nullptr;
    allPatients->peek(currentPatient);
    if (currentPatient != nullptr)
    {
      handleCurrentPatient(currentPatient, timestep);
    }
    moveToWaitLists(timestep);
    waitingToTreatment(electromagneticWaitList, timestep);
    waitingToTreatment(ultrasonicWaitList, timestep);
    waitingToTreatment(exerciseWaitList, timestep);
    callTreatmentsMoveToWait(timestep);

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

    ui->printData(timestep,
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

bool Scheduler::isElectromagneticAvailable()
{
  Resource *resource = nullptr;
  return electromagneticDevices->peek(resource);
}

bool Scheduler::isUltrasonicAvailable()
{
  Resource *resource = nullptr;
  return ultrasonicDevices->peek(resource);
}

bool Scheduler::isExerciseRoomAvailable()
{
  Resource *resource = nullptr;
  bool isAvailable = exerciseRooms->peek(resource);
  if (!isAvailable)
  {
    return false;
  }
  return resource->isAvailable();
}

void Scheduler::moveToFinishedPatients(Patient *patient, int timestep)
{
  finishedPatients->push(patient);
  patient->setStatus(Patient::FNSH);
  patient->setFinishTime(timestep);
}

void Scheduler::movePateintToWaitList(Patient *patient, int timestep)
{
  LinkedQueue<Treatment *> *treatments = patient->getTreatments();
  Treatment *treatment = nullptr;
  treatments->peek(treatment);
  if (!treatment)
  {
    return;
  }
  patient->setStatus(Patient::WAIT);
  switch (patient->getType())
  {
  case Patient::NORMAL:
    handleNormalPatientToWaitingList(treatment, patient);
    break;
  case Patient::RECOVERING:
    LinkedQueue<Patient *> *waitList = getLeastLatencyWaitList();
    waitList->enqueue(patient);
    break;
  }
}

void Scheduler::moveToWaitingOrFinished(Patient *patient, int timestep)
{
  // Check if the patient has any treatments left
  if (patient->hasTreatment())
  {
    // Move the patient to the waiting list
    movePateintToWaitList(patient, timestep);
  }
  else
  {
    // Move the patient to the finished patients stack
    moveToFinishedPatients(patient, timestep);
  }
}

int Scheduler::getTimestep()
{
  return timestep;
}

void Scheduler::handleNormalPatientToWaitingList(Treatment *treatment, Patient *&patient)
{
  if (dynamic_cast<ElectroTreatment *>(treatment) != nullptr)
  {
    electromagneticWaitList->enqueue(patient);
  }
  else if (dynamic_cast<UltrasoundTreatment *>(treatment) != nullptr)
  {
    ultrasonicWaitList->enqueue(patient);
  }
  else if (dynamic_cast<RoomTreatment *>(treatment) != nullptr)
  {
    exerciseWaitList->enqueue(patient);
  }
}

void Scheduler::moveToWaitLists(int timestep)
{
  // Move patients from late and early lists to wait lists
  Patient *patient = nullptr;
  int timeToMove = 0;
  bool isPatient = latePatients->peek(patient, timeToMove);
  timeToMove *= -1;
  while (isPatient && timeToMove == timestep)
  {
    latePatients->dequeue(patient, timeToMove);
    movePateintToWaitList(patient, timestep);
    isPatient = latePatients->peek(patient, timeToMove);
    timeToMove *= -1;
  }

  isPatient = earlyPatients->peek(patient, timeToMove);
  timeToMove *= -1;
  while (isPatient && timeToMove == timestep)
  {
    earlyPatients->dequeue(patient, timeToMove);
    movePateintToWaitList(patient, timestep);
    isPatient = earlyPatients->peek(patient, timeToMove);
    timeToMove *= -1;
  }
}

void Scheduler::waitingToTreatment(LinkedQueue<Patient *> *waitList, int timestep)
{
  Patient *patient = nullptr;
  bool isPatient = waitList->peek(patient);
  while (isPatient)
  {
    Treatment *treatment = nullptr;
    patient->getTreatments()->peek(treatment);
    if (!treatment->canAssign(this))
    {
      break; // No available resources for this treatment
    }

    Resource *resource = nullptr;
    if (dynamic_cast<ElectroTreatment *>(treatment) != nullptr)
    {
      electromagneticDevices->dequeue(resource);
    }
    else if (dynamic_cast<UltrasoundTreatment *>(treatment) != nullptr)
    {
      ultrasonicDevices->dequeue(resource);
    }
    else if (dynamic_cast<RoomTreatment *>(treatment) != nullptr)
    {
      bool isAvailable = exerciseRooms->peek(resource);
      if (isAvailable)
      {
        resource->incrementCurrentCapacity();
        if (!resource->isAvailable()) // check if the room is full
        {
          exerciseRooms->dequeue(resource); // remove the room from the list as it is full
        }
      }
    }
    treatment->setResource(resource);

    waitList->dequeue(patient);
    int timeToGetOut = (timestep + treatment->getDuration()) * -1; // negative time as the priority queue sorts descendingly
    inTreatmentPatients->enqueue(patient, timeToGetOut);
    patient->setAssignmentTime(timestep);
    patient->setStatus(Patient::SERV);
    isPatient = waitList->peek(patient);
  }
}
