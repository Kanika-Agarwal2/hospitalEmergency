#ifndef HOSPITAL_H
#define HOSPITAL_H
#include "Patient.h"
#include "Doctor.h"
#include "TriageManager.h"
#include "DoctorManager.h"
#include "FileManager.h"
#include "StatisticsManager.h"

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Hospital {

private:

    unordered_map<string, Patient> patients;

    TriageManager triageManager;
    DoctorManager doctorManager;
    FileManager fileManager;
    StatisticsManager statisticsManager;

    int nextPatientNumber;

    // Patient ID Management

    string generatePatientId();

    bool patientExists(
        const string& patientId
    ) const;

    // Persistence
    void loadPatientsFromFile();
    void loadDoctorsFromFile();
    void rebuildWaitingQueues();
    void updateNextPatientNumber();

    // Record Parsing
    bool parsePatientRecord(
        const string& record,
        Patient& patient
    ) const;
    bool parseDoctorRecord(
        const string& record,
        Doctor& doctor,
        bool& isBusy,
        string& currentPatientId
    ) const;
public:

    // Constructor
    Hospital();

    // Patient Management

    bool registerPatient(
        const string& name,
        int age,
        const string& gender,
        int severity,
        const string& emergencyType,
        const string& department
    );

    bool searchPatient(
        const string& patientId
    ) const;

    bool triageNextPatient();

    bool assignDoctor(
        const string& patientId
    );

    bool startTreatment(
        const string& patientId
    );

    bool dischargePatient(
        const string& patientId
    );

    // Doctor Management
    bool addDoctor(
        const string& doctorId,
        const string& name,
        const string& specialization
    );

    bool removeDoctor(
        const string& doctorId
    );

    // Display Functions
    void displayDoctors() const;

    void viewNextPatient() const;

    void displayWaitingPatients() const;

    void displayPatientDetails(
        const string& patientId
    ) const;

    void displayTreatmentHistory() const;

    void displayStatistics() const;
    // Data Management

    bool saveAllData();

    bool clearAllData();


    // Getters
    int getPatientCount() const;

    int getDoctorCount() const;

    vector<Patient> getAllPatients() const;
};
#endif