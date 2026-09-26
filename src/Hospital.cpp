#include "Hospital.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

using namespace std;

namespace {

vector<string> splitRecord(const string& record) {
    vector<string> fields;
    string field;
    stringstream stream(record);

    while (getline(stream, field, '|')) {
        fields.push_back(field);
    }

    return fields;
}
string toUpperCase(string value) {
    transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char c) {
            return static_cast<char>(toupper(c));
        }
    );
    return value;
}
}

Hospital::Hospital()
    : fileManager(
        "data/patients.txt",
        "data/doctors.txt",
        "data/treatment_history.txt"
      ),
      nextPatientNumber(1) {
    fileManager.initializeDataFiles();

    loadDoctorsFromFile();
    loadPatientsFromFile();

    /*
        If no doctors exist in the data file, create
        a default emergency department team.
    */
    if (doctorManager.getTotalDoctors() == 0) {

        doctorManager.addDoctor(
            Doctor(
                "D101",
                "Dr. Sharma",
                "Cardiology"
            )
        );

        doctorManager.addDoctor(
            Doctor(
                "D102",
                "Dr. Mehta",
                "Neurology"
            )
        );

        doctorManager.addDoctor(
            Doctor(
                "D103",
                "Dr. Verma",
                "Orthopedics"
            )
        );
        doctorManager.addDoctor(
            Doctor(
                "D104",
                "Dr. Gupta",
                "General Medicine"
            )
        );
        doctorManager.addDoctor(
            Doctor(
                "D105",
                "Dr. Singh",
                "Trauma"
            )
        );

        saveAllData();
    }

    updateNextPatientNumber();
    rebuildWaitingQueues();
}


   // PRIVATE HELPER FUNCTIONS
string Hospital::generatePatientId() {

    string patientId =
        "P" + to_string(nextPatientNumber);

    nextPatientNumber++;

    return patientId;
}


bool Hospital::patientExists(const string& patientId) const {
    return patients.find(patientId)
        != patients.end();
}
   //PATIENT REGISTRATION

bool Hospital::registerPatient(
    const string& name,
    int age,
    const string& gender,
    int severity,
    const string& emergencyType,
    const string& department
) {

    try {
        string patientId = generatePatientId();

        Patient patient(
            patientId,
            name,
            age,
            gender,
            severity,
            emergencyType,
            department
        );

        auto result =
            patients.emplace(
                patientId,
                patient
            );

        if (!result.second) {
            return false;
        }

        rebuildWaitingQueues();

        cout << "\nPatient registered successfully.\n";
        cout << "Patient ID : "
             << patientId << endl;

        saveAllData();

        return true;
    }
    catch (const exception& error) {
        cout << "Registration failed: "
             << error.what() << endl;

        return false;
    }
}
//PATIENT SEARCH
bool Hospital::searchPatient(
    const string& patientId
) const {

    auto iterator = patients.find(patientId);

    if (iterator == patients.end()) {
        cout << "\nPatient not found.\n";
        return false;
    }

    const Patient& patient = iterator->second;
    cout << "\n";
    cout << "============================================================\n";
    cout << "                    PATIENT DETAILS\n";
    cout << "============================================================\n";

    cout << "Patient ID       : "
         << patient.getPatientId() << endl;

    cout << "Name             : "
         << patient.getName() << endl;

    cout << "Age              : "
         << patient.getAge() << endl;

    cout << "Gender           : "
         << patient.getGender() << endl;

    cout << "Severity         : "
         << patient.getSeverity() << "/10" << endl;

    cout << "Emergency Type   : "
         << patient.getEmergencyType() << endl;

    cout << "Department       : "
         << patient.getDepartment() << endl;

    cout << "Status           : "
         << patient.getStatusString() << endl;

    cout << "Assigned Doctor  : ";

    if (patient.getAssignedDoctorId().empty()) {
        cout << "Not Assigned";
    }
    else {
        cout << patient.getAssignedDoctorId();
    }

    cout << endl;

    cout << "Waiting Time     : "
         << patient.getWaitingTimeMinutes()
         << " minutes" << endl;

    cout << "============================================================\n";

    return true;
}
bool Hospital::triageNextPatient() {

    Patient* patient =
        triageManager.getNextPatient();

    if (patient == nullptr) {

        cout << "\nNo patients are waiting for triage.\n";
        return false;
    }
    try {

        patient->markTriaged();

        cout << "\nPatient triaged successfully.\n";
        cout << "Patient ID : "
             << patient->getPatientId() << endl;

        cout << "Severity   : "
             << patient->getSeverity() << "/10"
             << endl;

        cout << "Department : "
             << patient->getDepartment()
             << endl;

        saveAllData();

        return true;
    }
    catch (const exception& error) {

        cout << "Triage failed: "
             << error.what() << endl;

        return false;
    }
}
//DOCTOR ASSIGNMENT
bool Hospital::assignDoctor(
    const string& patientId
) {

    auto patientIterator =
        patients.find(patientId);

    if (patientIterator == patients.end()) {

        cout << "\nPatient not found.\n";
        return false;
    }

    Patient& patient =
        patientIterator->second;

    if (
        patient.getStatus()
        == PatientStatus::DISCHARGED
    ) {

        cout << "\nDischarged patient cannot be assigned.\n";
        return false;
    }

    if (!patient.getAssignedDoctorId().empty()) {

        cout << "\nA doctor is already assigned to this patient.\n";
        return false;
    }

    Doctor* doctor = doctorManager.findAvailableDoctor(
            patient.getDepartment()
        );

    if (doctor == nullptr) {
        cout << "\nNo available doctor found for department: "
             << patient.getDepartment()
             << endl;

        return false;
    }
    try {
        bool assigned =
            doctorManager.assignPatientToDoctor(
                doctor->getDoctorId(),
                patientId
            );

        if (!assigned) {

            cout << "\nDoctor assignment failed.\n";
            return false;
        }

        patient.assignDoctor(
            doctor->getDoctorId()
        );

        cout << "\nDoctor assigned successfully.\n";

        cout << "Patient : "
             << patient.getPatientId()
             << endl;

        cout << "Doctor  : "
             << doctor->getDoctorId()
             << " - "
             << doctor->getName()
             << endl;

        saveAllData();

        return true;
    }
    catch (const exception& error) {

        cout << "Doctor assignment failed: "
             << error.what()
             << endl;

        return false;
    }
}
   //START TREATMENT
