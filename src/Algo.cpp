//
// Created by zivco on 05/04/2020.
//

#include "Algo.h"
void Algo::getInstructionForCargo(Port &port) {
    vector<Container*> temporaryUnloaded;
    ship->getPlan().printShipPlan();
    //first unload from ship all the containers with this destination
    for(int i=0; i<ship->getPlan().getLength();i++){
        for(int j=0; j<ship->getPlan().getWidth(); j++){
            int lowestFloor = -1;
            for(int level=0;level<ship->getPlan().getNumFloors(); level++){
                if(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j) == nullptr)
                    break;
                if(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getDest().compare(ship->getCurrentDestination())==0){
                    //we need to unload all containers above
                    lowestFloor = level;
                    break;
                }
                else{
                    std::cout << ship->getCurrentDestination() << std::endl;
                }
            }
            if(lowestFloor == -1) // no need to unload this tower
                continue;
            for(int level = ship->getPlan().getNumFloors()-1; level>=lowestFloor; level --){
                if(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j) == nullptr)
                    continue;
                if(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getBlocked())
                    break;
                std::cout << "U "<< ship->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getId() << " " << level << " " << i << " " << j <<std::endl;
                if(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getDest().compare(ship->getCurrentDestination())!=0){
                    temporaryUnloaded.push_back(ship->getPlan().getFloor(level)->getContainerAtPosition(i,j));
                }
            }
            int loadBackLevel=0;
            while(ship->getPlan().getFloor(loadBackLevel)->getContainerAtPosition(i,j)->getBlocked()){
                loadBackLevel++;
            }
            while(!(temporaryUnloaded.empty())){
                std::cout << "U "<< ship->getPlan().getFloor(loadBackLevel)->getContainerAtPosition(i,j)->getId() << " " << loadBackLevel << " " << i << " " << j <<std::endl;
                loadBackLevel++;
            }
        }
    }

    //now load everything from port to ship. check valid ids and destinations.
}