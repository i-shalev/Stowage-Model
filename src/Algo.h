//
// Created by zivco on 05/04/2020.
//

#ifndef STOWAGE_MODEL_ALGO_H
#define STOWAGE_MODEL_ALGO_H

#include "Ship.h"

class Algo{
    Ship* ship;
    //BalanceCalc* calc;
public:
    Algo(Ship* _ship): ship(_ship){}
    void getInstructionForCargo(Port& port);
private:
    int emptyPlacesInPosition(int i, int j, string portSymbol);
    bool checkContainer(Container *cont);
};


#endif //STOWAGE_MODEL_ALGO_H