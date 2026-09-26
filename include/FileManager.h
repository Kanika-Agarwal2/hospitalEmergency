#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include "Patient.h"
#include "Doctor.h"
#include <string>
#include <vector>
using namespace std;
class FileManager {
private:

    string patientFile;
    string doctorFile;
    string treatmentFile;

    // SERIALIZATION

    string serializePatient(
        const Patient& patient
    ) const;

    string serializeDoctor(
        const Doctor& doctor
    ) const;

    // Convert time_point into milliseconds since epoch.
    long long timePointToEpoch(
        chrono::system_clock::time_point time
    ) const;

public:
    // CONSTRUCTOR
    FileManager(
        const string& patientFile ="data/patients.txt",

        const string& doctorFile ="data/doctors.txt",

        const string& treatmentFile ="data/treatment_history.txt"
    );

    // INITIALIZATION
    bool initializeDataFiles() const;

    // SAVE INDIVIDUAL RECORDS
    bool savePatient(
        const Patient& patient
    );

    bool saveDoctor(
        const Doctor& doctor
    );
    bool saveTreatmentRecord(
        const Patient& patient
    );

    // SAVE COMPLETE STATE
 
    bool saveAllPatients(
        const vector<Patient>& patients
    );

    bool saveAllDoctors(
        const vector<Doctor>& doctors
    );

    // LOAD RECORDS
  
    vector<string> loadPatientRecords() const;

    vector<string> loadDoctorRecords() const;

    vector<string> loadTreatmentRecords() const;
    // CLEAR DATA
    bool clearAllFiles();
};

#endif