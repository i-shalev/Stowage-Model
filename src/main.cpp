#include <iostream>
#include "Container.h"
#include "ShipRoute.h"
#include "Floor.h"
#include "tests.h"

int main(){
    Container c1(5, "ISRAE", "CSQU3054383"); // place on stack!!! to allocate on heap, see below
    std::cout << c1.checkDestination() << std::endl;
    std::cout << c1.checkId() << std::endl;
    std::cout << c1.getWeight() << std::endl;
    std::cout << c1.getId() << std::endl;
    std::cout << " " << std::endl;
    ShipRoute s1;
    s1.addDstAtEnd("ISRAE");
    s1.deleteFirst();
    s1.deleteFirst();
    s1.addDstAtEnd("ISRAA");
    s1.addDstAtEnd("ISAAA");
    s1.printList();
    std::cout << "FloorCheck: " << std::endl;
    Floor f1(0,2,2);
    Container* c2 = new Container(1, "ISRAE", "CSQU3054383");
    Container* c3 = new Container(2, "ISRAE", "CSQU3054383");
    Container* c4 = new Container(3, "ISRAE", "CSQU3054383");
    f1.setContainerAtPosition(0, 0, &c1);
    f1.setContainerAtPosition(0, 1, c2);
    f1.setContainerAtPosition(1, 0, c3);
    f1.setContainerAtPosition(1, 1, c4);
    for(int i=0; i<2; i++) {
        for (int j = 0; j < 2; j++) {
            std::cout << f1.getContainerAtPosition(i, j)->getWeight() << std::endl;
        }
    }
    delete c2;
    delete c3;
    delete c4;
//    runTests();

}