#include "FileManager.h"

#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std;

// CONSTRUCTOR
FileManager::FileManager( const string& patientFile,const string& doctorFile,const string& treatmentFile)
    : patientFile(patientFile),
      doctorFile(doctorFile),
      treatmentFile(treatmentFile) {}

// INITIALIZE DATA FILES


bool FileManager::initializeDataFiles() const {

    try {

        filesystem::path patientPath(
            patientFile
        );

        filesystem::path doctorPath(
            doctorFile
        );

        filesystem::path treatmentPath(
            treatmentFile
        );

        if (
            !patientPath.parent_path().empty()
        ) {

            filesystem::create_directories(
                patientPath.parent_path()
            );
        }

        if(!doctorPath.parent_path().empty()) {
            filesystem::create_directories(
                doctorPath.parent_path()
            );
        }
        if(!treatmentPath.parent_path().empty()) {
            filesystem::create_directories(
                treatmentPath.parent_path()
            );
        }
        ofstream patientStream(
            patientFile,
            ios::app
        );
        ofstream doctorStream(
            doctorFile,
            ios::app
        );
        ofstream treatmentStream(
            treatmentFile,
            ios::app
        );
        return patientStream.good() && doctorStream.good() && treatmentStream.good();
    }
    catch (...) {
        return false;
    }
}

// TIME POINT → EPOCH MILLISECONDS

long long FileManager::timePointToEpoch(chrono::system_clock::time_point time) const {
    if (time.time_since_epoch().count()==0) {
        return 0;
    }
    return chrono::duration_cast<
        chrono::milliseconds
    >(
        time.time_since_epoch()
    ).count();
}

// SERIALIZE PATIENT

string FileManager::serializePatient(const Patient& patient) const {
    stringstream data;
    data
        << patient.getPatientId()
        << "|"

        << patient.getName()
        << "|"

        << patient.getAge()
        << "|"

        << patient.getGender()
        << "|"

        << patient.getSeverity()
        << "|"

        << patient.getEmergencyType()
        << "|"

        << patient.getDepartment()
        << "|"

        << patient.getStatusString()
        << "|"

        << patient.getAssignedDoctorId()
        << "|"

        << timePointToEpoch(
               patient.getArrivalTime()
           )
        << "|"

        << timePointToEpoch(
               patient.getTreatmentStartTime()
           )
        << "|"

        << timePointToEpoch(
               patient.getDischargeTime()
           );

    return data.str();
}


// SERIALIZE DOCTOR


string FileManager::serializeDoctor(const Doctor& doctor) const {
    stringstream data;
    data
        << doctor.getDoctorId()
        << "|"

        << doctor.getName()
        << "|"

        << doctor.getSpecialization()
        << "|"
        << (
            doctor.isAvailable()
                ? "AVAILABLE"
                : "BUSY"
        )
        << "|"

        << doctor.getCurrentPatientId();

    return data.str();
}

// SAVE SINGLE PATIENT

bool FileManager::savePatient(const Patient& patient) {
    ofstream file(
        patientFile,
        ios::app
    );

    if (!file.is_open()) {
        return false;
    }

    file
        << serializePatient(patient)
        << '\n';

    file.close();
    return true;
}

// SAVE SINGLE DOCTOR

bool FileManager::saveDoctor(const Doctor& doctor) {
    ofstream file(
        doctorFile,
        ios::app
    );
    if (!file.is_open()) {
        return false;
    }
    file
        << serializeDoctor(doctor)
        << '\n';

    file.close();
    return true;
}

// SAVE TREATMENT RECORD

bool FileManager::saveTreatmentRecord(const Patient& patient) {
    ofstream file(
        treatmentFile,
        ios::app
    );

    if (!file.is_open()) {
        return false;
    }

    file
        << patient.getPatientId()
        << "|"

        << patient.getName()
        << "|"

        << patient.getDepartment()
        << "|"

        << patient.getSeverity()
        << "|"

        << patient.getAssignedDoctorId()
        << "|"

        << patient.getWaitingTimeMinutes()
        << "|"

        << timePointToEpoch(
               patient.getDischargeTime()
           )

        << '\n';

    file.close();

    return true;
}

// SAVE ALL PATIENTS

bool FileManager::saveAllPatients(const vector<Patient>& patients) {

    ofstream file(
        patientFile,
        ios::trunc
    );

    if (!file.is_open()) {
        return false;
    }

    for (const Patient& patient: patients) {
        file
            << serializePatient(patient)
            << '\n';
    }

    file.close();

    return true;
}

// SAVE ALL DOCTORS

bool FileManager::saveAllDoctors(
    const vector<Doctor>& doctors
) {

    ofstream file(
        doctorFile,
        ios::trunc
    );

    if (!file.is_open()) {
        return false;
    }

    for (
        const Doctor& doctor
        : doctors
    ) {

        file
            << serializeDoctor(doctor)
            << '\n';
    }

    file.close();

    return true;
}

// LOAD PATIENT RECORDS


vector<string>
FileManager::loadPatientRecords() const {

    vector<string> records;

    ifstream file(
        patientFile
    );

    if (!file.is_open()) {
        return records;
    }

    string line;

    while (
        getline(file, line)
    ) {

        if (!line.empty()) {

            records.push_back(
                line
            );
        }
    }

    file.close();

    return records;
}


// LOAD DOCTOR RECORDS

vector<string>
FileManager::loadDoctorRecords() const {

    vector<string> records;

    ifstream file(
        doctorFile
    );

    if (!file.is_open()) {
        return records;
    }

    string line;

    while (
        getline(file, line)
    ) {

        if (!line.empty()) {

            records.push_back(
                line
            );
        }
    }

    file.close();

    return records;
}


// LOAD TREATMENT RECORDS

vector<string>
FileManager::loadTreatmentRecords() const {

    vector<string> records;

    ifstream file(
        treatmentFile
    );

    if (!file.is_open()) {
        return records;
    }

    string line;

    while (
        getline(file, line)
    ) {

        if (!line.empty()) {

            records.push_back(
                line
            );
        }
    }

    file.close();

    return records;
}

// CLEAR ALL FILES
bool FileManager::clearAllFiles() {
    try {
        ofstream patientStream(
            patientFile,
            ios::trunc
        );

        ofstream doctorStream(
            doctorFile,
            ios::trunc
        );

        ofstream treatmentStream(
            treatmentFile,
            ios::trunc
        );

        bool success =patientStream.good() && doctorStream.good() && treatmentStream.good();

        patientStream.close();
        doctorStream.close();
        treatmentStream.close();

        return success;
    }
    catch (...) {

        return false;
    }
}