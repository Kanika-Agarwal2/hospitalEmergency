#ifndef DOCTOR_MANAGER_H
#define DOCTOR_MANAGER_H

#include "Doctor.h"
#include <unordered_map>
#include <vector>
#include <string>
using namespace std;
class DoctorManager {
private:
    unordered_map<string, Doctor> doctors;
public:
    // DOCTOR CRUD
    bool addDoctor(
        const Doctor& doctor
    );
    bool removeDoctor(
        const string& doctorId
    );

    Doctor* findDoctor(
        const string& doctorId
    );

    Doctor* findAvailableDoctor(
        const string& specialization
    );

    // PATIENT ASSIGNMENT
    bool assignPatientToDoctor(
        const string& doctorId,
        const string& patientId
    );

    bool releaseDoctor(
        const string& doctorId
    );

    // PERSISTENCE / RESTORATION
    vector<Doctor> getAllDoctors() const;

    bool restoreDoctorState(
        const string& doctorId,
        const string& patientId
    );
    void clearDoctors();

    // STATISTICS
    int getTotalDoctors() const;

    int getAvailableDoctors() const;

    int getBusyDoctors() const;

    // DISPLAY
    void displayDoctors() const;
};

#endif