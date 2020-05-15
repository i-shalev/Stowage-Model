//
// Created by zivco on 08/05/2020.
//

#ifndef STOWAGE_MODEL__319088373_B_H
#define STOWAGE_MODEL__319088373_B_H

#include "../Common/Ship.h"
#include "../Common/WeightBalanceCalculator.h"
#include "../Common/Crane.h"
#include <cstdio>
#include "fstream"
#include "../Common/Files.h"
#include "../Common/AbstractAlgorithm.h"
#include "../Common/AlgorithmRegistration.h"


// this algorithm sort the Containers by their destinations from closest to farthest and Load them one by one.
class _319088373_b : public AbstractAlgorithm{
    Ship* ship;
    ShipPlan* shipPlan;
    ShipRoute* shipRoute;
    WeightBalanceCalculator* calc;
public:

    _319088373_b();
    int getInstructionsForCargo(const std::string& input_full_path_and_file_name, const std::string& output_full_path_and_file_name) override ;
    int setWeightBalanceCalculator(WeightBalanceCalculator& calculator) override { this->calc = &calculator; return 0;}
    int readShipPlan(const std::string& full_path_and_file_name) override;
    int readShipRoute(const std::string& full_path_and_file_name) override;
    void printShipPlan();
    void printShipRoute();
private:
    int emptyPlacesInPosition(int i, int j, const std::string& portSymbol);
    bool checkContainer(Container *cont);
    void createShip();
};

int turnToTrueBit(int num, int bit);
int min(int x, int y);

#endif //STOWAGE_MODEL__319088373_B_H
