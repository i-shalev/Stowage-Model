//
// Created by itay on 10/04/2020.
//

#include "AlgoRunner.h"

AlgoRunner::AlgoRunner(AlgoType _algoType, const std::string& _pathToRootDir) {
    this->pathToRootDir = _pathToRootDir;
    this->algoType = _algoType;
    this->sumOperations = 0;
    this->errors = new std::vector<std::string>();
}

void AlgoRunner::startRun() {
    auto* dirs = getDirsFromRootDir(pathToRootDir);
    std::string resultFileName = R"(/simulation.results)";
    std::string errorFileName = R"(/simulation.errors)";
    switch(algoType){
        case NaiveAlgoEnum:
//            std::cout << "NaiveAlgo" << std::endl;
            writeToFile(pathToRootDir +  resultFileName, "NaiveAlgo, ");
            writeToFile(pathToRootDir +  errorFileName, "NaiveAlgo, ");
            for(const auto& dir:*dirs) {
                errors->clear();
//                std::cout << dir << std::endl;
                int numOp = simulateNaive(dir);
                if(numOp != -1) {
                    this->sumOperations += numOp;
                }
                writeToFile(pathToRootDir +  resultFileName, std::to_string(numOp) + ", ");
//                std::cout << "ERRORS:" << std::endl;
                if(!errors->empty()) {
                    std::string errorsString = errors->at(0);
                    for(size_t i = 1; i < errors->size(); i++) {
                        errorsString += " | ";
                        errorsString += errors->at(i);
                    }
                    writeToFile(pathToRootDir +  errorFileName, errorsString );
                }
//                for(const auto& error:*errors) {
//                    std::cout << error << std::endl;
//                }
                writeToFile(pathToRootDir +  errorFileName, "," );
            }
            break;
        case NaiveAlgoWithTrickEnum:
//            std::cout << "NaiveAlgoWithTrick" << std::endl;
            writeToFile(pathToRootDir +  resultFileName, "NaiveAlgoWithTrick, ");
            writeToFile(pathToRootDir +  errorFileName, "NaiveAlgoWithTrick, ");
            for(const auto& dir:*dirs) {
                errors->clear();
//                std::cout << dir << std::endl;
                int numOp = simulateNaiveWithTrick(dir);
                if(numOp != -1) {
                    this->sumOperations += numOp;
                }
                writeToFile(pathToRootDir +  resultFileName, std::to_string(numOp) + ", ");
//                std::cout << "ERRORS:" << std::endl;
                if(!errors->empty()) {
                    std::string errorsString = errors->at(0);
                    for(size_t i = 1; i < errors->size(); i++) {
                        errorsString += " | ";
                        errorsString += errors->at(i);
                    }
                    writeToFile(pathToRootDir +  errorFileName, errorsString );
                }
//                for(const auto& error:*errors) {
//                    std::cout << error << std::endl;
//                }
                writeToFile(pathToRootDir +  errorFileName, "," );
            }
            break;

    }

    writeToFile(pathToRootDir +  resultFileName, std::to_string(this->sumOperations) + "\n");
    writeToFile(pathToRootDir +  errorFileName, "\n" );
    delete errors;
    delete dirs;
}

int AlgoRunner::simulateNaive(const std::string &pathToDir) {
        auto* ship = createShip(pathToDir);
        if(ship == nullptr) {
            return -1;
        }
        NaiveAlgo alg(ship);
        int portOperations, sumOp=0;
        while(!ship->finishRoute()){
//            std::cout << "enter to port "<<ship->getCurrentDestination() << std::endl;
            alg.getInstructionsForCargo("", pathToDir + R"(/instructions.txt)");
            Crane c1(ship, errors);
            portOperations = c1.executeOperationList(pathToDir +  R"(/instructions.txt)");
            if(portOperations<0){
                errors->push_back("Error: Algo return negative number of moves (Algo failed)");
//                std::cout << "NaiveAlgo failure, exiting..." << std::endl;
                delete ship;
                return -1;
            }
            sumOp+=portOperations;
            if(!validate(ship)){
                errors->push_back("Error: Algo reject containers with no reason");
//                std::cout << "Error: Algo reject containers with no reason" << std::endl;
            }
            c1.disconnect();
            ship->moveToNextPort();
//            std::cout << "Moving to the next destination" << std::endl;
        }
        delete ship;
        return sumOp;
}

int AlgoRunner::simulateNaiveWithTrick(const std::string &pathToDir) {
    auto* ship = createShip(pathToDir);
    if(ship == nullptr) {
        return -1;
    }
    NaiveAlgoWithTrick alg(ship);
    int portOperations, sumOp=0;
    while(!ship->finishRoute()){
//            std::cout << "enter to port "<<ship->getCurrentDestination() << std::endl;
        alg.getInstructionsForCargo("",pathToDir +  R"(/instructions.txt)");
        Crane c1(ship, errors);
        portOperations = c1.executeOperationList(pathToDir +  R"(/instructions.txt)");
        if(portOperations<0){
            errors->push_back("Error: Algo return negative number of moves (Algo failed)");
//                std::cout << "NaiveAlgoWithTrick failure, exiting..." << std::endl;
            delete ship;
            return -1;
        }
        sumOp+=portOperations;
        if(!validate(ship)){
            errors->push_back("Error: Algo reject containers with no reason");
//                std::cout << "Error: Algo reject containers with no reason" << std::endl;
        }
        c1.disconnect();
        ship->moveToNextPort();
//            std::cout << "Moving to the next destination" << std::endl;
    }
    delete ship;
    return sumOp;
}

