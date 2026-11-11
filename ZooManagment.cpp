#define NOMINMAX

#include <iostream>
#include <windows.h>
#include <vector>
#include "Structures.h"
#include "CorrectInputChecker.h"
#include "AnimalManager.h"

bool Authentication(const std::vector<Accounts>& accounts, Role& currentRole)    // Феоктістова
{
    std::string login;
    std::string password;

    std::cout << "Логін та пароль (login password): ";
    std::cin >> login >> password;

    for (const Accounts& account : accounts)
    {
        if (account.login == login && account.password == password)
        {
            currentRole = account.role;

            std::cout << "Авторизація успішна!" << std::endl;
            return true;
        }
    }

    std::cout << "Неправильний логін або пароль!" << std::endl;
    return false;
}

void addNewAccount(std::vector<Accounts>& accounts)    // Феоктістова
{
    Accounts newAccount;

    std::cout << "Введіть логін нового користувача: ";
    std::cin >> newAccount.login;

    for (const Accounts& account : accounts)
    {
        if (account.login == newAccount.login)
        {
            std::cout << "Користувач із таким логіном вже існує!" << std::endl;
            return;
        }
    }

    std::cout << "Введіть пароль: ";
    std::cin >> newAccount.password;

    int choice;
    std::cout << "Оберіть роль (1 - працівник, 2 - ветеринар): ";
    do
    {
        correctInputCheckerForInt(choice);
        if (choice != 1 && choice != 2)
        {
            std::cout << "Некоректний вибір! Введіть 1 або 2: ";
        }
    } while (choice != 1 && choice != 2);

    newAccount.role = choice == 1 ? Role::Caregiver : Role::Veterinar;

    accounts.push_back(newAccount);

    std::cout << "Акаунт успішно створено!" << std::endl;
}

void startOutput(Role currentRole)    // Феоктістова
{
    std::cout << "===== ZOO MANAGEMENT SYSTEM =====" << std::endl;
    std::cout << std::endl;

    if (currentRole == Role::Veterinar)
    {
        std::cout << "1. Переглянути всіх тварин" << std::endl;
        std::cout << "2. Знайти тварину" << std::endl;
        std::cout << "3. Редагувати медичні дані" << std::endl;
        std::cout << "4. Внести дані ветеринарного огляду" << std::endl;
        std::cout << "5. Переглянути медичні дані" << std::endl;
        std::cout << "6. Вийти з акаунта" << std::endl;
    }
    else
    {
        std::cout << "1. Додати тварину" << std::endl;
        std::cout << "2. Переглянути всіх тварин" << std::endl;
        std::cout << "3. Знайти тварину" << std::endl;
        std::cout << "4. Редагувати тварину" << std::endl;
        std::cout << "5. Видалити тварину" << std::endl;
        std::cout << "6. Переглянути медичні дані" << std::endl;
        if (currentRole == Role::Admin)
        {
            std::cout << "7. Створити акаунт" << std::endl;
            std::cout << "8. Вийти з акаунта" << std::endl;
        }
        else
        {
            std::cout << "7. Вийти з акаунта" << std::endl;
        }
    }

    std::cout << "0. Вихід" << std::endl;
    std::cout << std::endl;
    std::cout << "| Оберіть дію: ";
}

int main()    // Феоктістова
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int lastAnimalIdentificationNumber = 10000000;

    std::vector<Animal> animals;

    std::vector<Accounts> accounts =
    {
        {"admin", "admin", Role::Admin}
    };

    Role currentRole = Role::Admin;

    while (!Authentication(accounts, currentRole))
    {
    }

    while (true)
    {
        int choice;

        startOutput(currentRole);
        correctInputCheckerForInt(choice);

        if (currentRole == Role::Veterinar)
        {
            switch (choice)
            {
            case 1:
                showAnimalList(animals);
                break;
            case 2:
                searchAnimalByIdentificationNumber(animals);
                break;
            case 3:
                if (currentRole == Role::Veterinar)
                {
                    editMedicalData(animals);
                }
                else
                {
                    std::cout << "Недостатньо прав доступу!" << std::endl;
                }
                break;
            case 4:
                addVeterinaryExamination(animals);
                break;
            case 5:
                showMedicalData(animals);
                break;

            case 6:
                while (!Authentication(accounts, currentRole))
                {
                }
                break;
            case 0:
                return 0;
            default:
                std::cout << "Некоректний вибір!" << std::endl;
                break;
            }
            std::cout << std::endl;
            continue;
        }

        switch (choice)
        {
        case 1:
            if (currentRole != Role::Veterinar)
            {
                addNewAnimal(animals, lastAnimalIdentificationNumber);
            }
            else
            {
                std::cout << "Недостатньо прав доступу!" << std::endl;
            }
            break;

        case 2:
            showAnimalList(animals);
            break;

        case 3:
            searchAnimalByIdentificationNumber(animals);
            break;

        case 4:
            if (currentRole != Role::Veterinar)
            {
                editDataOfAnimal(animals);
            }
            else
            {
                std::cout << "Недостатньо прав доступу!" << std::endl;
            }
            break;

        case 5:
            if (currentRole != Role::Veterinar)
            {
                deleteAnimalFromVector(animals);
            }
            else
            {
                std::cout << "Недостатньо прав доступу!" << std::endl;
            }
            break;

        case 6:
            showMedicalData(animals);
            break;

        case 7:
            if (currentRole == Role::Admin)
            {
                addNewAccount(accounts);
            }
            else
            {
                while (!Authentication(accounts, currentRole))
                {
                }
            }
            break;

        case 8:
            if (currentRole == Role::Admin)
            {
                while (!Authentication(accounts, currentRole))
                {
                }
            }
            else
            {
                std::cout << "Некоректний вибір!" << std::endl;
            }
            break;

        case 0:
            return 0;

        default:
            std::cout << "Некоректний вибір!" << std::endl;
            break;
        }
    }
}
