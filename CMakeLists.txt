cmake_minimum_required(VERSION 3.16)

project(HospitalEmergencyManagement)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

# INCLUDE DIRECTORY


include_directories(include)


# APPLICATION SOURCE FILES


set(SOURCES

    src/Patient.cpp
    src/Doctor.cpp
    src/TriageManager.cpp
    src/DoctorManager.cpp
    src/FileManager.cpp
    src/StatisticsManager.cpp
    src/Hospital.cpp

)


# MAIN APPLICATION


add_executable(

    HospitalEmergencyManagement

    ${SOURCES}

    src/main.cpp

)