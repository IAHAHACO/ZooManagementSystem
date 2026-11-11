#include <iostream>
#include <algorithm>
#include <array>

#include "AnimalManager.h"
#include "CorrectInputChecker.h"

void addNewAnimal(std::vector<Animal>& animals, int& lastAnimalIdentificationNumber)    // Феоктістова
{
    std::string newAnimalName;
    std::string typeOfNewAnimal;
    std::string genderOfNewAnimal;
    std::string lastFeedingTime;
    int newAnimalIdentificationNumber;
    int newAnimalAge;
    int newAnimalEnclosureID;
    Animal newAnimalData;
    int choice = 0;

    std::cout << "Введіть ім'я нової тварини: ";
    correctInputCheckerForString(newAnimalName);

    std::cout << "Введіть вид нової тварини: ";
    correctInputCheckerForString(typeOfNewAnimal);

    std::cout << "Оберіть стать нової тварини (1 - самець, 2 - самка): ";
    do
    {
        correctInputCheckerForInt(choice);

        if (choice != 1 && choice != 2)
        {
            std::cout << "Некоректний вибір! Введіть 1 або 2: ";
        }
    } while (choice != 1 && choice != 2);

    choice == 1 ? genderOfNewAnimal = "Самець" : genderOfNewAnimal = "Самка";

    std::cout << "Введіть вік нової тварини: ";
    do
    {
        correctInputCheckerForInt(newAnimalAge);

        if (newAnimalAge < 0)
        {
            std::cout << "Помилка! Введіть коректне значення: ";
        }
    } while (newAnimalAge < 0);

    std::cout << "Введіть ID вольєру нової тварини: ";
    do
    {
        correctInputCheckerForInt(newAnimalEnclosureID);

        if (newAnimalEnclosureID <= 0)
        {
            std::cout << "Помилка! Введіть коректне значення: ";
        }
    } while (newAnimalEnclosureID <= 0);

    std::cout << "Введіть час останнього годування (DD/MM/YY XX:XX): ";
    correctInputCheckerForString(lastFeedingTime);

    lastAnimalIdentificationNumber++;
    newAnimalIdentificationNumber = lastAnimalIdentificationNumber;

    newAnimalData.nameOfAnimal = newAnimalName;
    newAnimalData.typeOfAnimal = typeOfNewAnimal;
    newAnimalData.animalIdentificationNumber = newAnimalIdentificationNumber;
    newAnimalData.genderOfAnimal = genderOfNewAnimal;
    newAnimalData.ageOfAnimal = newAnimalAge;
    newAnimalData.healthStatus = "Не вказано";
    newAnimalData.enclosureID = newAnimalEnclosureID;
    newAnimalData.lastFeeding = lastFeedingTime;
    newAnimalData.lastVeterinarCheckDate = "Не вказано";

    animals.push_back(newAnimalData);

    std::cout << "Тварину додано успішно!" << std::endl;
}

namespace
{
    using AnimalTableRow = std::array<std::string, 7>;
    using AnimalTableWidths = std::array<std::size_t, 7>;

    const AnimalTableRow animalTableHeaders =
    {
        "ID", "Кличка", "Вид", "Стать", "Вік", "Вольєр", "Останнє годування"
    };

    std::size_t utf8CharacterCount(const std::string& text)
    {
        std::size_t count = 0;
        // Українські літери в UTF-8 займають кілька байтів, але один стовпець.
        for (unsigned char byte : text)
        {
            if ((byte & 0xC0) != 0x80)
            {
                ++count;
            }
        }
        return count;
    }

    AnimalTableRow animalTableRow(const Animal& animal)
    {
        return {std::to_string(animal.animalIdentificationNumber), animal.nameOfAnimal,
            animal.typeOfAnimal, animal.genderOfAnimal, std::to_string(animal.ageOfAnimal),
            std::to_string(animal.enclosureID), animal.lastFeeding};
    }

    void updateAnimalTableWidths(AnimalTableWidths& widths, const AnimalTableRow& row)
    {
        for (std::size_t column = 0; column < row.size(); ++column)
        {
            widths[column] = std::max(widths[column], utf8CharacterCount(row[column]));
        }
    }

    void printAnimalTableRow(const AnimalTableRow& row, const AnimalTableWidths& widths)
    {
        std::cout << "|";
        for (std::size_t column = 0; column < row.size(); ++column)
        {
            std::cout << " " << row[column]
                << std::string(widths[column] - utf8CharacterCount(row[column]), ' ')
                << " |";
        }
        std::cout << std::endl;
    }
}

