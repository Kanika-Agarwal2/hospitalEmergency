#include "StatisticsManager.h"
#include <iostream>
#include <iomanip>
using namespace std;

void StatisticsManager::displayStatistics(const vector<Patient>& patients, const vector<Doctor>& doctors,int waitingPatients)
const {
    int totalPatients = static_cast<int>(patients.size());
    int waitingCount = 0;
    int triagedCount = 0;
    int assignedCount = 0;
    int treatmentCount = 0;
    int dischargedCount = 0;
    int emergencyCount = 0;

    long long totalWaitingTime = 0;
    int patientsWithTreatmentStart = 0;

    for (const Patient& patient : patients) {

        switch (patient.getStatus()) {
            case PatientStatus::WAITING:
                waitingCount++;
                break;

            case PatientStatus::TRIAGED:
                triagedCount++;
                break;

            case PatientStatus::DOCTOR_ASSIGNED:
                assignedCount++;
                break;

            case PatientStatus::UNDER_TREATMENT:
                treatmentCount++;
                break;

            case PatientStatus::DISCHARGED:
                dischargedCount++;
                break;
        }

        if (patient.isEmergency()) {
            emergencyCount++;
        }

        if (patient.getTreatmentStartTime().time_since_epoch().count() != 0) {
            totalWaitingTime += patient.getWaitingTimeMinutes();
            patientsWithTreatmentStart++;
        }
    }

    int totalDoctors = static_cast<int>(doctors.size());
    int availableDoctors = 0;
    int busyDoctors = 0;

    for (const Doctor& doctor : doctors) {
        if (doctor.isAvailable()) {
            availableDoctors++;
        }
        else {
            busyDoctors++;
        }
    }

    double averageWaitingTime = 0.0;

    if (patientsWithTreatmentStart > 0) {
        averageWaitingTime =
            static_cast<double>(
                totalWaitingTime
            )
            / patientsWithTreatmentStart;
    }

    cout << "\n";
    cout << "============================================================\n";
    cout << "                  HOSPITAL STATISTICS\n";
    cout << "============================================================\n";

    cout << left
         << setw(32)
         << "Total Patients"
         << ": "
         << totalPatients
         << endl;

    cout << setw(32)
         << "Emergency Patients"
         << ": "
         << emergencyCount
         << endl;

    cout << setw(32)
         << "Patients Waiting"
         << ": "
         << waitingCount
         << endl;

    cout << setw(32)
         << "Patients in Triage"
         << ": "
         << triagedCount
         << endl;

    cout << setw(32)
         << "Doctor Assigned"
         << ": "
         << assignedCount
         << endl;

    cout << setw(32)
         << "Under Treatment"
         << ": "
         << treatmentCount
         << endl;

    cout << setw(32)
         << "Discharged Patients"
         << ": "
         << dischargedCount
         << endl;

    cout << "------------------------------------------------------------\n";

    cout << setw(32)
         << "Total Doctors"
         << ": "
         << totalDoctors
         << endl;

    cout << setw(32)
         << "Available Doctors"
         << ": "
         << availableDoctors
         << endl;

    cout << setw(32)
         << "Busy Doctors"
         << ": "
         << busyDoctors
         << endl;

    cout << "------------------------------------------------------------\n";

    cout << setw(32)
         << "Waiting Queue Size"
         << ": "
         << waitingPatients
         << endl;

    cout << fixed
         << setprecision(2);
    cout << setw(32)
         << "Average Waiting Time"
         << ": "
         << averageWaitingTime
         << " minutes"
         << endl;
    cout << "============================================================\n";
}