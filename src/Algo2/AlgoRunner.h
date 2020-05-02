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
#include "NaiveAlgoWithTrick.h"


enum AlgoType  {NaiveAlgoEnum, NaiveAlgoWithTrickEnum};

// This Class is the actual simulation it's get an Algorithm Object and run the simulation.
//using namespace std;
class AlgoRunner {
    AlgoType algoType;
    std::string pathToRootDir;
    int sumOperations;
    std::vector<std::string>* errors;

public:
    AlgoRunner(AlgoType _algoType, const std::string& _pathToRootDir);
    AlgoRunner(const AlgoRunner& algoRunner)  = delete;

    //Start the run of the simulation
    void startRun();

    //The simulation for the naive algorithm.
    int simulateNaive(const std::string &pathToDir);

    //The simulation for the little bit smarter algorithm.
    int simulateNaiveWithTrick(const std::string &pathToDir);

    // Get a name of a file if it's in the form "port_index" it's put the values to the args and return true.
    bool handleNameOfFile (const std::string& fileName, std::string& portName, int & indexNumber);

    std::map<std::string, int>* createMapOfPortAndNumberOfVisits(std::vector<std::string>* portList);

    ShipPlan* createShipPlan(const std::string& pathToShipPlan);

    ShipRoute *createShipRoute(const std::string &pathToShipPorts);

    Ship* createShip(const std::string &pathToDir);

    std::map<std::string, Port*>* createPortNameToPortMap(const std::string &pathToDir, std::map<std::string, int>* mapPortVisits, const std::string& lastPort);

    void addPortsWithNoFileToMap(std::map<std::string, int> *mapPortVisits, const std::string &lastPort,
                                 std::map<std::string, Port *> *mapPortNameToPort);
    void addPortsWithFileToMap(const std::string &pathToDir, std::map<std::string, int> *mapPortVisits, std::map<std::string, Port*>* mapPortNameToPort);
};

bool validate(Ship* ship);




#endif //STOWAGE_MODEL_ALGORUNNER_H
