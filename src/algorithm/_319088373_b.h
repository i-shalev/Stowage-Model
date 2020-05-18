//
// Created by zivco on 08/05/2020.
//

#ifndef STOWAGE_MODEL__319088373_B_H
#define STOWAGE_MODEL__319088373_B_H

#include "../common/Ship.h"
#include "../common/WeightBalanceCalculator.h"
#include "../common/Crane.h"
#include <cstdio>
#include "fstream"
#include "../common/Files.h"
#include "../common/AbstractAlgorithm.h"
#include "../common/AlgorithmRegistration.h"


// this algorithm sort the Containers by their destinations from closest to farthest and Load them one by one.
class _319088373_b : public AbstractAlgorithm{
    Ship* ship;
    ShipPlan* shipPlan;
    ShipRoute* shipRoute;
    WeightBalanceCalculator* calc;
public:

    _319088373_b();
    ~_319088373_b(){delete ship;}
    int getInstructionsForCargo(const std::string& input_full_path_and_file_name, const std::string& output_full_path_and_file_name) override ;
    int setWeightBalanceCalculator(WeightBalanceCalculator& calculator) override;
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
