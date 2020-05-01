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
#include "AbstractAlgorithm.h"

// this algorithm sort the Containers by their destinations from closest to farthest.
// than calculate how much space is left on the boat and Load the containers that should get on the ship from the
// farthest to the closest.
class NaiveAlgoWithTrick : public AbstractAlgorithm{
    Ship* ship;
    WeightBalanceCalculator* calc;
public:
    explicit NaiveAlgoWithTrick(Ship* _ship): ship(_ship){}
    int getInstructionsForCargo(const std::string& input_full_path_and_file_name, const std::string& output_full_path_and_file_name) override ;
    int setWeightBalanceCalculator(WeightBalanceCalculator& calculator) override { this->calc = calc; return 0;}
    int readShipPlan(const std::string& full_path_and_file_name) override{return 0;}
    int readShipRoute(const std::string& full_path_and_file_name) override{return 0;}
private:
    int emptyPlacesInPosition(int i, int j, const std::string& portSymbol);
    bool checkContainer(Container *cont);
};

int min(int x, int y);

#endif //STOWAGE_MODEL_NAIVEALGOWITHTRICK_H
