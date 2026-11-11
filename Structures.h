#pragma once
#include <string>

struct Animal
{
    std::string nameOfAnimal;
    std::string typeOfAnimal;
    int animalIdentificationNumber = 0;
    std::string genderOfAnimal;
    int ageOfAnimal = 0;
    std::string healthStatus;
    int enclosureID = 0;
    std::string lastFeeding;
    std::string lastVeterinarCheckDate;
    std::string treatmentPlan;
    std::string veterinaryExaminationResult;

};

enum class Role
{
    Admin,
    Caregiver,
    Veterinar
};

struct Accounts
{
    std::string login;
    std::string password;
    Role role;
};
