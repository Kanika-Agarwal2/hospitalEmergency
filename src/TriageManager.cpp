#include "TriageManager.h"

#include <iostream>
#include <iomanip>

using namespace std;

// EMERGENCY PRIORITY COMPARATOR


bool TriageManager::EmergencyComparator::operator()(
    const Patient* a,
    const Patient* b
) const {

    // Higher severity gets higher priority.
    if (
        a->getSeverity()
        !=
        b->getSeverity()
    ) {

        return
            a->getSeverity()
            <
            b->getSeverity();
    }

    // If severity is equal,
    // older patient gets higher priority.
    if (
        a->getAge()
        !=
        b->getAge()
    ) {

        return
            a->getAge()
            <
            b->getAge();
    }

    // If both severity and age are equal,
    // earlier arrival gets higher priority.
    return
        a->getArrivalTime()
        >
        b->getArrivalTime();
}


// ADD PATIENT

void TriageManager::addPatient(Patient* patient) {

    if (patient == nullptr) {
        return;
    }

    if (patient->isEmergency()) {
        emergencyQueue.push(
            patient
        );

        cout << "\nPatient added to Emergency Queue.\n";

        cout << "Patient ID: "
             << patient->getPatientId()
             << endl;

        cout << "Severity : "
             << patient->getSeverity()
             << endl;

    }
    else {
        normalQueue.push(
            patient
        );

        cout << "\nPatient added to Normal Queue.\n";
        cout << "Patient ID: "
             << patient->getPatientId()
             << endl;
    }
}
// GET NEXT PATIENT


Patient* TriageManager::getNextPatient() {
    // Emergency patients always go first.
    if (!emergencyQueue.empty()) {

        Patient* patient = emergencyQueue.top();
        emergencyQueue.pop();
        return patient;
    }

    // If no emergency patient exists,
    // process normal patients FIFO.
    if (!normalQueue.empty()) {
        Patient* patient = normalQueue.front();
        normalQueue.pop();
        return patient;
    }

    return nullptr;
}


// PEEK NEXT PATIENT


Patient* TriageManager::peekNextPatient() const {

    if (!emergencyQueue.empty()) {
        return emergencyQueue.top();
    }
    if (!normalQueue.empty()) {
        return normalQueue.front();
    }
    return nullptr;
}


// REMOVE PATIENT
bool TriageManager::removePatient(
    const string& patientId
) {

    if (patientId.empty()) {
        return false;
    }

    bool found = false;
    // Rebuild emergency priority queue

    priority_queue<
        Patient*,
        vector<Patient*>,
        EmergencyComparator
    > newEmergencyQueue;

    while (!emergencyQueue.empty()) {

        Patient* patient =
            emergencyQueue.top();

        emergencyQueue.pop();

        if (
            patient->getPatientId()
            ==
            patientId
        ) {

            found = true;
        }
        else {

            newEmergencyQueue.push(
                patient
            );
        }
    }

    emergencyQueue = newEmergencyQueue;
    // Rebuild normal queue

    queue<Patient*> newNormalQueue;

    while (!normalQueue.empty()) {

        Patient* patient =
            normalQueue.front();

        normalQueue.pop();

        if (patient->getPatientId() == patientId) {
            found = true;
        }
        else {
            newNormalQueue.push(
                patient
            );
        }
    }

    normalQueue =
        newNormalQueue;

    return found;
}


// CHECK EMPTY
bool TriageManager::empty() const {

    return
        emergencyQueue.empty()
        &&
        normalQueue.empty();
}

// EMERGENCY COUNT

int TriageManager::getEmergencyCount() const {
    return static_cast<int>(
        emergencyQueue.size()
    );
}
// NORMAL COUNT
int TriageManager::getNormalCount() const {

    return static_cast<int>(
        normalQueue.size()
    );
}


// TOTAL WAITING PATIENTS


int TriageManager::getTotalWaitingPatients() const {
    return
        getEmergencyCount() + getNormalCount();
}


// CLEAR QUEUES
void TriageManager::clearQueues() {

    while (!emergencyQueue.empty()) {
        emergencyQueue.pop();
    }

    while (!normalQueue.empty()) {
        normalQueue.pop();
    }
}

// DISPLAY WAITING PATIENTS
void TriageManager::displayWaitingPatients() const {

    cout << "\n";
    cout << "============================================================\n";
    cout << "                  WAITING PATIENTS\n";
    cout << "============================================================\n";

    if (empty()) {

        cout << "No patients are currently waiting.\n";

        return;
    }

    // --------------------------------------------------------
    // Copy emergency queue so original queue remains unchanged.
    // --------------------------------------------------------

    priority_queue<
        Patient*,
        vector<Patient*>,
        EmergencyComparator
    > tempEmergency =
        emergencyQueue;

    cout << "\n------------------- EMERGENCY QUEUE ------------------------\n";

    cout << left
         << setw(12) << "ID"
         << setw(20) << "Name"
         << setw(8)  << "Age"
         << setw(10) << "Severity"
         << setw(20) << "Department"
         << endl;

    cout << "------------------------------------------------------------\n";

    while (!tempEmergency.empty()) {

        Patient* patient =
            tempEmergency.top();

        tempEmergency.pop();

        cout << left
             << setw(12)
             << patient->getPatientId()

             << setw(20)
             << patient->getName()

             << setw(8)
             << patient->getAge()

             << setw(10)
             << patient->getSeverity()

             << setw(20)
             << patient->getDepartment()

             << endl;
    }

    // Copy normal queue so original remains unchanged.

    queue<Patient*> tempNormal =
        normalQueue;

    cout << "\n--------------------- NORMAL QUEUE -------------------------\n";

    cout << left
         << setw(12) << "ID"
         << setw(20) << "Name"
         << setw(8)  << "Age"
         << setw(10) << "Severity"
         << setw(20) << "Department"
         << endl;

    cout << "------------------------------------------------------------\n";

    while (!tempNormal.empty()) {

        Patient* patient =
            tempNormal.front();

        tempNormal.pop();

        cout << left
             << setw(12)
             << patient->getPatientId()

             << setw(20)
             << patient->getName()

             << setw(8)
             << patient->getAge()

             << setw(10)
             << patient->getSeverity()

             << setw(20)
             << patient->getDepartment() << endl;
    }

    cout << "\nEmergency Patients : "
         << getEmergencyCount() << endl;

    cout << "Normal Patients    : "
         << getNormalCount() << endl;

    cout << "Total Waiting      : "
         << getTotalWaitingPatients() << endl;

    cout << "============================================================\n";
}