void outputAnimalData(const Animal& animal)    // Анастасьєв
{
    const AnimalTableRow row = animalTableRow(animal);
    AnimalTableWidths widths{};
    updateAnimalTableWidths(widths, animalTableHeaders);
    updateAnimalTableWidths(widths, row);
    printAnimalTableRow(row, widths);
}

void showAnimalList(const std::vector<Animal>& animals)    // Шимко
{
    if (animals.empty())
    {
        std::cout << "Вивід пустого списку неможливий!" << std::endl;
        return;
    }

    std::cout << "===== СПИСОК ТВАРИН =====" << std::endl;
    std::cout << std::endl;
    AnimalTableWidths widths{};
    updateAnimalTableWidths(widths, animalTableHeaders);
    // Однакові ширини для заголовка та всіх тварин, без обрізання тексту.
    for (const Animal& animal : animals)
    {
        updateAnimalTableWidths(widths, animalTableRow(animal));
    }

    printAnimalTableRow(animalTableHeaders, widths);
    for (const Animal& animal : animals)
    {
        printAnimalTableRow(animalTableRow(animal), widths);
    }
}

int binaryCycleForNeededAnimalSearch(const std::vector<Animal>& animals, int identificatorOfAnimalToBeFound)    // Шимко
{
    if (animals.empty())
    {
        return -1;
    }

    int leftIndexOfTheVector = 0,
        rightIndexOfTheVector = animals.size() - 1,
        middlePartOfTheVector;

    while (leftIndexOfTheVector <= rightIndexOfTheVector)
    {
        middlePartOfTheVector = (leftIndexOfTheVector + rightIndexOfTheVector) / 2;

        if (animals[middlePartOfTheVector].animalIdentificationNumber == identificatorOfAnimalToBeFound)
        {
            return middlePartOfTheVector;
        }
        else if (animals[middlePartOfTheVector].animalIdentificationNumber < identificatorOfAnimalToBeFound)
        {
            leftIndexOfTheVector = middlePartOfTheVector + 1;
        }
        else
        {
            rightIndexOfTheVector = middlePartOfTheVector - 1;
        }
    }

    return -1;
}

void searchAnimalByIdentificationNumber(const std::vector<Animal>& animals)    // Шимко
{
    int identificatorOfAnimalToBeFound;

    if (animals.empty())
    {
        std::cout << "У списку немає даних для пошуку!" << std::endl;
        return;
    }

    std::cout << "Введіть ID тварини: ";
    correctInputCheckerForInt(identificatorOfAnimalToBeFound);

    int vectorElementOfSearchedAnimal = binaryCycleForNeededAnimalSearch(animals, identificatorOfAnimalToBeFound);

    if (vectorElementOfSearchedAnimal < 0)
    {
        std::cout << "Тварину з ID "
            << identificatorOfAnimalToBeFound << " не вдалось знайти." << std::endl;
        return;
    }

    outputAnimalData(animals[vectorElementOfSearchedAnimal]);
}

