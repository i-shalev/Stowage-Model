//
// Created by zivco on 01/05/2020.
//

#ifndef STOWAGE_MODEL_ABSTRACTALGORITHM_H
#define STOWAGE_MODEL_ABSTRACTALGORITHM_H

// AbstractAlgorithm.h

#pragma once
#include <string>

// forward declaration
class WeightBalanceCalculator;

class AbstractAlgorithm {
public:
    virtual ~AbstractAlgorithm(){}

    enum class Action { LOAD = 'L', UNLOAD = 'U', MOVE = 'M', REJECT = 'R'};

// methods below return int for returning 0 for success
// and any other number as error code
    virtual int readShipPlan(const std::string& full_path_and_file_name) = 0;
    virtual int readShipRoute(const std::string& full_path_and_file_name) = 0;
    virtual int setWeightBalanceCalculator(WeightBalanceCalculator& calculator) = 0;
    virtual int getInstructionsForCargo(
            const std::string& input_full_path_and_file_name,
            const std::string& output_full_path_and_file_name) = 0;
};

#endif //STOWAGE_MODEL_ABSTRACTALGORITHM_H
