#include "Doctor.h"

#include <stdexcept>

using namespace std;

Doctor::Doctor()
    : doctorId(""),
      name(""),
      specialization(""),
      available(true),
      currentPatientId("") {
}

Doctor::Doctor(
    const string& doctorId,
    const string& name,
    const string& specialization
)
    : doctorId(doctorId),
      name(name),
      specialization(specialization),
      available(true),
      currentPatientId("") {

    if (doctorId.empty()) {
        throw invalid_argument("Doctor ID cannot be empty.");
    }

    if (name.empty()) {
        throw invalid_argument("Doctor name cannot be empty.");
    }

    if (specialization.empty()) {
        throw invalid_argument("Specialization cannot be empty.");
    }
}


// ================= GETTERS =================

string Doctor::getDoctorId() const {
    return doctorId;
}

string Doctor::getName() const {
    return name;
}

string Doctor::getSpecialization() const {
    return specialization;
}

bool Doctor::isAvailable() const {
    return available;
}

string Doctor::getCurrentPatientId() const {
    return currentPatientId;
}


// ================= ASSIGNMENT =================

void Doctor::assignPatient(const string& patientId) {

    if (!available) {
        throw logic_error("Doctor is already busy.");
    }

    if (patientId.empty()) {
        throw invalid_argument("Patient ID cannot be empty.");
    }

    currentPatientId = patientId;
    available = false;
}

void Doctor::releasePatient() {

    currentPatientId = "";
    available = true;
}