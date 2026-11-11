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

void correctInputCheckerForDate(std::string& animalStringData, bool withTime) // Анастасьєв
{
    while (true)
    {
        correctInputCheckerForString(animalStringData);

        bool correctFormat = animalStringData.size() == (withTime ? 14 : 8);

        if (correctFormat)
        {
            for (int i = 0; i < animalStringData.size(); i++)
            {
                if (i == 2 || i == 5)
                {
                    if (animalStringData[i] != '/')
                        correctFormat = false;
                }
                else if (i == 8)
                {
                    if (animalStringData[i] != ' ')
                        correctFormat = false;
                }
                else if (i == 11)
                {
                    if (animalStringData[i] != ':')
                        correctFormat = false;
                }
                else
                {
                    if (animalStringData[i] < '0' || animalStringData[i] > '9')
                        correctFormat = false;
                }
            }
        }

        if (correctFormat)
        {
            int day = std::stoi(animalStringData.substr(0, 2));
            int month = std::stoi(animalStringData.substr(3, 2));

            if (day < 1 || day > 31 || month < 1 || month > 12)
            {
                correctFormat = false;
            }

            if (withTime)
            {
                int hours = std::stoi(animalStringData.substr(9, 2));
                int minutes = std::stoi(animalStringData.substr(12, 2));

                if (hours > 23 || minutes > 59)
                {
                    correctFormat = false;
                }
            }
        }

        if (correctFormat)
        {
            break;
        }

        std::cout << "Некоректне значення! Введіть "
            << (withTime ? "DD/MM/YY XX:XX: " : "DD/MM/YY: ");
    }
}
