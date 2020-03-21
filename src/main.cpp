#include <iostream>
#include "Container.h"
#include "ShipRoute.h"

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
}