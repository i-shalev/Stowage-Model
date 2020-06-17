//
// Created by itay on 05/04/2020.
//

#include "../common/WeightBalanceCalculator.h"

//the parameters are in comment in order to avoid unused variables warnings
WeightBalanceCalculator::BalanceStatus WeightBalanceCalculator::tryOperation(char loadUnload, int kg, int x, int y) {
    (void) loadUnload;
    (void) kg;
    (void) x;
    (void) y;
    return WeightBalanceCalculator::BalanceStatus::APPROVED;
}
