#include <iostream>
#include <string>
#include <limits>

void correctInputCheckerForInt(int& value) // Анастасьєв
{
    std::cin >> value;

    while (std::cin.fail() || std::cin.peek() != '\n')
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Некоректне значення! Введіть ціле число: ";
        std::cin >> value;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void correctInputCheckerForString(std::string& animalStringData) // Анастасьєв
{
    while (true)
    {
        std::getline(std::cin, animalStringData);

        while (!animalStringData.empty() &&
            (animalStringData[0] == ' ' || animalStringData[0] == '\t'))
        {
            animalStringData.erase(0, 1);
        }

        bool hasAtLeastOneSymbol = false;

        for (int i = 0; i < animalStringData.size(); i++)
        {
            if (animalStringData[i] != ' ' && animalStringData[i] != '\t')
            {
                hasAtLeastOneSymbol = true;
                break;
            }
        }

        if (hasAtLeastOneSymbol)
        {
            break;
        }

        std::cout << "Некоректне значення! Потрібен текст: ";
    }
}


void correctInputCheckerForDateTime(std::string& dateTime)
{
    bool correct;

    do
    {
        correctInputCheckerForString(dateTime);
        correct = false;
        if (dateTime.size() == 14 &&
            dateTime[2] == '/' &&
            dateTime[5] == '/' &&
            dateTime[8] == ' ' &&
            dateTime[11] == ':')
        {
            bool digitsCorrect = true;

            for (int i = 0; i < 14; i++)
            {
                if (i == 2 || i == 5 || i == 8 || i == 11)
                    continue;

                if (dateTime[i] < '0' || dateTime[i] > '9')
                    digitsCorrect = false;
            }

            if (digitsCorrect)
            {
                int day = std::stoi(dateTime.substr(0, 2));
                int month = std::stoi(dateTime.substr(3, 2));
                int year = 2000 + std::stoi(dateTime.substr(6, 2));
                int hour = std::stoi(dateTime.substr(9, 2));
                int minute = std::stoi(dateTime.substr(12, 2));

                int days[] = {
                    31, 28, 31, 30, 31, 30,
                    31, 31, 30, 31, 30, 31
                };

                if (year % 4 == 0 &&
                    (year % 100 != 0 || year % 400 == 0))
                    days[1] = 29;

                if (month >= 1 && month <= 12 &&
                    day >= 1 && day <= days[month - 1] &&
                    hour >= 0 && hour <= 23 &&
                    minute >= 0 && minute <= 59)
                {
                    correct = true;
                }
            }
        }

        if (!correct)
            std::cout << "Некоректна дата або час! "
                      << "Введіть коректне значення (DD/MM/YY HH:MM): ";

    } while (!correct);
}