void editDataOfAnimal(std::vector<Animal>& animals)    // Анастасьєв
{
    if (animals.empty())
    {
        std::cout << "У списку немає даних для редагування!" << std::endl;
        return;
    }

    int choiceOfActionsMenu;
    int identificatorOfAnimalToBeFound;
    std::string newStringDataOfTheAnimal;
    int newIntDataOfTheAnimal = 0;

    std::cout << "Введіть ID тварини: ";
    correctInputCheckerForInt(identificatorOfAnimalToBeFound);

    int vectorElementOfSearchedAnimal =
        binaryCycleForNeededAnimalSearch(animals, identificatorOfAnimalToBeFound);

    if (vectorElementOfSearchedAnimal < 0)
    {
        std::cout << "Тварину з ID "
            << identificatorOfAnimalToBeFound << " не вдалось знайти." << std::endl;
        return;
    }

    Animal& animal = animals[vectorElementOfSearchedAnimal];

    std::cout << "1. Змінити ім'я тварини" << std::endl;
    std::cout << "2. Змінити вид тварини" << std::endl;
    std::cout << "3. Змінити стать тварини" << std::endl;
    std::cout << "4. Змінити вік тварини" << std::endl;
    std::cout << "5. Змінити вольєр тварини" << std::endl;
    std::cout << "6. Змінити час останнього годування" << std::endl;
    std::cout << "0. Повернутись" << std::endl;
    std::cout << std::endl;

    std::cout << "Виберіть пункт меню: ";
    correctInputCheckerForInt(choiceOfActionsMenu);

    switch (choiceOfActionsMenu)
    {
    case 1:
        std::cout << "Введіть нове ім'я тварини: ";
        correctInputCheckerForString(newStringDataOfTheAnimal);

        animal.nameOfAnimal = newStringDataOfTheAnimal;
        std::cout << "Ім'я тварини змінено успішно!" << std::endl;
        break;
    case 2:
        std::cout << "Введіть новий тип тварини: ";
        correctInputCheckerForString(newStringDataOfTheAnimal);

        animal.typeOfAnimal = newStringDataOfTheAnimal;

        std::cout << "Тип тварини змінено успішно!" << std::endl;
        break;
    case 3:
        std::cout << "Введіть нову стать тварини (1 - самець, 2 - самка): ";
        do
        {
            correctInputCheckerForInt(newIntDataOfTheAnimal);

            if (newIntDataOfTheAnimal != 1 && newIntDataOfTheAnimal != 2)
            {
                std::cout << "Некоректний вибір! Введіть 1 або 2: ";
            }
        } while (newIntDataOfTheAnimal != 1 && newIntDataOfTheAnimal != 2);
        newIntDataOfTheAnimal == 1 ? animal.genderOfAnimal = "Самець" :
            animal.genderOfAnimal = "Самка";
        break;
    case 4:
        std::cout << "Введіть новий вік тварини: ";
        do
        {
            correctInputCheckerForInt(newIntDataOfTheAnimal);

            if (newIntDataOfTheAnimal < 0)
            {
                std::cout << "Помилка! Введіть коректне значення: ";
            }
        } while (newIntDataOfTheAnimal < 0);
        animal.ageOfAnimal = newIntDataOfTheAnimal;
        break;
    case 5:
        std::cout << "Введіть новий ID вольєру тварини: ";
        do
        {
            correctInputCheckerForInt(newIntDataOfTheAnimal);

            if (newIntDataOfTheAnimal <= 0)
            {
                std::cout << "Помилка! Введіть коректне значення: ";
            }
        } while (newIntDataOfTheAnimal <= 0);
        animal.enclosureID = newIntDataOfTheAnimal;
        break;
    case 6:
        std::cout << "Введіть дату та час останнього годування (DD/MM/YY XX:XX): ";
        correctInputCheckerForString(newStringDataOfTheAnimal);

        animal.lastFeeding = newStringDataOfTheAnimal;

        std::cout << "Час останнього годування змінено успішно!" << std::endl;
        break;
    case 0:
        break;
    default:
        std::cout << "Помилка! Введіть корректне значення! " << std::endl;
        break;
    }
}

void deleteAnimalFromVector(std::vector<Animal>& animals)    // Шимко
{
    int identificatorOfAnimalToBeDeleted;

    if (animals.empty())
    {
        std::cout << "У списку немає даних для видалення!" << std::endl;
        return;
    }

    std::cout << "Введіть ID тварини: ";
    correctInputCheckerForInt(identificatorOfAnimalToBeDeleted);

    int vectorElementOfSearchedAnimal = binaryCycleForNeededAnimalSearch(animals, identificatorOfAnimalToBeDeleted);

    if (vectorElementOfSearchedAnimal < 0)
    {
        std::cout << "Тварину з ID "
            << identificatorOfAnimalToBeDeleted << " не вдалось знайти." << std::endl;
        return;
    }

    int confirmationOfDelete;
    std::cout << "Ви підтверджуєте видалення тварини? (1 - так / 0 - ні): ";
    correctInputCheckerForInt(confirmationOfDelete);

    while (confirmationOfDelete != 1 && confirmationOfDelete != 0)
    {
        std::cout << "Введіть коректне значення!: ";
        correctInputCheckerForInt(confirmationOfDelete);
    }

    if (confirmationOfDelete == 0)
    {
        return;
    }
    if (confirmationOfDelete == 1)
    {
        animals.erase(animals.begin() + vectorElementOfSearchedAnimal);
        std::cout << "Тварину видалено успішно!" << std::endl;
    }
}

