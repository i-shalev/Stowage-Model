#include <iostream>
#include "tests.h"
#include "Ship.h"
#include "Files.h"
#include "Port.h"
#include "main.h"

int main(){
//    Port p1("AAAAA",0);
//    std::cout << p1.getSymbol() << std::endl;
    simulate( R"(C:\Users\itay\Desktop\)");
}

int simulate(const string &pathToDir) {
    int numFloors=0 , length=0, width=0, numLines;
    string pathToShipPlan       = pathToDir + R"(ShipPort.csv)";
    string pathToShipPorts      = pathToDir + R"(Ports.csv)";
    string pathToPortContainers = pathToDir + R"(PortContainers.csv)";

    // read the shipPlan file and get the sizes of the shipPlan
    if(! getSizesShipPlan(pathToShipPlan, numFloors, length, width, numLines)) {
        exit(-1);
    }


    // create the ShipPlanVector
    auto* blocks = new vector<vector<int>>(numLines-1);
    if(! readShipPlan(*blocks, pathToShipPlan)) {
        exit(-1);
    }

    auto* shipPlan = new ShipPlan(numFloors, length, width, *blocks);
    delete blocks;

    // read the shipPorts file
//    getNumberOfNonEmtpyLines(pathToShipPorts, numLines);
    auto* ports = new vector<string>();
    readShipPorts(*ports, pathToShipPorts);

    auto* shipRoute = new ShipRoute(*ports);
    delete ports;

    // read the portContainers file
//    getNumberOfNonEmtpyLines(pathToPortContainers, numLines);
    auto* portContainers = new vector<Container*>();
    readPortContainers(*portContainers, pathToPortContainers);

    // debugging prints
    shipPlan->printShipPlan();
    shipRoute->printList();

    for(const auto &container: *portContainers) {
        (*container).printContainer();
    }

    delete shipRoute;
    delete shipPlan;

    return 0;
}