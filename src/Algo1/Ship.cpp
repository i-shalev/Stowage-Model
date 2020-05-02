//
// Created by zivco on 26/03/2020.
//

#include "Ship.h"

#include <utility>

Ship::Ship(ShipRoute *sr, ShipPlan *sp): route(sr), plan(sp){
    if(!this->plan->isValid()){
//        errors->push_back("Warning: invalid plan");
//        std::cout << "invalid plan!" << std::endl;
    }
}

Ship::~Ship() {
    delete this->plan;
    delete this->route;
}

ShipPlan& Ship::getPlan() { return *(this->plan);}
//const ShipRoute& Ship::getRoute() {return *(this->route);}
bool Ship::willVisit(const std::string& dest) const {
    return this->route->willVisit(dest);
}

void Ship::getContainerPosition(const std::string& id, std::vector<int>& res){
    if(!res.empty()){
//        errors->push_back("Warning: incorrect use of getContainerPosition");
//        std::cout << "Warning: incorrect use of getContainerPosition" << std::endl;
    }
    for(int level=0; level<this->plan->getNumFloors(); level++){
        for(int i=0; i<this->plan->getLength(); i++){
            for(int j=0; j<this->plan->getWidth(); j++ ){
                if(this->plan->getFloor(level)->getContainerAtPosition(i,j)->getId()==id){
                    res.push_back(level);
                    res.push_back(i);
                    res.push_back(j);
                    return;
                }
            }
        }
    }
    res.push_back(-1);
    res.push_back(-1);
    res.push_back(-1);

}

void Ship::getAllContainersWithDest(const std::string& dest, std::vector<Container*>& res) {
    if (!res.empty()) {
//        errors->push_back("Warning: incorrect use of getAllContainersWithDest");
//        std::cout << "Warning: incorrect use of getAllContainersWithDest" << std::endl;
    }
    for (int level = 0; level < this->plan->getNumFloors(); level++) {
        for (int i = 0; i < this->plan->getLength(); i++) {
            for (int j = 0; j < this->plan->getWidth(); j++) {
                if (this->plan->getFloor(level)->getContainerAtPosition(i, j)->getDest() == dest) {
                    res.push_back(plan->getFloor(level)->getContainerAtPosition(i, j));
                }
            }
        }
    }
    //res may stay empty!
}


bool Ship::isFull() const{
    for (int level = 0; level < this->plan->getNumFloors(); level++) {
        for (int i = 0; i < this->plan->getLength(); i++) {
            for (int j = 0; j < this->plan->getWidth(); j++) {
                if (this->plan->getFloor(level)->getContainerAtPosition(i, j) == nullptr) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool Ship::finishRoute() const{
    return this->route->getRouteLength() == 0;
}

int Ship::numEmptyPlaces() const{
    int counter = 0;
    for (int level = 0; level < this->plan->getNumFloors(); level++) {
        for (int i = 0; i < this->plan->getLength(); i++) {
            for (int j = 0; j < this->plan->getWidth(); j++) {
                if (this->plan->getFloor(level)->getContainerAtPosition(i, j) == nullptr) {
                    counter++;
                }
            }
        }
    }
    return counter;
}