#include "Patient.h"
#include "Doctor.h"
#include "TriageManager.h"

#include <cassert>
#include <iostream>

using namespace std;


// ============================================================
// TEST PATIENT CREATION
// ============================================================

void testPatientCreation() {

    Patient patient(
        "P1",
        "Kanika",
        21,
        "Female",
        8,
        "Chest Pain",
        "Cardiology"
    );

    assert(
        patient.getPatientId() == "P1"
    );

    assert(
        patient.getName() == "Kanika"
    );

    assert(
        patient.getSeverity() == 8
    );

    assert(
        patient.isEmergency()
    );

    assert(
        patient.getStatus()
        == PatientStatus::WAITING
    );
}


// ============================================================
// TEST DOCTOR ASSIGNMENT
// ============================================================

void testDoctorAssignment() {

    Doctor doctor(
        "D101",
        "Dr. Sharma",
        "Cardiology"
    );

    assert(
        doctor.isAvailable()
    );

    doctor.assignPatient("P1");

    assert(
        !doctor.isAvailable()
    );

    assert(
        doctor.getCurrentPatientId()
        == "P1"
    );

    doctor.releasePatient();

    assert(
        doctor.isAvailable()
    );
}


// ============================================================
// TEST TRIAGE PRIORITY
// ============================================================

void testTriagePriority() {

    Patient normalPatient(
        "P1",
        "Normal Patient",
        30,
        "Male",
        4,
        "Fracture",
        "Orthopedics"
    );

    Patient emergencyPatient(
        "P2",
        "Emergency Patient",
        40,
        "Female",
        9,
        "Chest Pain",
        "Cardiology"
    );

    TriageManager triage;

    triage.addPatient(
        &normalPatient
    );

    triage.addPatient(
        &emergencyPatient
    );

    Patient* next =
        triage.getNextPatient();

    assert(
        next != nullptr
    );

    assert(
        next->getPatientId()
        == "P2"
    );
}


// ============================================================
// TEST UNKNOWN NEXT PATIENT
// ============================================================

void testEmptyTriage() {

    TriageManager triage;

    assert(
        triage.empty()
    );

    Patient* patient =
        triage.getNextPatient();

    assert(
        patient == nullptr
    );
}


// ============================================================
// MAIN TEST RUNNER
// ============================================================

int main() {

    testPatientCreation();

    testDoctorAssignment();

    testTriagePriority();

    testEmptyTriage();

    cout
        << "All tests passed successfully.\n";

    return 0;
}