bool AlgoRunner::handleNameOfFile (const std::string& fileName, std::string& portName, int & indexNumber) {
    std::vector<std::string> elems;
    std::stringstream s(fileName);
    std::string word;
    while(getline(s, word, '_')) {
        elems.push_back(word);
    }
    if(elems.size() != 2) {
        errors->push_back("Warning: the file name: " + fileName + " is not legal.");
//        std::cout << "Warning: the file name: " << fileName << " is not legal." << std::endl;
        return false;
    }
    portName = elems[0];
    try {
        indexNumber = stoi(elems[1]);
    } catch (const std::exception& e) {
        errors->push_back("Warning: the file name: " + fileName + " is not legal.");
//        std::cout << "Warning: the file name: " << fileName << " is not legal." << std::endl;
        return false;
    }
    return true;
}

std::map<std::string, int>* AlgoRunner::createMapOfPortAndNumberOfVisits(std::vector<std::string>* portList) {
    auto* mapPortVisits = new std::map<std::string, int>();
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

ShipPlan* AlgoRunner::createShipPlan(const std::string& pathToShipPlan) {
    int numFloors=0 , length=0, width=0, numLines;
    if(! getSizesShipPlan(pathToShipPlan, numFloors, length, width, numLines, this->errors)) {
        return nullptr;
    }

    // create the ShipPlanVector
    auto* blocks = new std::vector<std::vector<int>>(numLines-1);
    if(!readShipPlan(*blocks, pathToShipPlan, errors)) {
        return nullptr;
    }

    auto* shipPlan = new ShipPlan(numFloors, length, width, *blocks, errors);
    delete blocks;
    return  shipPlan;
}

ShipRoute* AlgoRunner::createShipRoute(const std::string &pathToShipPorts) {
    auto* ports = new std::vector<std::string>();
    readShipPorts(*ports, pathToShipPorts, errors);
    auto* shipRoute = new ShipRoute(ports, errors);

    delete ports;
    return shipRoute;
}

Ship* AlgoRunner::createShip(const std::string &pathToDir){
    auto* shipPlan = createShipPlan(pathToDir + R"(/ShipPort.csv)");
    auto* shipRoute = createShipRoute(pathToDir + R"(/Ports.csv)");
    auto* mapPortVisits = createMapOfPortAndNumberOfVisits(shipRoute->getDstList());

    if(shipPlan == nullptr or shipRoute == nullptr or shipRoute->getRouteLength() == 0){
        return nullptr;
    }

    std::map<std::string, Port*>* portNameToPortMap = createPortNameToPortMap(pathToDir, mapPortVisits, shipRoute->getDstList()->at(shipRoute->getDstList()->size()-1));

    auto* ship = new Ship(shipRoute, shipPlan, portNameToPortMap, errors);
    delete mapPortVisits;
    return ship;
}

std::map<std::string, Port*>* AlgoRunner::createPortNameToPortMap(const std::string &pathToDir, std::map<std::string, int>* mapPortVisits, const std::string& lastPort) {

    auto* mapPortNameToPort = new std::map<std::string, Port*>();

    addPortsWithFileToMap(pathToDir, mapPortVisits, mapPortNameToPort);

    addPortsWithNoFileToMap(mapPortVisits, lastPort, mapPortNameToPort);

    return mapPortNameToPort;
}

void AlgoRunner::addPortsWithNoFileToMap(std::map<std::string, int> *mapPortVisits, const std::string &lastPort, std::map<std::string, Port *> *mapPortNameToPort) {

    for(const auto& elem : *mapPortVisits)
    {
        for(int i = 0; i < elem.second; i++)
        {
            std::string port_index = elem.first + "_" + std::to_string(i);
            auto res = mapPortNameToPort->find(port_index);
            if(!mapPortNameToPort->empty() && res!=mapPortNameToPort->end()){
                continue;
            }

            if(lastPort == elem.first and i == elem.second - 1){
                // the last port - dont need to do anything for now
            } else {
                errors->push_back("Warning: the file " + elem.first + "_" + std::to_string(i) + ".cargo_data is missing.");
//                std::cout << "Warning: the file " << elem.first + "_" + to_string(i) << ".cargo_data is missing." << std::endl;
            }
            Port *port = new Port(elem.first, i, errors);
            mapPortNameToPort->insert({port_index, port});
        }
    }
}

void AlgoRunner::addPortsWithFileToMap(const std::string &pathToDir, std::map<std::string, int> *mapPortVisits, std::map<std::string, Port*>* mapPortNameToPort) {
    char* pathToDirChar = (char *)(malloc((pathToDir.size() + 1) * sizeof(char)));
    stringToCharStar(pathToDirChar, pathToDir);
    std::vector<std::string> namesOfFilesEndsWithCargoData;
    getCargoData(pathToDirChar, namesOfFilesEndsWithCargoData);

    int indexNumber;
    std::string portName;
    std::string fullname;


    for (const auto& name: namesOfFilesEndsWithCargoData) {
        if (handleNameOfFile(name, portName, indexNumber)) {
            auto res = mapPortVisits->find(portName);
            if(res !=  mapPortVisits->end() and res->second > indexNumber) {
                Port *port = new Port(portName, indexNumber, errors);
                fullname =  pathToDir + '/';
                fullname += name;
                fullname += R"(.cargo_data)";
                if (readPortContainers(port, fullname, errors)) {
                    mapPortNameToPort->insert({name, port});
                }
            } else {
                errors->push_back("Warning: the file " + name + ".cargo_data is not necessary");
//                std::cout << "Warning: the file " << name << ".cargo_data is not necessary" << std::endl;
            }
        }
    }
    delete pathToDirChar;
}

bool validate(Ship* ship){
    if(ship->isFull())
        return true;
    std::vector<Container*> vec;
    ship->getCurrentPort()->getVectorOfContainers(vec);
    for(auto & i : vec){
        if(i->checkId() && ship->willVisit(i->getDest()))
            return false;
    }
    return true;
}