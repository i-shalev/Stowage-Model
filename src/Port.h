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
    bool hasContainers();
    Container* getContainerByID(const string& id);
    void getVectorOfContainers(vector<Container*>& vector) ;
    void getContainersWithDest(const string& dest, vector<Container*>& vec);
    void getContainersByDistance(ShipRoute& sr, vector<Container*>& vec);
};


#endif //STOWAGE_MODEL_PORT_H
