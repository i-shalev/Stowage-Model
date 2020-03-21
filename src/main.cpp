#include <iostream>
#include "Container.h"
#include "ShipRoute.h"
#include "Floor.h"

int main(){
    Container c1(5, "ISRAE", "CSQU3054383");
    std::cout << c1.checkDestination() << std::endl;
    std::cout << c1.checkId() << std::endl;
    std::cout << c1.getWeight() << std::endl;
    std::cout << c1.getId() << std::endl;
    std::cout << " " << std::endl;
    ShipRoute s1;
    s1.addDstAtEnd("ISRAE");
    s1.addDstAtEnd("ISRAA");
    s1.addDstAtEnd("ISAAA");
    s1.printList();
    std::cout << "FloorCheck: " << std::endl;
    Floor f1(0,2,2);
    for(int i=0; i<2; i++){
        for(int j=0; j<2; j++){
            Container c1(10*i + j, "ISRAE", "CSQU3054383");
            f1.set(i,j,&c1);
        }
    }
    for(int i=0; i<2; i++){
        for(int j=0; j<2; j++){
            std::cout << f1.get(i,j)->getWeight() << std::endl;
        }
    }


}