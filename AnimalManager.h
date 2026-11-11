#pragma once

#include <vector>
#include "Structures.h"

void addNewAnimal(std::vector<Animal>& animals, int& lastAnimalIdentificationNumber);

void showAnimalList(const std::vector<Animal>& animals);

int binaryCycleForNeededAnimalSearch(const std::vector<Animal>& animals, int identificatorOfAnimalToBeFound);

void searchAnimalByIdentificationNumber(const std::vector<Animal>& animals);

void editDataOfAnimal(std::vector<Animal>& animals);

void deleteAnimalFromVector(std::vector<Animal>& animals);

void editMedicalData(std::vector<Animal>& animals);

void addVeterinaryExamination(std::vector<Animal>& animals);

void showMedicalData(const std::vector<Animal>& animals);
