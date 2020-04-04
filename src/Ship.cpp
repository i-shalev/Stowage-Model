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

void Ship::getContainerPosition(const string& id, std::vector<int>& res){
    if(res.size()!=0){
        std::cout << "Warning: incorrect use of getContainerPosition" << std::endl;
    }
    for(int level=0; level<this->plan->getNumFloors(); level++){
        for(int i=0; i<this->plan->getLength(); i++){
            for(int j=0; j<this->plan->getWidth(); j++ ){
                if(this->plan->getFloor(level)->getContainerAtPosition(i,j)->getId().compare(id)==0){
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

void Ship::getAllContainersWithDest(const string& dest, std::vector<Container*>& res) {
    if (res.size() != 0) {
        std::cout << "Warning: incorrect use of getAllContainersWithDest" << std::endl;
    }
    for (int level = 0; level < this->plan->getNumFloors(); level++) {
        for (int i = 0; i < this->plan->getLength(); i++) {
            for (int j = 0; j < this->plan->getWidth(); j++) {
                if (this->plan->getFloor(level)->getContainerAtPosition(i, j)->getDest().compare(dest) == 0) {
                    res.push_back(plan->getFloor(level)->getContainerAtPosition(i, j));
                }
            }
        }
    }
    //res may stay empty!
}
