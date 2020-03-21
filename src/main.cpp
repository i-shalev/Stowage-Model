#include <iostream>
#include "Container.h"

int main(){
    Container c1(5, "ISRAE", "CSQU3054383");
    std::cout << c1.checkDestination() << std::endl;
    std::cout << c1.checkId() << std::endl;
    std::cout << c1.getWeight() << std::endl;
    std::cout << c1.getId() << std::endl;
}