bool Hospital::startTreatment(
    const string& patientId
) {

    auto iterator =
        patients.find(patientId);

    if (iterator == patients.end()) {

        cout << "\nPatient not found.\n";
        return false;
    }

    Patient& patient =
        iterator->second;

    try {
        patient.startTreatment();

        cout << "\nTreatment started.\n";
        cout << "Patient ID : "
             << patient.getPatientId()
             << endl;

        cout << "Doctor ID  : "
             << patient.getAssignedDoctorId()
             << endl;

        cout << "Waiting Time : "
             << patient.getWaitingTimeMinutes()
             << " minutes"
             << endl;
        saveAllData();

        return true;
    }
    catch (const exception& error) {

        cout << "Could not start treatment: "
             << error.what()
             << endl;

        return false;
    }
}
   // DISCHARGE PATIENT
bool Hospital::dischargePatient(
    const string& patientId
) {

    auto iterator =
        patients.find(patientId);

    if (iterator == patients.end()) {

        cout << "\nPatient not found.\n";
        return false;
    }

    Patient& patient =
        iterator->second;

    string doctorId =
        patient.getAssignedDoctorId();

    try {

        patient.discharge();

        if (!doctorId.empty()) {

            doctorManager.releaseDoctor(
                doctorId
            );
        }

        fileManager.saveTreatmentRecord(
            patient
        );

        cout << "\nPatient discharged successfully.\n";

        cout << "Patient ID : "
             << patient.getPatientId()
             << endl;

        cout << "Doctor     : "
             << doctorId
             << endl;

        saveAllData();

        return true;
    }
    catch (const exception& error) {

        cout << "Discharge failed: "
             << error.what()
             << endl;

        return false;
    }
}
   // DOCTOR MANAGEMENT
