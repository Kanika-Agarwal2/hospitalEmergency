#ifndef STATISTICS_MANAGER_H
#define STATISTICS_MANAGER_H

#include "Patient.h"
#include "Doctor.h"

#include <vector>

using namespace std;

class StatisticsManager {

public:

    void displayStatistics(
        const vector<Patient>& patients,
        const vector<Doctor>& doctors,
        int waitingPatients
    ) const;
};

#endif