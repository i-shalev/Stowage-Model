//
// Created by zivco on 05/04/2020.
//

#ifndef STOWAGE_MODEL_NAIVEALGO_H
#define STOWAGE_MODEL_NAIVEALGO_H

#include "Ship.h"
#include "WeightBalancerCalculator.h"
#include <cstdio>
#include "fstream"
#include "Files.h"

// this algorithm sort the Containers by their destinations from closest to farthest and Load them one by one.
class NaiveAlgo{
    Ship* ship;
    WeightBalancerCalculator* calc;
public:
    explicit NaiveAlgo(Ship* _ship): ship(_ship){}
    void getInstructionForCargo(const std::string& outputPath);
private:
    int emptyPlacesInPosition(int i, int j, const std::string& portSymbol);
    bool checkContainer(Container *cont);
};


#endif //STOWAGE_MODEL_NAIVEALGO_H
