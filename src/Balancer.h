//
// Created by itay on 05/04/2020.
//

#ifndef STOWAGE_MODEL_BALANCER_H
#define STOWAGE_MODEL_BALANCER_H

#include <string>

enum BalanceStatus  {APPROVED, X_IMBALANCED , Y_IMBALANCED , X_Y_IMBALANCED  };

using namespace std;
class Balancer {

};

BalanceStatus tryOperation(char loadUnload, int kg, int x, int y);
int tryOperation(string path);

#endif //STOWAGE_MODEL_BALANCER_H
