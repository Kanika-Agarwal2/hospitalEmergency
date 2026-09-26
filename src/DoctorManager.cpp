#include "DoctorManager.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cctype>

using namespace std;

// ADD DOCTOR

bool DoctorManager::addDoctor(
    const Doctor& doctor
) {

    string doctorId =
        doctor.getDoctorId();

    if (doctorId.empty()) {
        return false;
    }

    auto result =
        doctors.emplace(
            doctorId,
            doctor
        );

    return result.second;
}

// REMOVE DOCTOR

bool DoctorManager::removeDoctor(const string& doctorId) {
    auto iterator =
        doctors.find(doctorId);

    if (iterator == doctors.end()) {
        return false;
    }

    // Busy doctors cannot be removed.
    if (!iterator->second.isAvailable()) {
        return false;
    }

    doctors.erase(iterator);

    return true;
}


// FIND DOCTOR

Doctor* DoctorManager::findDoctor(const string& doctorId) {

    auto iterator = doctors.find(doctorId);

    if (iterator == doctors.end()) {
        return nullptr;
    }

    return &iterator->second;
}


// FIND AVAILABLE DOCTOR BY SPECIALIZATION

Doctor* DoctorManager::findAvailableDoctor(const string& specialization) {
    string requested =
        specialization;

    transform(
        requested.begin(),
        requested.end(),
        requested.begin(),
        [](unsigned char c) {
            return static_cast<char>(tolower(c));
        }
    );

    for (auto& entry : doctors) {
        Doctor& doctor = entry.second;

        string doctorSpecialization =
            doctor.getSpecialization();

        transform(
            doctorSpecialization.begin(),
            doctorSpecialization.end(),
            doctorSpecialization.begin(),
            [](unsigned char c) {
                return static_cast<char>(
                    tolower(c)
                );
            }
        );

        if (doctor.isAvailable() && doctorSpecialization == requested) {
            return &doctor;
        }
    }

    return NULL;
}


// ASSIGN PATIENT TO DOCTOR


bool DoctorManager::assignPatientToDoctor(const string& doctorId,const string& patientId) {
    Doctor* doctor =
        findDoctor(doctorId);

    if (doctor == nullptr) {
        return false;
    }

    if (!doctor->isAvailable()) {
        return false;
    }

    doctor->assignPatient(
        patientId
    );

    return true;
}

// RELEASE DOCTOR

bool DoctorManager::releaseDoctor(const string& doctorId) {

    Doctor* doctor =
        findDoctor(doctorId);

    if (doctor == nullptr) {
        return false;
    }

    doctor->releasePatient();

    return true;
}

vector<Doctor> DoctorManager::getAllDoctors() const {

    vector<Doctor> doctorList;

    doctorList.reserve(doctors.size());

    for (const auto& entry : doctors) {
        doctorList.push_back(
            entry.second
        );
    }

    return doctorList;
}
// RESTORE DOCTOR STATE
bool DoctorManager::restoreDoctorState(const string& doctorId,const string& patientId) {

    Doctor* doctor =
        findDoctor(doctorId);

    if (doctor == nullptr) {
        return false;
    }

    if (patientId.empty()) {
        return false;
    }

    if (!doctor->isAvailable()) {
        return false;
    }

    doctor->assignPatient(
        patientId
    );

    return true;
}

// CLEAR ALL DOCTORS
void DoctorManager::clearDoctors() {

    doctors.clear();
}

// TOTAL DOCTORS

int DoctorManager::getTotalDoctors() const {
    return static_cast<int>(
        doctors.size()
    );
}


// AVAILABLE DOCTORS

int DoctorManager::getAvailableDoctors() const {
    int count = 0;
    for (const auto& entry : doctors) {

        if (entry.second.isAvailable()) {
            count++;
        }
    }

    return count;
}

// BUSY DOCTORS


int DoctorManager::getBusyDoctors() const {

    return
        getTotalDoctors()
        -
        getAvailableDoctors();
}


// DISPLAY DOCTORS

void DoctorManager::displayDoctors() const {
    cout << "\n";
    cout << "============================================================\n";
    cout << "                     DOCTOR DIRECTORY\n";
    cout << "============================================================\n";

    if (doctors.empty()) {

        cout << "No doctors registered.\n";

        return;
    }

    cout << left
         << setw(12) << "ID"
         << setw(22) << "Name"
         << setw(20) << "Specialization"
         << setw(15) << "Status"
         << setw(15) << "Patient"
         << endl;

    cout << "------------------------------------------------------------\n";

    for (const auto& entry : doctors) {
        const Doctor& doctor =
            entry.second;

        string status =
            doctor.isAvailable()
                ? "AVAILABLE"
                : "BUSY";

        string patient =
            doctor.isAvailable()
                ? "-"
                : doctor.getCurrentPatientId();

        cout << left
             << setw(12)
             << doctor.getDoctorId()

             << setw(22)
             << doctor.getName()

             << setw(20)
             << doctor.getSpecialization()

             << setw(15)
             << status

             << setw(15)
             << patient
             << endl;
    }

    cout << "------------------------------------------------------------\n";

    cout << "Total Doctors     : "
         << getTotalDoctors()
         << endl;

    cout << "Available Doctors : "
         << getAvailableDoctors()
         << endl;

    cout << "Busy Doctors      : "
         << getBusyDoctors()
         << endl;

    cout << "============================================================\n";
}