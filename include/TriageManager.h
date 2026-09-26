#ifndef TRIAGE_MANAGER_H
#define TRIAGE_MANAGER_H

#include "Patient.h"

#include <queue>
#include <string>
#include <vector>

using namespace std;

class TriageManager {
private:

    struct EmergencyComparator {

        bool operator()(
            const Patient* a,
            const Patient* b
        ) const;
    };

    priority_queue<
        Patient*,
        vector<Patient*>,
        EmergencyComparator
    > emergencyQueue;

    queue<Patient*> normalQueue;

public:

    // QUEUE OPERATIONS

    void addPatient(
        Patient* patient
    );

    Patient* getNextPatient();

    Patient* peekNextPatient() const;

    bool removePatient(
        const string& patientId
    );

    // QUEUE STATE

    bool empty() const;

    int getEmergencyCount() const;

    int getNormalCount() const;

    int getTotalWaitingPatients() const;

    void clearQueues();
    // DISPLAY
    void displayWaitingPatients() const;
};

#endif