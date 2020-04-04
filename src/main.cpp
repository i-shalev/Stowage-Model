#include <iostream>
#include "tests.h"
#include "Ship.h"
#include "Files.h"

int main(){
//    vector<vector<int>> vec = {{1,0,4},{1,1,2}};
//    ShipPlan* sp = new ShipPlan(5,2,2,vec);
//    sp -> printShipPlan();

    int numFloors=0 , length=0, width=0, numLines;
    string pathToShipPlan   = R"(C:\Users\itay\Desktop\ShipPort.csv)";
    string pathToShipPorts  = R"(C:\Users\itay\Desktop\Ports.csv)";

    // read the shipPlan file and get the sizes of the shipPlan
    getSizesShipPlan(pathToShipPlan, numFloors, length, width, numLines);

    // create the ShipPlanVector
    auto* blocks = new vector<vector<int>>(numLines-1);
    readShipPlan(*blocks, pathToShipPlan);

    getNumberOfNonEmtpyLines(pathToShipPorts, numLines);
    auto* ports = new vector<string>();
    readShipPorts(*ports, pathToShipPorts);

    ShipRoute* shipRoute = new ShipRoute(*ports);
    ShipPlan* shipPlan = new ShipPlan(numFloors, length, width, *blocks);

    for(int i=0; i<shipRoute->getRouteLength(); i++) {

    }

    delete blocks;
    delete ports;

    shipPlan->printShipPlan();
    shipRoute->printList();


    delete shipRoute;
    delete shipPlan;

/*
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