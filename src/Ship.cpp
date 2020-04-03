//
// Created by zivco on 26/03/2020.
//

#include "Ship.h"

Ship::Ship(ShipRoute* sr, ShipPlan* sp): route(sr), plan(sp){
    if(!this->plan->isValid()){
        std::cout << "invalid plan!" << std::endl;
    }

};
ShipPlan& Ship::getPlan() { return *(this->plan);}
//const ShipRoute& Ship::getRoute() {return *(this->route);}
bool Ship::willVisit(string dest) {
    return this->route->willVisit(dest);
}