bool Hospital::addDoctor(
    const string& doctorId,
    const string& name,
    const string& specialization
) {

    try {

        Doctor doctor(
            doctorId,
            name,
            specialization
        );

        bool added =
            doctorManager.addDoctor(
                doctor
            );

        if (!added) {

            cout << "\nDoctor ID already exists.\n";
            return false;
        }

        cout << "\nDoctor added successfully.\n";

        saveAllData();

        return true;
    }
    catch (const exception& error) {

        cout << "Could not add doctor: "
             << error.what()
             << endl;

        return false;
    }
}
bool Hospital::removeDoctor(
    const string& doctorId
) {

    bool removed =
        doctorManager.removeDoctor(
            doctorId
        );

    if (!removed) {

        cout << "\nDoctor could not be removed.\n";
        cout << "Doctor may not exist or may currently be busy.\n";

        return false;
    }
    cout << "\nDoctor removed successfully.\n";

    saveAllData();

    return true;
}
   //DISPLAY FUNCTIONS

void Hospital::displayDoctors() const {

    doctorManager.displayDoctors();
}
void Hospital::viewNextPatient() const {

    Patient* patient = triageManager.peekNextPatient();
    if (patient == nullptr) {

        cout << "\nNo patients are currently waiting.\n";
        return;
    }
    cout << "\n";
    cout << "============================================================\n";
    cout << "                   NEXT PATIENT\n";
    cout << "============================================================\n";

    cout << "Patient ID      : "
         << patient->getPatientId()
         << endl;

    cout << "Name            : "
         << patient->getName()
         << endl;

    cout << "Severity        : "
         << patient->getSeverity()
         << "/10"
         << endl;

    cout << "Age             : "
         << patient->getAge()
         << endl;

    cout << "Emergency Type  : "
         << patient->getEmergencyType()
         << endl;

    cout << "Department      : "
         << patient->getDepartment()
         << endl;

    cout << "============================================================\n";
}


void Hospital::displayWaitingPatients() const {
    triageManager.displayWaitingPatients();
}

void Hospital::displayPatientDetails(const string& patientId) const {

    searchPatient(patientId);
}


void Hospital::displayTreatmentHistory() const {

    vector<string> records =
        fileManager.loadTreatmentRecords();

    cout << "\n";
    cout << "============================================================\n";
    cout << "                  TREATMENT HISTORY\n";
    cout << "============================================================\n";

    if (records.empty()) {

        cout << "No treatment records found.\n";
        return;
    }

    for (const string& record : records) {
        cout << record << endl;
    }

    cout << "============================================================\n";
}

void Hospital::displayStatistics() const {

    vector<Patient> patientList =
        getAllPatients();

    vector<Doctor> doctorList =
        doctorManager.getAllDoctors();

    statisticsManager.displayStatistics(
        patientList,
        doctorList,
        triageManager.getTotalWaitingPatients()
    );
}

   // DATA ACCESS

int Hospital::getPatientCount() const {

    return static_cast<int>(
        patients.size()
    );
}


int Hospital::getDoctorCount() const {

    return doctorManager.getTotalDoctors();
}



   //SAVE ALL DATA
bool Hospital::saveAllData() {

    vector<Patient> patientList;

    patientList.reserve(
        patients.size()
    );
    for (const auto& entry : patients) {
        patientList.push_back(
            entry.second
        );
    }
    vector<Doctor> doctorList =
        doctorManager.getAllDoctors();

    bool patientsSaved =
        fileManager.saveAllPatients(
            patientList
        );

    bool doctorsSaved =
        fileManager.saveAllDoctors(
            doctorList
        );

    return patientsSaved && doctorsSaved;
}
   //CLEAR ALL DATA
bool Hospital::clearAllData() {

    patients.clear();

    triageManager.clearQueues();

    doctorManager.clearDoctors();

    nextPatientNumber = 1;

    bool cleared =
        fileManager.clearAllFiles();

    if (cleared) {

        cout << "\nAll hospital data cleared successfully.\n";
    }
    else {

        cout << "\nSome data files could not be cleared.\n";
    }

    return cleared;
}

   //LOAD PATIENTS

void Hospital::loadPatientsFromFile() {

    vector<string> records =
        fileManager.loadPatientRecords();

    for (const string& record : records) {

        Patient patient;

        if (parsePatientRecord(record,patient)) {
            patients[
                patient.getPatientId()
            ] = patient;
        }
    }
}


   //LOAD DOCTORS

