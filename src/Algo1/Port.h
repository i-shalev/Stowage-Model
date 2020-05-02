//
// Created by zivco on 03/04/2020.
//

#ifndef STOWAGE_MODEL_PORT_H
#define STOWAGE_MODEL_PORT_H

#include <map>
#include <set>
#include <utility>
#include "Container.h"
#include "ShipRoute.h"
#include "vector"
class Port{
    const std::string symbol;
    int number;
    std::map<std::string, Container*>* ContainersAwaiting;

public:
    Port();
    Port(const Port& port)  = delete;
    ~Port();
    const std::string& getSymbol() const;
    void addContainer(Container* container);
    Container* removeContainer(const std::string& id);
    bool hasContainers() const;
    Container* getContainerByID(const std::string& id) const;
    void getVectorOfContainers(std::vector<Container*>& vector) const;
    void getContainersWithDest(const std::string& dest, std::vector<Container*>& vec) const;
    void getContainersByDistance(ShipRoute& sr, std::vector<Container*>& vec) const;
};


#endif //STOWAGE_MODEL_PORT_H
