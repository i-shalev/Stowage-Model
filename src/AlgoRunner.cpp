//
// Created by itay on 10/04/2020.
//

#include "AlgoRunner.h"

AlgoRunner::AlgoRunner(AlgoType _algoType, string _pathToRootDir) {
    this->pathToRootDir = _pathToRootDir;
    this->algoType = _algoType;
    this->sumOperations = 0;
}

void AlgoRunner::startRun() {
    auto* dirs = getDirsFromRootDir(pathToRootDir);
    string resultFileName = R"(/simulation.results)";
    writeToFile(pathToRootDir +  resultFileName, "NaiveAlgo, ");
    switch(algoType){
        case NaiveAlgoEnum:
            for(const auto& dir:*dirs) {
                std::cout << dir << std::endl;
                int numOp = simulateNaive(dir);
                if(numOp != -1) {
                    this->sumOperations += simulateNaive(dir);
                }
                writeToFile(pathToRootDir +  resultFileName, std::to_string(numOp) + ", ");

            }
            writeToFile(pathToRootDir +  resultFileName, std::to_string(this->sumOperations) + "\n");
            break;
    }

    delete dirs;
}

int AlgoRunner::simulateNaive(const string &pathToDir) {
        auto* ship = createShip(pathToDir);
        if(ship == nullptr) {
            return -1;
        }
        NaiveAlgo alg(ship);
        int portOperations, sumOperations=0;
        while(!ship->finishRoute()){
            std::cout << "enter to port "<<ship->getCurrentDestination() << std::endl;
            alg.getInstructionForCargo(pathToDir +  R"(/instructions.txt)");
            Crane c1(ship);
            portOperations = c1.executeOperationList(pathToDir +  R"(/instructions.txt)");
            if(portOperations<0){
                std::cout << "NaiveAlgo failure, exiting..." << std::endl;
                delete ship;
                return -1;
            }
            sumOperations+=portOperations;

            ship->moveToNextPort();
            std::cout << "Moving to the next destination" << std::endl;
        }
//        string msg = "Naive algo done with total " + std::to_string(sumOperations) + " operations.";
//        writeToFile(pathToDir +  R"(/Results.txt)", msg);
        delete ship;
        return sumOperations;
}

bool handleNameOfFile (const string& fileName, string& portName, int & indexNumber) {
    vector<string> elems;
    stringstream s(fileName);
    string word;
    while(getline(s, word, '_')) {
        elems.push_back(word);
    }
    if(elems.size() != 2) {
        std::cout << "Warning: the file name: " << fileName << " is not legal." << std::endl;
        return false;
    }
    portName = elems[0];
    try {
        indexNumber = stoi(elems[1]);
    } catch (const std::exception& e) {
        std::cout << "Warning: the file name: " << fileName << " is not legal." << std::endl;
        return false;
    }
    return true;
}

map<string, int>* createMapOfPortAndNumberOfVisits(vector<string>* portList) {
    auto* mapPortVisits = new map<string, int>();
    for(const auto& port : *portList) {
        auto res = mapPortVisits->find(port);
        int ans;
        if(res == mapPortVisits->end()) {
            ans = 0;
        } else {
            ans = res->second;
            mapPortVisits->erase(port);
        }
        mapPortVisits->insert({port, ans+1});
    }
    return mapPortVisits;
}

ShipPlan* createShipPlan(const string& pathToShipPlan) {
    int numFloors=0 , length=0, width=0, numLines;
    if(! getSizesShipPlan(pathToShipPlan, numFloors, length, width, numLines)) {
        return nullptr;
    }

    // create the ShipPlanVector
    auto* blocks = new vector<vector<int>>(numLines-1);
    if(! readShipPlan(*blocks, pathToShipPlan)) {
        return nullptr;
    }

    auto* shipPlan = new ShipPlan(numFloors, length, width, *blocks);
    delete blocks;
    return  shipPlan;
}

ShipRoute* createShipRoute(const string &pathToShipPorts) {
    auto* ports = new vector<string>();
    readShipPorts(*ports, pathToShipPorts);
    auto* shipRoute = new ShipRoute(ports);

    delete ports;
    return shipRoute;
}

Ship* createShip(const string &pathToDir){
    auto* shipPlan = createShipPlan(pathToDir + R"(/ShipPort.csv)");
    auto* shipRoute = createShipRoute(pathToDir + R"(/Ports.csv)");
    auto* mapPortVisits = createMapOfPortAndNumberOfVisits(shipRoute->getDstList());

    if(shipPlan == nullptr or shipRoute == nullptr or shipRoute->getRouteLength() == 0){
        return nullptr;
    }

    map<string, Port*>* portNameToPortMap = createPortNameToPortMap(pathToDir, mapPortVisits, shipRoute->getDstList()->at(shipRoute->getDstList()->size()-1));

    auto* ship = new Ship(shipRoute, shipPlan, portNameToPortMap);
    delete mapPortVisits;
    return ship;
}

map<string, Port*>* createPortNameToPortMap(const string &pathToDir, map<string, int>* mapPortVisits, const string& lastPort) {

    auto* mapPortNameToPort = new map<string, Port*>();

    addPortsWithFileToMap(pathToDir, mapPortVisits, mapPortNameToPort);

    addPortsWithNoFileToMap(mapPortVisits, lastPort, mapPortNameToPort);

    return mapPortNameToPort;
}

void addPortsWithNoFileToMap(map<string, int> *mapPortVisits, const string &lastPort, map<string, Port *> *mapPortNameToPort) {

    for(const auto& elem : *mapPortVisits)
    {
        for(int i = 0; i < elem.second; i++)
        {
            string port_index = elem.first + "_" + to_string(i);
            auto res = mapPortNameToPort->find(port_index);
            if(!mapPortNameToPort->empty() && res!=mapPortNameToPort->end()){
                continue;
            }

            if(lastPort == elem.first and i == elem.second - 1){
                // the last port - dont need to do anything for now
            } else {
                std::cout << "Warning: the file " << elem.first + "_" + to_string(i) << ".cargo_data is missing." << std::endl;
            }
            Port *port = new Port(elem.first, i);
            mapPortNameToPort->insert({port_index, port});
        }
    }
}

void addPortsWithFileToMap(const string &pathToDir, map<string, int> *mapPortVisits, map<string, Port*>* mapPortNameToPort) {
    char* pathToDirChar = (char *)(malloc((pathToDir.size() + 1) * sizeof(char)));
    stringToCharStar(pathToDirChar, pathToDir);
    vector<string> namesOfFilesEndsWithCargoData;
    getCargoData(pathToDirChar, namesOfFilesEndsWithCargoData);

    int indexNumber;
    string portName;
    string fullname;


    for (const auto& name: namesOfFilesEndsWithCargoData) {
        if (handleNameOfFile(name, portName, indexNumber)) {
            auto res = mapPortVisits->find(portName);
            if(res !=  mapPortVisits->end() and res->second > indexNumber) {
                Port *port = new Port(portName, indexNumber);
                fullname =  pathToDir + '/';
                fullname += name;
                fullname += R"(.cargo_data)";
                if (readPortContainers(port, fullname)) {
                    mapPortNameToPort->insert({name, port});
                }
            } else {
                std::cout << "Warning: the file " << name << ".cargo_data is not necessary" << std::endl;
            }
        }
    }
    delete pathToDirChar;
}