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
            if (animalStringData[i] != ' ')
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
