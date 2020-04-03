#include <iostream>
#include "tests.h"
#include "Crane.h"
#include "Files.h"

int main(){
//    Container* c1 = new Container(5, "ISRAE", "CSQU3054383"); // place on stack!!! to allocate on heap, see below
//    std::cout << c1->checkDestination() << std::endl;
//    std::cout << c1->checkId() << std::endl;
//    std::cout << c1->getWeight() << std::endl;
//    std::cout << c1->getId() << std::endl;
//    std::cout << " " << std::endl;
//    ShipRoute* s1 = new ShipRoute();
//    s1->addDstAtEnd("ISRAE");
//    s1->addDstAtEnd("ISRAA");
//    s1->addDstAtEnd("ISAAA");
//    s1->addDstAtEnd("ZIVCO");
//    s1->addDstAtEnd("ABCDE");
    int numFloors=0 , length=0, width=0;
    string path = R"(C:\Users\zivco\Downloads\exmaple1.csv)";

    getSizesShipPlan(path, numFloors, length, width);
    vector<vector<vector<int>>>* blocks = new vector<vector<vector<int>>>(numFloors, vector< vector<int>>(length * width, vector<int>(2, {-1})));

    std::cout << numFloors << std::endl;
    std::cout << length << std::endl;
    std::cout << width << std::endl;

    readShipPlan(*blocks, path);
    ShipPlan* shipPlan = new ShipPlan(numFloors, length, width, blocks);
    Container* c2 = new Container(1005, "ISRAE", "CSQU3054383");
    Container* c3 = new Container(2, "ISRAE", "CSQU3054383");
    Container* c4 = new Container(3, "ISRAE", "CSQU3054383");
    ShipRoute* sr = new ShipRoute();
    sr->addDstAtEnd("ABCDE");
    sr->addDstAtEnd("ISRAE");
    Ship* ship = new Ship(sr, shipPlan);
    Crane* crane = new Crane(ship);
    ship->getPlan().printShipPlan();
    std::cout << crane->Load(c2,0,0,1) <<std::endl;
    Container* res;
    std::cout << crane->Unload("CSQU3054383",0,0,1,&res) << std::endl;
    std::cout << res->getWeight() << std::endl;
    std::cout << (ship->getPlan().getFloor(0)->getContainerAtPosition(0,1) == nullptr) << std::endl;
    ship->getPlan().printShipPlan();
    delete blocks;
    delete shipPlan;
    delete c2;
    delete c4;
    delete c3;
//    for(auto & block : *blocks) {
//        for(auto & j : block) {
//            for (int k : j) {
//                std::cout << k << std::endl;
//            }
//        }
//    }

//    Floor* f1 = new Floor(0,2,2);
//    Container* c2 = new Container(1005, "ISRAE", "CSQU3054383");
//    Container* c3 = new Container(2, "ISRAE", "CSQU3054383");
//    Container* c4 = new Container(3, "ISRAE", "CSQU3054383");
//    f1->setContainerAtPosition(0, 0, c1);
//    f1->setContainerAtPosition(0, 1, c2);
//    f1->setContainerAtPosition(1, 0, c3);
//    f1->setContainerAtPosition(1, 1, c4);
//    for(int i=0; i<2; i++) {
//        for (int j = 0; j < 2; j++) {
//            std::cout << f1->getContainerAtPosition(i, j)->getWeight() << std::endl;
//        }
//    }

//    ShipPlan* sp = new ShipPlan(3,2,2);
//    sp->getFloor(0)->setContainerAtPosition(0,0,c1);
//    sp->getFloor(0)->setContainerAtPosition(0,1,c2);
//    Ship* ship = new Ship(s1,sp);
//    printf("%d\n\n",ship->getPlan().getFloor(0)->getContainerAtPosition(0,1)->getWeight());
//    delete c1;
//    delete c2;
//    delete c3;
//    delete c4;
//    delete s1;
//    delete f1;
//    delete sp;
//    runTests();

}