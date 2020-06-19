//
// Created by itay on 16/06/2020.
//

#ifndef STOWAGE_MODEL_RUNNERALLTRAVELSALLALGO_H
#define STOWAGE_MODEL_RUNNERALLTRAVELSALLALGO_H

#include <algorithm>
#include "AlgorithmRegistrar.h"
#include "ThreadPoolExecuter.h"
#include "TasksProducer.h"
#include "RunnerHelper.h"
#include "../common/Crane.h"
#include "../common/Ship.h"
#include "../common/WeightBalanceCalculator.h"




#define PATH_TO_EMPTY_FILE R"(.\empty.empty_file)"

class RunnerAllTravelsAllAlgo {

};

void runThreads(int numThreads, const std::string& algoPath, const std::string &travelPath, const std::string &outputPath);

void runAllAlgo(const std::string& algoPath, const std::string &travelPath, const std::string &outputPath);
void runAlgoForAllTravels(AlgorithmRegistrar::const_iterator *algoFactory, const std::string &travelPath,
        const std::string &outputPath, const std::string &algoName, std::vector<std::string>* dirs,
        std::vector<std::vector<int>>& results, std::vector<int>& sums, std::vector<int>& numErrs, int index);
int runAlgoForTravel(AbstractAlgorithm &algo, const std::string &pathToDir, const std::string &outputPath,
        const std::string &algoName, const std::string &travelName);
int runAlgoOnPort(Ship *ship, const std::string& cargoDataPath, const std::string& instructionsPath,
        std::vector<std::string>& errorReason);
bool validateFarRejected(std::vector<Container*>& left, std::vector<Container*>& was, std::vector<Container*> contByDist);
bool validTravel(const std::string& travelPath, const std::string& outputPath, const std::string& dirName);


#endif //STOWAGE_MODEL_RUNNERALLTRAVELSALLALGO_H
