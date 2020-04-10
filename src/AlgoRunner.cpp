//
// Created by itay on 10/04/2020.
//

#include "AlgoRunner.h"

AlgoRunner::AlgoRunner(AlgoType _algoType, string _pathToDir) {
    this->pathToDir = _pathToDir;
    this->algoType = _algoType;
    this->sumOperations = 0;
}
