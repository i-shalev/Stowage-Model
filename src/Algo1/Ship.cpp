//
// Created by zivco on 26/03/2020.
//

#include "Ship.h"

#include <utility>

Ship::Ship(ShipRoute *sr, ShipPlan *sp, std::map<std::string, Port *> *mapPortToPort, std::vector<std::string> *_errors)
        : route(sr), plan(sp), mapPortNameToPort(mapPortToPort), mapPortNameToNumberOfVisitsUntilNow(new std::map<std::string,int>){
    this->errors = _errors;
    if(!this->plan->isValid()){
        errors->push_back("Warning: invalid plan");
//        std::cout << "invalid plan!" << std::endl;
    }
    addOneVisitToMap();
}

Ship::~Ship() {
    for(const auto& elem : *this->mapPortNameToPort ) {
        delete elem.second;
    }
    this->mapPortNameToPort->clear();
    this->mapPortNameToNumberOfVisitsUntilNow->clear();
    delete this->mapPortNameToPort;
    delete this->plan;
    delete this->route;
    delete this->mapPortNameToNumberOfVisitsUntilNow;
}

ShipPlan& Ship::getPlan() { return *(this->plan);}
//const ShipRoute& Ship::getRoute() {return *(this->route);}
bool Ship::willVisit(std::string dest) const {
    return this->route->willVisit(std::move(dest));
}

void Ship::getContainerPosition(const std::string& id, std::vector<int>& res){
    if(!res.empty()){
        errors->push_back("Warning: incorrect use of getContainerPosition");
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
        errors->push_back("Warning: incorrect use of getAllContainersWithDest");
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

void Ship::moveToNextPort(){
    if(finishRoute())
        return;
    this->route->deleteFirst();
    if(!finishRoute())
        addOneVisitToMap();
}

void Ship::addOneVisitToMap(){
    int ans = 0;
    auto res = mapPortNameToNumberOfVisitsUntilNow->find(this->route->getHead());
    if(!mapPortNameToNumberOfVisitsUntilNow->empty() && res!=mapPortNameToNumberOfVisitsUntilNow->end()){
        ans = res->second+1;
        mapPortNameToNumberOfVisitsUntilNow->erase(this->route->getHead());
    }
    mapPortNameToNumberOfVisitsUntilNow->insert({this->route->getHead(), ans});
}

int Ship::getIndexOfPort() const{
    auto res = mapPortNameToNumberOfVisitsUntilNow->find(this->route->getHead());
    return res->second;
}

Port* Ship::getCurrentPort() const{
    int index = getIndexOfPort();
    std::string fullName = this->route->getHead() + "_" + std::to_string(index);
    auto res = mapPortNameToPort->find(fullName);
    if(!mapPortNameToPort->empty() && res!=mapPortNameToPort->end()){
        return res->second;
    }
    return nullptr;
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