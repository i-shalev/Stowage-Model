//
// Created by itay on 04/04/2020.
//

#ifndef STOWAGE_MODEL_MAIN_H
#define STOWAGE_MODEL_MAIN_H
#include <iostream>
#include "Files.h"
#include "Port.h"
#include "Crane.h"
#include "Algo.h"
#include "AlgoRunner.h"

using namespace std;
class main {

};

int main();
int simulate(const string &pathToDir);
bool handleNameOfFile (const string& fileName, string& portName, int & indexNumber);
map<string, int>* createMapOfPortAndNumberOfVisits(vector<string>* portList);
void addPortsWithNoFile(map<string, int> *mapPortVisits, vector<string> *portVector, const string &path);
ShipPlan* createShipPlan(const string& pathToShipPlan);
ShipRoute *createShipRoute(const string &pathToShipPorts);
Ship* createShip(const string &pathToDir);
map<string, Port*>* createPortNameToPortMap(const string &pathToDir, map<string, int>* mapPortVisits, const string& lastPort);
void addPortsWithNoFileToMap(map<string, int> *mapPortVisits, const string &lastPort,
                             map<string, Port *> *mapPortNameToPort);
void addPortsWithFileToMap(const string &pathToDir, map<string, int> *mapPortVisits, map<string, Port*>* mapPortNameToPort);

#endif //STOWAGE_MODEL_MAIN_H
