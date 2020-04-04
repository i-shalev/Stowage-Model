//
// Created by zivco on 03/04/2020.
//

#ifndef STOWAGE_MODEL_PORT_H
#define STOWAGE_MODEL_PORT_H

#include <queue>
#include "Container.h"
#include "vector"
class Port{
    const std::string symbol;
    int number;
    queue<Container*> ContainersAwaiting;

public:
    Port(const std::string& symbol, int number): symbol(symbol), number(number){}
    const std::string& getSymbol() const { return symbol; }
    void addContainer(Container* container){ ContainersAwaiting.push(container);}
    Container* removeContainer(){
        if(!ContainersAwaiting.empty()) {
            Container *res = ContainersAwaiting.front();
            ContainersAwaiting.pop();
            return res;
        }
        return nullptr;
    }
    bool hasContainers(){return !ContainersAwaiting.empty();}
};


#endif //STOWAGE_MODEL_PORT_H
