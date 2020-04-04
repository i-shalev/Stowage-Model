#include <iostream>
#include <bits/stdc++.h>
#include "Ship.h"
#include "Files.h"
#include "Port.h"
#include "main.h"

int main(){
    simulate( R"(C:\Users\itay\Desktop\ex)");
}

int simulate(const string &pathToDir) {
    int numFloors=0 , length=0, width=0, numLines;
    string pathToShipPlan       = pathToDir + R"(\ShipPort.csv)";
    string pathToShipPorts      = pathToDir + R"(\Ports.csv)";
    string pathToPortContainers = pathToDir + R"(\PortContainers.csv)";
    char pathToDirChar[pathToDir.size()+1];
    stringToCharStar(pathToDirChar, pathToDir);
//    std::cout << pathToDirChar << std::endl;

    // read the shipPlan file and get the sizes of the shipPlan
    if(! getSizesShipPlan(pathToShipPlan, numFloors, length, width, numLines)) {
        return -1;
    }

    // create the ShipPlanVector
    auto* blocks = new vector<vector<int>>(numLines-1);
    if(! readShipPlan(*blocks, pathToShipPlan)) {
        return -1;
    }

    auto* shipPlan = new ShipPlan(numFloors, length, width, *blocks);
    delete blocks;

    // read the shipPorts file
//    getNumberOfNonEmtpyLines(pathToShipPorts, numLines);
    auto* ports = new vector<string>();
    readShipPorts(*ports, pathToShipPorts);

    auto* shipRoute = new ShipRoute(*ports);
    auto* mapPortVisits = new map<string, int>();
    createMapOfPortAndNumberOfVisits(ports, mapPortVisits);

    delete ports;


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


//  debugging prints
//    shipPlan->printShipPlan();
//    shipRoute->printList();


    delete shipRoute;
    delete shipPlan;
    delete portsVector;


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

void createMapOfPortAndNumberOfVisits(vector<string>* portList, map<string, int> *mapPortVisits) {
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
}

void findMissingPortFiles(map<string, int> *mapPortVisits, vector<Port*> portVector) {
    int sum = 0;
    for(const auto& elem : *mapPortVisits)
    {
        sum += elem.second;
    }

}