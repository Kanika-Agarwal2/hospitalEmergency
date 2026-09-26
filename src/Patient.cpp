#include "Patient.h"

#include <stdexcept>
#include <algorithm>
#include <cctype>

using namespace std;

// HELPER


namespace {

string toUpperCase(string value) {

    transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char c) {
            return static_cast<char>(
                toupper(c)
            );
        }
    );

    return value;
}

chrono::system_clock::time_point
epochToTimePoint(long long epochMilliseconds) {

    if (epochMilliseconds <= 0) {
        return chrono::system_clock::time_point();
    }

    return
        chrono::system_clock::time_point(
            chrono::milliseconds(
                epochMilliseconds
            )
        );
}

}


// DEFAULT CONSTRUCTOR


Patient::Patient()
    : patientId(""),
      name(""),
      age(0),
      gender(""),
      severity(0),
      emergencyType(""),
      department(""),
      status(PatientStatus::WAITING),
      assignedDoctorId(""),
      arrivalTime(
          chrono::system_clock::now()
      ),
      treatmentStartTime(),
      dischargeTime() {}


// PARAMETERIZED CONSTRUCTOR


Patient::Patient(
    const string& patientId,
    const string& name,
    int age,
    const string& gender,
    int severity,
    const string& emergencyType,
    const string& department
)
    : patientId(patientId),
      name(name),
      age(age),
      gender(gender),
      severity(severity),
      emergencyType(emergencyType),
      department(department),
      status(PatientStatus::WAITING),
      assignedDoctorId(""),
      arrivalTime(
          chrono::system_clock::now()
      ),
      treatmentStartTime(),
      dischargeTime() {

    if (patientId.empty()) {
        throw invalid_argument(
            "Patient ID cannot be empty."
        );
    }

    if (name.empty()) {
        throw invalid_argument(
            "Patient name cannot be empty."
        );
    }

    if (age <= 0) {
        throw invalid_argument(
            "Age must be greater than 0."
        );
    }

    if (severity < 1 || severity > 10) {
        throw invalid_argument(
            "Severity must be between 1 and 10."
        );
    }
}


// GETTERS

string Patient::getPatientId() const {
    return patientId;
}

string Patient::getName() const {
    return name;
}

int Patient::getAge() const {
    return age;
}

string Patient::getGender() const {
    return gender;
}

int Patient::getSeverity() const {
    return severity;
}

string Patient::getEmergencyType() const {
    return emergencyType;
}

string Patient::getDepartment() const {
    return department;
}

PatientStatus Patient::getStatus() const {
    return status;
}

string Patient::getAssignedDoctorId() const {
    return assignedDoctorId;
}

chrono::system_clock::time_point
Patient::getArrivalTime() const {
    return arrivalTime;
}

chrono::system_clock::time_point
Patient::getTreatmentStartTime() const {
    return treatmentStartTime;
}

chrono::system_clock::time_point
Patient::getDischargeTime() const {
    return dischargeTime;
}

// SET SEVERITY


void Patient::setSeverity(int severity) {

    if (severity < 1 || severity > 10) {

        throw invalid_argument(
            "Severity must be between 1 and 10."
        );
    }

    this->severity = severity;
}


// SET DEPARTMENT


void Patient::setDepartment(const string& department) {
    this->department = department;
}

// ASSIGN DOCTOR
void Patient::assignDoctor(const string& doctorId) {
    if (doctorId.empty()) {

        throw invalid_argument(
            "Doctor ID cannot be empty."
        );
    }

    assignedDoctorId =doctorId;

    status =PatientStatus::DOCTOR_ASSIGNED;
}

// START TREATMENT

void Patient::startTreatment() {
    if (assignedDoctorId.empty()) {

        throw logic_error(
            "Cannot start treatment without assigning a doctor."
        );
    }

    treatmentStartTime =
        chrono::system_clock::now();

    status =
        PatientStatus::UNDER_TREATMENT;
}


// DISCHARGE
void Patient::discharge() {

    if (status != PatientStatus::UNDER_TREATMENT) {

        throw logic_error(
            "Patient must be under treatment before discharge."
        );
    }

    dischargeTime =
        chrono::system_clock::now();

    status =
        PatientStatus::DISCHARGED;
}


// MARK TRIAGED


void Patient::markTriaged() {

    if (status !=PatientStatus::WAITING) {
        throw logic_error(
            "Only waiting patients can be triaged."
        );
    }

    status =
        PatientStatus::TRIAGED;
}

// RESTORE SAVED PATIENT STATE

void Patient::restoreState(
    PatientStatus restoredStatus,
    const string& doctorId,
    long long arrivalEpoch,
    long long treatmentEpoch,
    long long dischargeEpoch
) {

    status = restoredStatus;

    assignedDoctorId = doctorId;

    arrivalTime =
        epochToTimePoint(
            arrivalEpoch
        );

    treatmentStartTime =
        epochToTimePoint(
            treatmentEpoch
        );

    dischargeTime =
        epochToTimePoint(
            dischargeEpoch
        );

    // Safety fallback for old/incomplete records.
    if (arrivalTime.time_since_epoch().count()==0) {
        arrivalTime = chrono::system_clock::now();
    }
}


// EMERGENCY CHECK
bool Patient::isEmergency() const {

    return severity >= 7;
}


// STATUS TO STRING


string Patient::getStatusString() const {

    switch (status) {

        case PatientStatus::WAITING:
            return "WAITING";

        case PatientStatus::TRIAGED:
            return "TRIAGED";

        case PatientStatus::DOCTOR_ASSIGNED:
            return "DOCTOR_ASSIGNED";

        case PatientStatus::UNDER_TREATMENT:
            return "UNDER_TREATMENT";

        case PatientStatus::DISCHARGED:
            return "DISCHARGED";
    }

    return "UNKNOWN";
}


// STRING TO STATUS


PatientStatus Patient::statusFromString(const string& status) {
    string value = toUpperCase(status);

    if (value == "WAITING") {
        return PatientStatus::WAITING;
    }

    if (value == "TRIAGED") {
        return PatientStatus::TRIAGED;
    }

    if (value == "DOCTOR_ASSIGNED") {
        return PatientStatus::DOCTOR_ASSIGNED;
    }

    if (value == "UNDER_TREATMENT") {
        return PatientStatus::UNDER_TREATMENT;
    }

    if (value == "DISCHARGED") {
        return PatientStatus::DISCHARGED;
    }

    throw invalid_argument(
        "Invalid patient status: " + status
    );
}


// WAITING TIME
long long Patient::getWaitingTimeMinutes() const {

    if (treatmentStartTime.time_since_epoch().count()==0) {
        return 0;
    }
    auto duration =
        chrono::duration_cast<chrono::minutes>(
            treatmentStartTime-arrivalTime
        );
    return duration.count();
}