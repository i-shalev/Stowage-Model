//
// Created by itay on 13/06/2020.
//

#ifndef STOWAGE_MODEL__319088373_D_H
#define STOWAGE_MODEL__319088373_D_H

#include "../common/Ship.h"
#include "../common/WeightBalanceCalculator.h"
#include "../common/Crane.h"
#include <cstdio>
#include "fstream"
#include "../common/Files.h"
#include "../common/AbstractAlgorithm.h"
#include "../common/AlgorithmRegistration.h"

// this algorithm sort the Containers by their destinations from closest to farthest and Load them one by one.
class _319088373_d : public AbstractAlgorithm{
    Ship* ship;
    ShipPlan* shipPlan;
    ShipRoute* shipRoute;
    WeightBalanceCalculator* calc;
public:
    _319088373_d();
    ~_319088373_d(){delete ship;}
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


#endif //STOWAGE_MODEL__319088373_D_H

