//
// Created by itay on 04/04/2020.
//

#ifndef STOWAGE_MODEL_MAIN_H
#define STOWAGE_MODEL_MAIN_H
#include <iostream>
#include "../common/Files.h"
#include "../common/Port.h"
#include "../common/Crane.h"
#include "../common/AbstractAlgorithm.h"
#include "AlgorithmRegistrar.h"
#include "TasksProducer.h"
#include "../common/WeightBalanceCalculator.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include "ThreadPoolExecuter.h"

//using namespace std;
class main {

};

int main(int argc, char **argv);
int turnToTrueBit(int num, int bit);
int createArgs(std::map<std::string, std::string>& args, int& argc, char **argv);
void printArgs(std::map<std::string, std::string>& args);
void runThreads(int numThreads, const std::string& algoPath, const std::string &travelPath, const std::string &outputPath);
void runAllAlgo(const std::string& algoPath, const std::string &travelPath, const std::string &outputPath);
void runAlgoForAllTravels(AlgorithmRegistrar::const_iterator *algoFactory, const std::string &travelPath, const std::string &outputPath,
                          const std::string &algoName, std::vector<std::string>* dirs);
int runAlgoForTravel(AbstractAlgorithm &algo, const std::string &pathToDir, const std::string &outputPath,
                     const std::string &algoName, const std::string &travelName);
int runAlgoOnPort(Ship *ship, const std::string& cargoDataPath, const std::string& instructionsPath, std::vector<std::string>& errorReason);
int getShipPlanAndRoutePaths(const std::string& pathToDir, std::string& shipPlanPath, std::string& shipRoutePath);
ShipPlan* createShipPlan(int &errorCode, const std::string& shipPlanPath);
ShipRoute* createShipRoute(int &errorCode, const std::string& shipRoutePath);
std::unique_ptr<std::map<std::string, int>>  createMapOfPortAndNumberOfVisits(std::vector<std::string>* portList);
std::unique_ptr<std::map<std::string, std::string>> createMapPortFullNameToCargoPath(const std::string &pathToDir,
        std::map<std::string, int> *mapPortVisits, const std::string &lastPort, std::vector<std::string>* errors);
bool handleNameOfFile (const std::string& fileName, std::string& portName, int & indexNumber);
void addPortsWithFileToMap(const std::string &pathToDir, std::map<std::string, int> *mapPortVisits,
        std::map<std::string, std::string> *mapPortFullNameToCargoPath, std::vector<std::string>* errors);
void addPortsWithNoFileToMap(std::map<std::string, int> *mapPortVisits,
                             std::map<std::string, std::string> *mapPortFullNameToCargoPath,
                             const std::string &lastPort, std::vector<std::string> *errors);
bool validateFarRejected(std::vector<Container*>& left, std::vector<Container*>& was, std::vector<Container*> contByDist);
void getStringOfErrors(int num, std::string& result);
bool containsFatalError(int errorCode);
int getBitInNumber(int num, int bit);
int folderIsExistOrCanBeBuilt(const std::string& path);
bool isFolderExist(const std::string& path);
void createFolder(const std::string& path);
#endif //STOWAGE_MODEL_MAIN_H
