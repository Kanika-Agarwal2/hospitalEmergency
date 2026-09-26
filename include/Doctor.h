#ifndef DOCTOR_H
#define DOCTOR_H
#include <string>
using namespace std;
class Doctor {
private:
    string doctorId;
    string name;
    string specialization;
    bool available;
    string currentPatientId;
public:
    Doctor();
    Doctor(const string& doctorId,const string& name,const string& specialization);
    string getDoctorId() const;
    string getName() const;
    string getSpecialization() const;
    bool isAvailable() const;
    string getCurrentPatientId() const;
    void assignPatient(const string& patientId);
    void releasePatient();
};

#endif