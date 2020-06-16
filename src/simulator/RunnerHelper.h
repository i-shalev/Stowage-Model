//
// Created by itay on 16/06/2020.
//

#ifndef STOWAGE_MODEL_RUNNERHELPER_H
#define STOWAGE_MODEL_RUNNERHELPER_H

#include <iostream>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory>
#include "../common/ShipPlan.h"
#include "../common/ShipRoute.h"
#include "../common/Files.h"

#define PATH_TO_EMPTY_FILE R"(.\empty.empty_file)"

class RunnerHelper {};

int getShipPlanAndRoutePaths(const std::string& pathToDir, std::string& shipPlanPath, std::string& shipRoutePath);
ShipPlan* createShipPlan(int &errorCode, const std::string& shipPlanPath);
ShipRoute* createShipRoute(int &errorCode, const std::string& shipRoutePath);
std::unique_ptr<std::map<std::string, int>> createMapOfPortAndNumberOfVisits(std::vector<std::string>* portList);
std::unique_ptr<std::map<std::string, std::string>> createMapPortFullNameToCargoPath(const std::string &pathToDir,
        std::map<std::string, int> *mapPortVisits, const std::string &lastPort, std::vector<std::string>* errors);
void addPortsWithFileToMap(const std::string &pathToDir, std::map<std::string, int> *mapPortVisits,
        std::map<std::string, std::string> *mapPortFullNameToCargoPath, std::vector<std::string>* errors);
void addPortsWithNoFileToMap(std::map<std::string, int> *mapPortVisits,
        std::map<std::string, std::string> *mapPortFullNameToCargoPath, const std::string &lastPort,
        std::vector<std::string> *errors);
bool handleNameOfFile (const std::string& fileName, std::string& portName, int & indexNumber);
bool findInVec(std::vector<Container*>& vec, const std::string& id);
bool findInVec(std::vector<std::string>& vec, const std::string& id);
int getBitInNumber(int num, int bit);
void getStringOfErrors(int num, std::string& result);
bool containsFatalError(int errorCode);
int turnToTrueBit(int num, int bit);
int folderIsExistOrCanBeBuilt(const std::string& path);
bool isFolderExist(const std::string& path);
void createFolder(const std::string& path);

#endif //STOWAGE_MODEL_RUNNERHELPER_H
