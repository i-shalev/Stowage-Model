#include <iostream>
#include "Container.h"
#include "ShipRoute.h"
#include "Floor.h"
#include "tests.h"

int main(){
    Container c1(5, "ISRAE", "CSQU3054383");
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
    Container c2(1, "ISRAE", "CSQU3054383");
    Container c3(2, "ISRAE", "CSQU3054383");
    Container c4(3, "ISRAE", "CSQU3054383");
    f1.set(0,0,&c1);
    f1.set(0,1,&c2);
    f1.set(1,0,&c3);
    f1.set(1,1,&c4);
    for(int i=0; i<2; i++){
        for(int j=0; j<2; j++){
            std::cout << f1.get(i,j)->getWeight() << std::endl;
        }
    }

    runTests();

}