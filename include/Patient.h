#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <chrono>

using namespace std;

enum class PatientStatus {
    WAITING,
    TRIAGED,
    DOCTOR_ASSIGNED,
    UNDER_TREATMENT,
    DISCHARGED
};

class Patient {
private:

    string patientId;
    string name;
    int age;
    string gender;
    int severity;
    string emergencyType;
    string department;

    PatientStatus status;

    string assignedDoctorId;

    chrono::system_clock::time_point arrivalTime;
    chrono::system_clock::time_point treatmentStartTime;
    chrono::system_clock::time_point dischargeTime;

public:

    // CONSTRUCTORS

    Patient();

    Patient(
        const string& patientId,
        const string& name,
        int age,
        const string& gender,
        int severity,
        const string& emergencyType,
        const string& department
    );

    // GETTERS

    string getPatientId() const;

    string getName() const;

    int getAge() const;

    string getGender() const;

    int getSeverity() const;

    string getEmergencyType() const;

    string getDepartment() const;

    PatientStatus getStatus() const;

    string getAssignedDoctorId() const;

    chrono::system_clock::time_point getArrivalTime() const;

    chrono::system_clock::time_point getTreatmentStartTime() const;

    chrono::system_clock::time_point getDischargeTime() const;

    // SETTERS
    void setSeverity(
        int severity
    );

    void setDepartment(
        const string& department
    );

    // LIFECYCLE OPERATIONS
    void assignDoctor(
        const string& doctorId
    );

    void startTreatment();

    void discharge();

    void markTriaged();

    // RESTORE SAVED STATE
    void restoreState(
        PatientStatus status,
        const string& doctorId,
        long long arrivalEpoch,
        long long treatmentEpoch,
        long long dischargeEpoch
    );

    bool isEmergency() const;

    string getStatusString() const;

    static PatientStatus statusFromString(
        const string& status
    );
    long long getWaitingTimeMinutes() const;
};

#endif