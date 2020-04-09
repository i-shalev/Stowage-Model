//
// Created by zivco on 05/04/2020.
//

#ifndef STOWAGE_MODEL_ALGO_H
#define STOWAGE_MODEL_ALGO_H

#include "Ship.h"
#include "Balancer.h"
#include <cstdio>
#include "fstream"
#include "Files.h"

class Algo{
    Ship* ship;
    //BalanceCalc* calc;
public:
    explicit Algo(Ship* _ship): ship(_ship){}
    void getInstructionForCargo(const std::string& outputPath);
private:
    int emptyPlacesInPosition(int i, int j, const string& portSymbol);
    bool checkContainer(Container *cont);
};


#endif //STOWAGE_MODEL_ALGO_H
