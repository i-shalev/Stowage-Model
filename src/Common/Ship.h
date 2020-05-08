//
// Created by zivco on 26/03/2020.
//

#include <map>
#include "ShipRoute.h"
#include "ShipPlan.h"
#include "Port.h"

#ifndef STOWAGE_MODEL_SHIP_H
#define STOWAGE_MODEL_SHIP_H

//using namespace std;
class Ship {
private:
    ShipRoute* route;
    ShipPlan* plan;

public:
    Ship(ShipRoute *sr, ShipPlan *sp);
    ~Ship();
    Ship(const Ship &ship) = delete; //disable copy constr'. the deafult is not good, need deep copy.
    ShipPlan& getPlan();
    bool willVisit(const std::string& dest) const;
    bool isFull() const;
    void getAllContainersWithDest(const std::string &dest, std::vector<Container*> &res);
    std::string getCurrentDestination() const {return route->getHead();}
    void getContainerPosition(const std::string &id, std::vector<int> &res);
    bool finishRoute() const;
    bool lastStop() const;
    ShipRoute& getRoute() const {return *(this->route);}
    int numEmptyPlaces() const;
    bool hasContainer(const std::string ID) const;
};

#endif //STOWAGE_MODEL_SHIP_H
