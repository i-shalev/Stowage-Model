//
// Created by itay on 11/04/2020.
//

#ifndef STOWAGE_MODEL_NAIVEALGOWITHTRICK_H
#define STOWAGE_MODEL_NAIVEALGOWITHTRICK_H

#include "Ship.h"
#include "WeightBalanceCalculator.h"
#include <cstdio>
#include "fstream"
#include "Files.h"

// this algorithm sort the Containers by their destinations from closest to farthest.
// than calculate how much space is left on the boat and Load the containers that should get on the ship from the
// farthest to the closest.
class NaiveAlgoWithTrick {
    Ship* ship;
    WeightBalanceCalculator* calc;
public:
    explicit NaiveAlgoWithTrick(Ship* _ship): ship(_ship){}
    void getInstructionForCargo(const std::string& outputPath);
private:
    int emptyPlacesInPosition(int i, int j, const std::string& portSymbol);
    bool checkContainer(Container *cont);
};

int min(int x, int y);

#endif //STOWAGE_MODEL_NAIVEALGOWITHTRICK_H