void editMedicalData(std::vector<Animal>& animals)    // Анастасьєв
{
    if (animals.empty())
    {
        std::cout << "У списку немає даних!" << std::endl;
        return;
    }

    int identificatorOfAnimalToBeFound;
    std::cout << "Введіть ID тварини: ";
    correctInputCheckerForInt(identificatorOfAnimalToBeFound);

    int vectorElementOfSearchedAnimal =
        binaryCycleForNeededAnimalSearch(animals, identificatorOfAnimalToBeFound);

    if (vectorElementOfSearchedAnimal < 0)
    {
        std::cout << "Тварину не вдалось знайти!" << std::endl;
        return;
    }

    Animal& animal = animals[vectorElementOfSearchedAnimal];

    int choiceOfActionsMenu;
    int newIntDataOfTheAnimal;
    std::string newStringDataOfTheAnimal;

    std::cout << "1. Змінити стан здоров'я та діагноз" << std::endl;
    std::cout << "2. Змінити план лікування" << std::endl;
    std::cout << "0. Повернутись" << std::endl;
    std::cout << "Вибір: ";
    correctInputCheckerForInt(choiceOfActionsMenu);

    switch (choiceOfActionsMenu)
    {
    case 1:
        std::cout << "Оберіть стан здоров'я (1 - Здоровий, 2 - Хворий): ";
        do
        {
            correctInputCheckerForInt(newIntDataOfTheAnimal);
            if (newIntDataOfTheAnimal != 1 && newIntDataOfTheAnimal != 2)
            {
                std::cout << "Некоректний вибір! Введіть 1 або 2: ";
            }
        } while (newIntDataOfTheAnimal != 1 && newIntDataOfTheAnimal != 2);

        if (newIntDataOfTheAnimal == 2)
        {
            std::cout << "Введіть діагноз: ";
            correctInputCheckerForString(newStringDataOfTheAnimal);
            animal.healthStatus = newStringDataOfTheAnimal;
        }
        else
        {
            animal.healthStatus = "Здоровий";
        }
        std::cout << "Стан здоров'я змінено!" << std::endl;
        break;
    case 2:
        std::cout << "Введіть план лікування: ";
        correctInputCheckerForString(newStringDataOfTheAnimal);
        animal.treatmentPlan = newStringDataOfTheAnimal;
        std::cout << "План лікування змінено!" << std::endl;
        break;
    case 0:
        break;
    default:
        std::cout << "Некоректний вибір!" << std::endl;
        break;
    }
}

void addVeterinaryExamination(std::vector<Animal>& animals)    // Феоктістова
{
    if (animals.empty())
    {
        std::cout << "У списку немає даних!" << std::endl;
        return;
    }

    int identificatorOfAnimalToBeFound;
    std::cout << "Введіть ID тварини: ";
    correctInputCheckerForInt(identificatorOfAnimalToBeFound);

    int vectorElementOfSearchedAnimal =
        binaryCycleForNeededAnimalSearch(animals, identificatorOfAnimalToBeFound);

    if (vectorElementOfSearchedAnimal < 0)
    {
        std::cout << "Тварину не вдалось знайти!" << std::endl;
        return;
    }

    Animal& animal = animals[vectorElementOfSearchedAnimal];

    std::string lastVeterinarCheck;
    std::string veterinaryExaminationResult;

    std::cout << "Введіть дату огляду (DD/MM/YY): ";
    correctInputCheckerForString(lastVeterinarCheck);
    std::cout << "Введіть результат огляду: ";
    correctInputCheckerForString(veterinaryExaminationResult);

    animal.lastVeterinarCheckDate = lastVeterinarCheck;
    animal.veterinaryExaminationResult = veterinaryExaminationResult;
    std::cout << "Дані ветеринарного огляду збережено!" << std::endl;
}


void showMedicalData(const std::vector<Animal>& animals)    // Шимко
{
    if (animals.empty())
    {
        std::cout << "У списку немає даних!" << std::endl;
        return;
    }

    int identificatorOfAnimalToBeFound;
    std::cout << "Введіть ID тварини: ";
    correctInputCheckerForInt(identificatorOfAnimalToBeFound);

    int vectorElementOfSearchedAnimal =
        binaryCycleForNeededAnimalSearch(animals, identificatorOfAnimalToBeFound);

    if (vectorElementOfSearchedAnimal < 0)
    {
        std::cout << "Тварину не вдалось знайти!" << std::endl;
        return;
    }

    const Animal& animal = animals[vectorElementOfSearchedAnimal];

    std::cout << "===== МЕДИЧНІ ДАНІ ТВАРИНИ =====" << std::endl;
    std::cout << "ID: " << animal.animalIdentificationNumber << std::endl;
    std::cout << "Ім'я: " << animal.nameOfAnimal << std::endl;
    std::cout << "Стан здоров'я / діагноз: " << animal.healthStatus << std::endl;
    std::cout << "План лікування: "
        << (animal.treatmentPlan.empty() ? "Не вказано" : animal.treatmentPlan) << std::endl;
    std::cout << "Дата останнього огляду: " << animal.lastVeterinarCheckDate << std::endl;
    std::cout << "Результат огляду: " << (animal.veterinaryExaminationResult.empty() ?
        "Не вказано" : animal.veterinaryExaminationResult) << std::endl;
}
