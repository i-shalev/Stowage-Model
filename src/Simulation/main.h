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
void getShipPlanAndRoutePaths(const std::string& travelPath, std::string& shipPlanPath, std::string& shipRoutePath);
ShipPlan* createShipPlan(int &errorCode, const std::string& shipPlanPath);
ShipRoute* createShipRoute(int &errorCode, const std::string& shipRoutePath);
std::map<std::string, int>* createMapOfPortAndNumberOfVisits(std::vector<std::string>* portList);
bool handleNameOfFile (const std::string& fileName, std::string& portName, int & indexNumber);
void addPortsWithFileToMap(const std::string &pathToDir, std::map<std::string,
        int> *mapPortVisits, std::map<std::string, std::string>* mapPortFullNameToCargoPath);
#endif //STOWAGE_MODEL_MAIN_H
