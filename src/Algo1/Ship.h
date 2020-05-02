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
    std::map<std::string, int>* mapPortNameToNumberOfVisitsUntilNow;

public:
    Ship(ShipRoute *sr, ShipPlan *sp);
    ~Ship();
    Ship(const Ship &ship) = delete; //disable copy constr'. the deafult is not good, need deep copy.
    ShipPlan& getPlan();
    bool willVisit(std::string dest) const;
    bool isFull() const;
//    bool calculateBalanceAfter(const string& opcode, const string& containerId){ return true;}
    void getAllContainersWithDest(const std::string &dest, std::vector<Container*> &res);
    std::string getCurrentDestination() const {return route->getHead();}
    void getContainerPosition(const std::string &id, std::vector<int> &res);
    bool finishRoute() const;
    void moveToNextPort();
    void addOneVisitToMap();
    int getIndexOfPort() const;
    Port* getCurrentPort() const;
    ShipRoute& getRoute() const {return *(this->route);}
    int numEmptyPlaces() const;
};

#endif //STOWAGE_MODEL_SHIP_H
