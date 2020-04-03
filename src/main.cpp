#include <iostream>
#include "tests.h"
#include "Ship.h"
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
   /* int numFloors=0 , length=0, width=0;
    string path = R"(C:\Users\itay\Desktop\exmaple1.csv)";
    vector<vector<vector<int>>> blocks = readShipPlan(path, numFloors, length, width);
    for(auto & block : blocks) {
        for(auto & j : block) {
            for (int k : j) {
                std::cout << k << std::endl;
            }
        }
    }*/
   vector<vector<int>> vec;
   Floor* f1 = new Floor(0,2,2, vec);
   Container* c2 = new Container(1005, "ISRAE", "CSQU3054383");
   Container* c3 = new Container(2, "ISRAE", "CSQU3054383");
   Container* c4 = new Container(3, "ISRAE", "CSQU3054383");
   f1->setContainerAtPosition(0,0,c2);
   f1->setContainerAtPosition(1,0,c3);
   f1->setContainerAtPosition(0,1,c4);
   std::cout << f1->getContainerAtPosition(0,0)->getWeight() << std::endl;
   std::cout << f1->getContainerAtPosition(1,0)->getWeight() << std::endl;
   std::cout << f1->getContainerAtPosition(0,1)->getWeight() << std::endl;
    delete f1;
    delete c2;
    delete c3;
    delete c4;

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