#include "Hospital.h"
#include <iostream>
#include <limits>
#include <string>
#include <vector>
using namespace std;

void clearInputBuffer() {
    cin.ignore(
        numeric_limits<streamsize>::max(),
        '\n'
    );
}
int readInteger(
    const string& message
) {
    int value;
    while (true) {

        cout << message;

        if (cin >> value) {

            clearInputBuffer();
            return value;
        }

        cout << "\n[ERROR] Invalid input. Please enter a number.\n";
        cin.clear();
        clearInputBuffer();
    }
}

int readIntegerInRange(
    const string& message,
    int minimum,
    int maximum
) {
    while (true) {
        int value = readInteger(message);
        if (value >= minimum && value <= maximum) {
            return value;
        }
        cout << "\n[ERROR] Please enter a value between "
             << minimum
             << " and "
             << maximum
             << ".\n";
    }
}


string readString(const string& message) {
    string value;
    cout << message;
    getline(cin,value);
    return value;
}
string readNonEmptyString(const string& message) {
    while (true) {
        string value = readString(message);
        if (!value.empty()) {
            return value;
        }
        cout << "\n[ERROR] Input cannot be empty. "
                "Please try again.\n";
    }
}
void pauseScreen() {
    cout << "\n";
    cout << "------------------------------------------------------------\n";
    cout << "Press ENTER to continue...";
    cin.get();
}
void printSectionHeader(const string& title) {
    cout << "\n";
    cout << "============================================================\n";
    cout << "  " << title << "\n";
    cout << "============================================================\n";
}

// GENDER SELECTION
string selectGender() {
    vector<string> genders = {
        "Male",
        "Female",
        "Other"
    };
    printSectionHeader("GENDER SELECTION");
    for (int i = 0;i < static_cast<int>(genders.size());i++) {
        cout << "  [" << i + 1 << "] "
             << genders[i]
             << "\n";
    }
    cout << "\n";
    int choice = readIntegerInRange(
            "Select gender: ",
            1,
            static_cast<int>(
                genders.size()
            )
        );
    return genders[
        choice - 1
    ];
}

// EMERGENCY TYPE
string selectEmergencyType() {
    vector<string> emergencyTypes = {
        "Chest Pain",
        "Accident / Trauma",
        "Stroke Symptoms",
        "Breathing Difficulty",
        "Fracture",
        "Other"
    };
    printSectionHeader("EMERGENCY TYPE");
    for (int i = 0;i < static_cast<int>(emergencyTypes.size());i++) {
        cout << "  [" << i + 1 << "] " << emergencyTypes[i] << "\n";
    }
    cout << "\n";
    int choice = readIntegerInRange(
            "Select emergency type: ",
            1,
            static_cast<int>(
                emergencyTypes.size()
            )
        );
    if (choice == 6) {
        string customEmergencyType;
        while (customEmergencyType.empty()) {
            customEmergencyType =
                readString(
                    "Enter emergency type: "
                );

            if (customEmergencyType.empty()) {
                cout << "[ERROR] Emergency type cannot be empty.\n";
            }
        }
        return customEmergencyType;
    }
    return emergencyTypes [
        choice - 1
    ];
}

// ============================================================
string selectDepartment() {
    vector<string> departments = {
        "Cardiology",
        "Neurology",
        "Orthopedics",
        "General Medicine",
        "Trauma"
    };
    printSectionHeader("DEPARTMENT SELECTION");
    for (int i = 0;i < static_cast<int>(departments.size());i++) {
        cout << "  [" << i + 1 << "] "
             << departments[i]
             << "\n";
    }
    cout << "\n";
    int choice =
        readIntegerInRange(
            "Select department: ",
            1,
            static_cast<int>(
                departments.size()
            )
        );
    return departments[
        choice - 1
    ];
}

// APPLICATION HEADER
void displayHeader() {
    cout << "\n";
    cout << "+------------------------------------------------------------+\n";
    cout << "|                                                            |\n";
    cout << "|        HOSPITAL EMERGENCY MANAGEMENT SYSTEM                |\n";
    cout << "|              Patient Triage & Care System                  |\n";
    cout << "|                                                            |\n";
    cout << "+------------------------------------------------------------+\n";
}

// MAIN MENU

