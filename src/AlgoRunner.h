//
// Created by itay on 10/04/2020.
//

#ifndef STOWAGE_MODEL_ALGORUNNER_H
#define STOWAGE_MODEL_ALGORUNNER_H

#include <string>
#include <iostream>
#include "Files.h"
#include "Port.h"
#include "Crane.h"
#include "NaiveAlgo.h"


enum AlgoType  {NaiveAlgoEnum};

using namespace std;
class AlgoRunner {
    AlgoType algoType;
    string pathToRootDir;
    int sumOperations;
    vector<string>* errors;

public:
    AlgoRunner(AlgoType _algoType, const string& _pathToRootDir);
    AlgoRunner(const AlgoRunner& algoRunner)  = delete;
    void startRun();
    int simulateNaive(const string &pathToDir);
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
};

bool validate(Ship* ship);




#endif //STOWAGE_MODEL_ALGORUNNER_H
