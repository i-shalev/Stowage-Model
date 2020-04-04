//
// Created by zivco on 03/04/2020.
//

#ifndef STOWAGE_MODEL_PORT_H
#define STOWAGE_MODEL_PORT_H

#include <map>
#include "Container.h"
#include "vector"
class Port{
    const std::string symbol;
    int number;
    map<std::string, Container*>* ContainersAwaiting;

public:
    Port(const std::string& symbol, int number): symbol(symbol), number(number){
        ContainersAwaiting = new map<std::string, Container*>;
    }
    const std::string& getSymbol() const { return symbol; }
    void addContainer(Container* container){
        auto res = ContainersAwaiting->find(container->getId());
        if(!ContainersAwaiting->empty() && res!=ContainersAwaiting->end()){
            std::cout << "Warning: the port already have this container";
            return;
        }
        ContainersAwaiting->insert({container->getId(),container});
    }
    Container* removeContainer(const std::string& id){
        auto res = ContainersAwaiting->find(id);
        Container* ans = res->second;
        if(res == ContainersAwaiting->end()){
            std::cout << "Warning: container not exists in the port" << std::endl;
            return nullptr;
        }
        ContainersAwaiting->erase(id);
        return ans;

    }
    bool hasContainers(){return !ContainersAwaiting->empty();}
};


#endif //STOWAGE_MODEL_PORT_H
