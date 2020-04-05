//
// Created by zivco on 26/03/2020.
//

#include <map>
#include "ShipRoute.h"
#include "ShipPlan.h"
#include "Port.h"

#ifndef STOWAGE_MODEL_SHIP_H
#define STOWAGE_MODEL_SHIP_H

#endif //STOWAGE_MODEL_SHIP_H

using namespace std;
class Ship {
private:
    ShipRoute* route;
    ShipPlan* plan;
    map<string, Port> *mapPortNameToPort;

public:
    Ship(ShipRoute *sr, ShipPlan *sp, map<string, Port> *map);
    Ship(const Ship &ship) = delete; //disable copy constr'. the deafult is not good, need deep copy.
    //const ShipRoute& getRoute();
    ShipPlan& getPlan();//TODO: make sure we really cant change the route from outside
    bool willVisit(string dest);
    bool calculateBalanceAfter(const string& opcode, const string& containerId){ return true;}
    void getAllContainersWithDest(const string &dest, vector<Container*> &res);
    const string getCurrentDestination(){return route->getHead();}
    void getContainerPosition(const string &id, vector<int> &res);

};