void Hospital::loadDoctorsFromFile() {

    vector<string> records =
        fileManager.loadDoctorRecords();

    for (const string& record : records) {

        Doctor doctor;

        bool isBusy = false;

        string currentPatientId;

        if (
            parseDoctorRecord(
                record,
                doctor,
                isBusy,
                currentPatientId
            )
        ) {

            bool added =
                doctorManager.addDoctor(
                    doctor
                );

            if (
                added &&
                isBusy &&
                !currentPatientId.empty()
            ) {

                doctorManager.restoreDoctorState(
                    doctor.getDoctorId(),
                    currentPatientId
                );
            }
        }
    }
}


   //REBUILD WAITING QUEUES

void Hospital::rebuildWaitingQueues() {

    triageManager.clearQueues();

    for (auto& entry : patients) {

        Patient& patient =entry.second;

        if (patient.getStatus()== PatientStatus::WAITING) {

            triageManager.addPatient(
                &patient
            );
        }
    }
}
   //UPDATE NEXT PATIENT ID

void Hospital::updateNextPatientNumber() {

    int maximumNumber = 0;

    for (const auto& entry : patients) {

        string patientId =
            entry.first;

        if (patientId.size() > 1 && patientId[0] == 'P') {

            try {
                int number =
                    stoi(
                        patientId.substr(1)
                    );

                maximumNumber =
                    max(
                        maximumNumber,
                        number
                    );
            }
            catch (...) {
                // Ignore invalid patient IDs.
            }
        }
    }
    nextPatientNumber =
        maximumNumber + 1;
}
   //PATIENT RECORD PARSING

bool Hospital::parsePatientRecord(
    const string& record,
    Patient& patient
) const {

    vector<string> fields =
        splitRecord(record);

    /*
        Current format:

        0  Patient ID
        1  Name
        2  Age
        3  Gender
        4  Severity
        5  Emergency Type
        6  Department
        7  Status
        8  Doctor ID
        9  Arrival Time
        10 Treatment Start Time
        11 Discharge Time
    */

    if (fields.size() < 9) {
        return false;
    }
    try {
        string patientId = fields[0];

        string name = fields[1];

        int age = stoi(fields[2]);

        string gender = fields[3];

        int severity = stoi(fields[4]);

        string emergencyType = fields[5];

        string department = fields[6];

        PatientStatus status =
            Patient::statusFromString(
                fields[7]
            );

        string doctorId = fields[8];

        patient =
            Patient(
                patientId,
                name,
                age,
                gender,
                severity,
                emergencyType,
                department
            );

        long long arrivalEpoch = 0;
        long long treatmentEpoch = 0;
        long long dischargeEpoch = 0;

        /*
            Timestamp fields were added later,
            so older 9-field records are still supported.
        */

        if (fields.size() >= 10) {
            arrivalEpoch = stoll(fields[9]);
        }

        if (fields.size() >= 11) {
            treatmentEpoch = stoll(fields[10]);
        }

        if (fields.size() >= 12) {
            dischargeEpoch = stoll(fields[11]);
        }

        patient.restoreState(
            status,
            doctorId,
            arrivalEpoch,
            treatmentEpoch,
            dischargeEpoch
        );

        return true;
    }
    catch (...) {

        return false;
    }
}

   //DOCTOR RECORD PARSING

bool Hospital::parseDoctorRecord(
    const string& record,
    Doctor& doctor,
    bool& isBusy,
    string& currentPatientId
) const {

    vector<string> fields =
        splitRecord(record);

    /*
        Doctor format:

        0  Doctor ID
        1  Name
        2  Specialization
        3  AVAILABLE / BUSY
        4  Current Patient ID
    */

    if (fields.size() < 3) {
        return false;
    }

    try {

        string doctorId =
            fields[0];

        string name =
            fields[1];

        string specialization =
            fields[2];

        doctor =
            Doctor(
                doctorId,
                name,
                specialization
            );

        isBusy = false;
        currentPatientId = "";

        if (fields.size() >= 4) {

            string status =
                toUpperCase(fields[3]);

            isBusy =
                status == "BUSY";
        }

        if (fields.size() >= 5) {
            currentPatientId = fields[4];
        }

        return true;
    }
    catch (...) {

        return false;
    }
}


   //GET ALL PATIENTS

vector<Patient> Hospital::getAllPatients() const {
    vector<Patient> patientList;
    patientList.reserve(
        patients.size()
    );

    for (const auto& entry : patients) {
        patientList.push_back(
            entry.second
        );
    }
    return patientList;
}