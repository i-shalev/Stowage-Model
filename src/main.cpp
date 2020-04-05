#include <iostream>
#include "Ship.h"
#include "Files.h"
#include "Port.h"
#include "main.h"

int main(){
    simulate( R"(C:\Users\itay\Desktop\ex)");
}

int simulate(const string &pathToDir) {

    createShip(pathToDir);

    return 0;
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

void findMissingPortFiles(map<string, int> *mapPortVisits, vector<string> *portVector, const string &path) {
    for(auto elem : *mapPortVisits)
    {
        for(int i = 0; i < elem.second; i++)
        {
            string fullPath = path + "\\" + elem.first + "_" + to_string(i) + ".cargo_data";
            char pathChar[fullPath.size()+1];
            stringToCharStar(pathChar, fullPath);
            if(!isFile(pathChar)){
                if(portVector->at(portVector->size()-1) == elem.first and i == elem.second-1){
                    // the last port - dont need to do anything for now
                } else {
                    std::cout << "Warning: the file " << elem.first + "_" + to_string(i) << ".cargo_data is missing." << std::endl;
                }
            }
        }
    }
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

map<string, Port>* createPortNameToPortMap(){
    auto* mapPortNameToPort = new map<string, Port>();

    return mapPortNameToPort;
}


Ship* createShip(const string &pathToDir){
    char pathToDirChar[pathToDir.size()+1];
    stringToCharStar(pathToDirChar, pathToDir);

    auto* shipPlan = createShipPlan(pathToDir + R"(\ShipPort.csv)");
    auto* shipRoute = createShipRoute(pathToDir + R"(\Ports.csv)");
    auto* mapPortVisits = createMapOfPortAndNumberOfVisits(shipRoute->getDstList());

    vector<string> namesOfFilesEndsWithCargoData;
    getCargoData(pathToDirChar, namesOfFilesEndsWithCargoData);

    auto* portsVector = new vector<Port*>();
    int indexNumber;
    string portName;
    for (const auto& name: namesOfFilesEndsWithCargoData) {
        if (handleNameOfFile(name, portName, indexNumber)) {
            auto res = mapPortVisits->find(portName);
            if(res !=  mapPortVisits->end() and res->second > indexNumber) {
                Port *port = new Port(portName, indexNumber);
                if (readPortContainers(port, pathToDir + '\\' + name + R"(.cargo_data)")) {
                    portsVector->push_back(port);
//                    std::cout << "port name: " << portName << " index: " << indexNumber << std::endl;
                }
            } else {
                std::cout << "Warning: the file " << name << ".cargo_data is not necessary" << std::endl;
            }
        }
    }
    findMissingPortFiles(mapPortVisits, shipRoute->getDstList(), pathToDir);

//    std::cout <<  shipRoute->getHead() << std::endl;

    delete mapPortVisits;
    delete shipRoute;
    delete shipPlan;
    delete portsVector;

    return nullptr;
}