void displayMenu() {
    cout << "\n";
    cout << "+------------------------ MAIN MENU -------------------------+\n";
    cout << "|                                                            |\n";
    cout << "|  PATIENT MANAGEMENT                                        |\n";
    cout << "|  [1]  Register Patient                                     |\n";
    cout << "|  [2]  View Next Patient                                    |\n";
    cout << "|  [3]  Triage Next Patient                                  |\n";
    cout << "|  [7]  Search Patient                                       |\n";
    cout << "|  [8]  Display Waiting Patients                             |\n";
    cout << "|                                                            |\n";
    cout << "|  TREATMENT                                                 |\n";
    cout << "|  [4]  Assign Doctor                                        |\n";
    cout << "|  [5]  Start Treatment                                      |\n";
    cout << "|  [6]  Discharge Patient                                    |\n";
    cout << "|  [10] Display Treatment History                            |\n";
    cout << "|                                                            |\n";
    cout << "|  DOCTOR MANAGEMENT                                         |\n";
    cout << "|  [9]  Display Doctors                                      |\n";
    cout << "|  [12] Add Doctor                                           |\n";
    cout << "|  [13] Remove Doctor                                        |\n";
    cout << "|                                                            |\n";
    cout << "|  SYSTEM                                                    |\n";
    cout << "|  [11] Hospital Statistics                                  |\n";
    cout << "|  [14] Save Data                                            |\n";
    cout << "|  [15] Clear All Data                                       |\n";
    cout << "|  [16] Exit                                                 |\n";
    cout << "|                                                            |\n";
    cout << "+------------------------------------------------------------+\n";
}

