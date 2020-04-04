#include <iostream>
#include "tests.h"
#include "Ship.h"
#include "Files.h"
#include "Port.h"

int main(){
    Port p1("AAAAA",0);
    std::cout << p1.getSymbol() << std::endl;
    int numFloors=0 , length=0, width=0, numLines;
    string pathToShipPlan       = R"(C:\Users\zivco\Downloads\ShipPort.csv)";
    string pathToShipPorts      = R"(C:\Users\zivco\Downloads\Ports.csv)";
    string pathToPortContainers = R"(C:\Users\zivco\Downloads\PortContainers.csv)";

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

//    Port p1("ABCD",5);
//    Container* c2 = new Container(1005, "ISRAE", "CSQU3054383");
//    Container* c3 = new Container(2, "ISRAE", "ggg");
//    Container* c4 = new Container(3, "ISRAE", "CSQU3054383");
//    p1.addContainer(c2);
//    p1.addContainer(c3);
//    for(auto item : p1.getContainersMap()){
//        std::cout << item.second->getWeight() << std::endl;
//    }
//    delete c2;
//    delete c3;
/*
    int numFloors=0 , length=0, width=0, numLines;
    string pathToShipPlan       = R"(C:\Users\itay\Desktop\ShipPort.csv)";
    string pathToShipPorts      = R"(C:\Users\itay\Desktop\Ports.csv)";
    string pathToPortContainers = R"(C:\Users\itay\Desktop\PortContainers.csv)";

    // read the shipPlan file and get the sizes of the shipPlan
    getSizesShipPlan(pathToShipPlan, numFloors, length, width, numLines);

    // create the ShipPlanVector
    auto* blocks = new vector<vector<int>>(numLines-1);
    readShipPlan(*blocks, pathToShipPlan);

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


    std::cout << ports->size() << std::endl;
    for(const auto & port : *ports) {
        std::cout << port << std::endl;
    }

    for(auto & block : *blocks) {
        std::cout << "(" << block.at(0) << ", ";
        std::cout << block.at(1) << ", ";
        std::cout << block.at(2) << ")" << std::endl;
    }

    Floor* f1 = new Floor(0,2,2);
    Container* c2 = new Container(1005, "ISRAE", "CSQU3054383");
    Container* c3 = new Container(2, "ISRAE", "CSQU3054383");
    Container* c4 = new Container(3, "ISRAE", "CSQU3054383");
    f1->setContainerAtPosition(0, 0, c1);
    f1->setContainerAtPosition(0, 1, c2);
    f1->setContainerAtPosition(1, 0, c3);
    f1->setContainerAtPosition(1, 1, c4);
    for(int i=0; i<2; i++) {
        for (int j = 0; j < 2; j++) {
            std::cout << f1->getContainerAtPosition(i, j)->getWeight() << std::endl;
        }
    }

    ShipPlan* sp = new ShipPlan(3,2,2);
    sp->getFloor(0)->setContainerAtPosition(0,0,c1);
    sp->getFloor(0)->setContainerAtPosition(0,1,c2);
    Ship* ship = new Ship(s1,sp);
    printf("%d\n\n",ship->getPlan().getFloor(0)->getContainerAtPosition(0,1)->getWeight());
    delete c1;
    delete c2;
    delete c3;
    delete c4;
    delete s1;
    delete f1;
    delete sp;
    runTests();
*/
}