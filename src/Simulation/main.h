//
// Created by itay on 04/04/2020.
//

#ifndef STOWAGE_MODEL_MAIN_H
#define STOWAGE_MODEL_MAIN_H
#include <iostream>
#include "../Common/Files.h"
#include "../Common/Port.h"
#include "../Common/Crane.h"
#include "AlgoRunner.h"
#include "../Common/AbstractAlgorithm.h"

//using namespace std;
class main {

};

int main(int argc, char **argv);
int createArgs(std::map<std::string, std::string>& args, int& argc, char **argv);
void printArgs(std::map<std::string, std::string>& args);
void runAllAlgo(const std::string& algoPath);
void runAlgoForAllTravels(AbstractAlgorithm& algo, const std::string &travelPath, const std::string &outputPath);
void runAlgoForTravel(AbstractAlgorithm& algo, const std::string &pathToDir, const std::string &outputPath);
int runAlgoOnPort(Ship *ship, const std::string& cargoDataPath, const std::string& instructionsPath);
void getShipPlanAndRoutePaths(const std::string& travelPath, std::string& shipPlanPath, std::string& shipRoutePath);
ShipPlan* createShipPlan(int &errorCode, const std::string& shipPlanPath);
ShipRoute* createShipRoute(int &errorCode, const std::string& shipRoutePath);
std::map<std::string, int>* createMapOfPortAndNumberOfVisits(std::vector<std::string>* portList);
std::map<std::string, std::string>* createMapPortFullNameToCargoPath(const std::string &pathToDir,
        std::map<std::string, int> *mapPortVisits, const std::string &lastPort, std::vector<std::string>* errors);
bool handleNameOfFile (const std::string& fileName, std::string& portName, int & indexNumber);
void addPortsWithFileToMap(const std::string &pathToDir, std::map<std::string, int> *mapPortVisits,
        std::map<std::string, std::string> *mapPortFullNameToCargoPath, std::vector<std::string>* errors);
void addPortsWithNoFileToMap(const std::string &pathToDir, std::map<std::string, int> *mapPortVisits,
        std::map<std::string, std::string> *mapPortFullNameToCargoPath,
        const std::string &lastPort, std::vector<std::string>* errors);
bool validateFarRejected(std::vector<Container*>& left, std::vector<Container*>& was, std::vector<Container*> contByDist);
void getStringOfErrors(int num, std::string& result);
#endif //STOWAGE_MODEL_MAIN_H
