//
// Created by itay on 10/04/2020.
//

#ifndef STOWAGE_MODEL_ALGORUNNER_H
#define STOWAGE_MODEL_ALGORUNNER_H
#include <string>

enum AlgoType  {NaiveAlgo};

using namespace std;
class AlgoRunner {
    AlgoType algoType;
    string pathToDir;
    int sumOperations;

public:
    AlgoRunner(AlgoType _algoType, string _pathToDir);
    AlgoRunner(const AlgoRunner& algoRunner)  = delete;
};



#endif //STOWAGE_MODEL_ALGORUNNER_H
