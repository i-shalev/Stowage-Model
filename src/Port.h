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
    map<std::string, Container*>* ContainersAwaiting;
    vector<string>* errors;

public:
    Port(std::string  symbol, int number, vector<string>* _errors);
    Port(const Port& port)  = delete;
    ~Port();
    const std::string& getSymbol() const;
    void addContainer(Container* container);
    Container* removeContainer(const std::string& id);
    bool hasContainers() const;
    Container* getContainerByID(const string& id) const;
    void getVectorOfContainers(vector<Container*>& vector) const;
    void getContainersWithDest(const string& dest, vector<Container*>& vec) const;
    void getContainersByDistance(ShipRoute& sr, vector<Container*>& vec) const;
};


#endif //STOWAGE_MODEL_PORT_H
