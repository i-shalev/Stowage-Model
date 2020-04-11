//
// Created by itay on 11/04/2020.
//

#ifndef STOWAGE_MODEL_NAIVEALGOWITHTRICK_H
#define STOWAGE_MODEL_NAIVEALGOWITHTRICK_H

#include "Ship.h"
#include "Balancer.h"
#include <cstdio>
#include "fstream"
#include "Files.h"

class NaiveAlgoWithTrick {
    Ship* ship;
    //BalanceCalc* calc;
public:
    explicit NaiveAlgoWithTrick(Ship* _ship): ship(_ship){}
    void getInstructionForCargo(const std::string& outputPath);
private:
    int emptyPlacesInPosition(int i, int j, const string& portSymbol);
    bool checkContainer(Container *cont);
};

int min(int x, int y);

#endif //STOWAGE_MODEL_NAIVEALGOWITHTRICK_H