// MAIN
int main() {
    Hospital hospital;
    displayHeader();
    cout << "\n";
    cout << "  System initialized successfully.\n";
    cout << "  Hospital management services are ready.\n";
    while (true) {
        displayMenu();
        int choice =
            readInteger(
                "\nEnter your choice: "
            );

        cout << "\n";
        // 1. REGISTER PATIENT
        if (choice == 1) {
            printSectionHeader(
                "PATIENT REGISTRATION"
            );
            string name =
                readNonEmptyString(
                    "Enter patient name: "
                );
            int age =
                readIntegerInRange(
                    "Enter age: ",
                    1,
                    120
                );
            string gender = selectGender();
            int severity =
                readIntegerInRange(
                    "Enter severity (1-10): ",
                    1,
                    10
                );
            string emergencyType = selectEmergencyType();
            string department = selectDepartment();
            printSectionHeader(
                "REGISTRATION SUMMARY"
            );
            cout << "  Name           : "
                 << name
                 << "\n";
            cout << "  Age            : "
                 << age
                 << "\n";
            cout << "  Gender         : "
                 << gender
                 << "\n";
            cout << "  Severity       : "
                 << severity
                 << "/10\n";
            cout << "  Emergency Type : "
                 << emergencyType
                 << "\n";
            cout << "  Department     : "
                 << department
                 << "\n";
            cout << "  Patient Class  : "
                 << (
                     severity >= 7
                         ? "EMERGENCY"
                         : "NORMAL"
                 )
                 << "\n";
            cout << "\n";
            cout << "  Registering patient...\n";
            hospital.registerPatient(
                name,
                age,
                gender,
                severity,
                emergencyType,
                department
            );
            pauseScreen();
        }
        // 2. VIEW NEXT PATIENT
        else if (choice == 2) {
            hospital.viewNextPatient();
            pauseScreen();
        }
        // 3. TRIAGE NEXT PATIENT
        else if (choice == 3) {
            hospital.triageNextPatient();
            pauseScreen();
        }
        // 4. ASSIGN DOCTOR
        else if (choice == 4) {
            printSectionHeader("DOCTOR ASSIGNMENT");

            string patientId =
                readNonEmptyString(
                    "Enter patient ID: "
                );
            hospital.assignDoctor(patientId);
            pauseScreen();
        }
        // 5. START TREATMENT
        else if (choice == 5) {
            printSectionHeader("START TREATMENT");
            string patientId =
                readNonEmptyString(
                    "Enter patient ID: "
                );

            hospital.startTreatment(patientId);
            pauseScreen();
        }
        // 6. DISCHARGE
        else if (choice == 6) {
            printSectionHeader(
                "PATIENT DISCHARGE"
            );
            string patientId =
                readNonEmptyString(
                    "Enter patient ID: "
                );
            hospital.dischargePatient(patientId);
            pauseScreen();
        }
        // 7. SEARCH PATIENT
        else if (choice == 7) {
            printSectionHeader("PATIENT SEARCH");
            string patientId =
                readNonEmptyString(
                    "Enter patient ID: "
                );

            hospital.displayPatientDetails(patientId);
            pauseScreen();
        }
        // 8. WAITING PATIENTS
        else if (choice == 8) {
            hospital.displayWaitingPatients();
            pauseScreen();
        }
        // 9. DOCTORS
        else if (choice == 9) {
            hospital.displayDoctors();
            pauseScreen();
        }
        // 10. TREATMENT HISTORY
        else if (choice == 10) {

            hospital.displayTreatmentHistory();

            pauseScreen();
        }
        // 11. STATISTICS
        else if (choice == 11) {
            hospital.displayStatistics();

            pauseScreen();
        }
        // 12. ADD DOCTOR
        else if (choice == 12) {
            printSectionHeader(
                "ADD DOCTOR"
            );
            string doctorId =
                readNonEmptyString(
                    "Enter doctor ID: "
                );
            string name =
                readNonEmptyString(
                    "Enter doctor name: "
                );
            cout << "\n";
            cout << "Available Specializations:\n\n";
            cout << "  [1] Cardiology\n";
            cout << "  [2] Neurology\n";
            cout << "  [3] Orthopedics\n";
            cout << "  [4] General Medicine\n";
            cout << "  [5] Trauma\n";
            cout << "\n";
            int specializationChoice =
                readIntegerInRange(
                    "Select specialization: ",
                    1,
                    5
                );
            vector<string> specializations = {
                "Cardiology",
                "Neurology",
                "Orthopedics",
                "General Medicine",
                "Trauma"
            };
            string specialization =
                specializations[
                    specializationChoice - 1
                ];
            cout << "\n";
            cout << "Adding doctor...\n";
            hospital.addDoctor(
                doctorId,
                name,
                specialization
            );

            pauseScreen();
        }
        // 13. REMOVE DOCTOR
        else if (choice == 13) {
            printSectionHeader(
                "REMOVE DOCTOR"
            );
            string doctorId =
                readNonEmptyString(
                    "Enter doctor ID: "
                );

            hospital.removeDoctor(
                doctorId
            );
            pauseScreen();
        }
        // 14. SAVE DATA
        else if (choice == 14) {
            printSectionHeader(
                "DATA PERSISTENCE"
            );
            cout << "Saving hospital data...\n\n";
            if (hospital.saveAllData()) {
                cout << "[SUCCESS] All data saved successfully.\n";
            }
            else {
                cout << "[ERROR] Some data could not be saved.\n";
            }
            pauseScreen();
        }
        // 15. CLEAR ALL DATA
        else if (choice == 15) {
            printSectionHeader("CLEAR ALL DATA");
            cout << "WARNING: This will remove all stored\n";
            cout << "patients, doctors and saved hospital data.\n\n";
            string confirmation =
                readString("Type YES to confirm deletion: ");
            if (confirmation == "YES") {
                hospital.clearAllData();
                cout << "\n";
                cout << "[SUCCESS] All data cleared.\n";
            }
            else {
                cout<< "\nOperation cancelled.\n";
            }
            pauseScreen();
        }
        // 16. EXIT
        else if (choice == 16) {
            printSectionHeader(
                "SYSTEM SHUTDOWN"
            );
            cout
                << "Saving data before exit...\n";
            if (hospital.saveAllData()) {
                cout << "Data saved successfully.\n";
            }
            else {
                cout << "Warning: Some data could not be saved.\n";
            }
            cout << "\n";
            cout << "System shutting down safely.\n";
            cout << "Thank you for using the Hospital Emergency\n";
            cout << "Management System.\n";
            break;
        }
        // INVALID OPTION
        else {
            cout << "[ERROR] Invalid choice.\n";
            cout << "Please select an option from 1 to 16.\n";
            pauseScreen();
        }
    }
    return 0;
}