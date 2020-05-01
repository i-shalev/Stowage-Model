//
// Created by itay on 05/04/2020.
//

#ifndef STOWAGE_MODEL_WEIGHTBALANCERCALCULATOR_H
#define STOWAGE_MODEL_WEIGHTBALANCERCALCULATOR_H

#pragma once

class WeightBalancerCalculator {
public:
    enum BalanceStatus  {APPROVED, X_IMBALANCED , Y_IMBALANCED , X_Y_IMBALANCED  };
    BalanceStatus tryOperation(char loadUnload, int kg, int X, int Y);
};


#endif //STOWAGE_MODEL_WEIGHTBALANCERCALCULATOR_H
