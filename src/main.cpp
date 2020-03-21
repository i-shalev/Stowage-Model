#include <iostream>
#include "Container.h"

int main(){
    Container c1(5, "hew", "hey");
    std::cout << c1.getWeight() << std::endl;
}