//
// Created by itay on 22/03/2020.
//

#include "ShipPlan.h"

ShipPlan::ShipPlan(int numFloors, int length, int width, int*** blocks)
{
    int** floorBlocks;
    this->numFloors = numFloors;
    this->length = length;
    this->width = width;
    this->floors = (Floor**) malloc(numFloors * sizeof(Floor*));

    for(int i=0; i<numFloors; i++)
    {
        floorBlocks = blocks[i];
        this->floors[i] = new Floor(i, length, width, floorBlocks);
    }
}

ShipPlan::~ShipPlan() {
    delete[] this->floors;
}

int ShipPlan::getNumFloors(){
    return this->numFloors;
}

int ShipPlan::getLength(){
    return this->length;
}

int ShipPlan::getWidth(){
    return this->width;
}

Floor* ShipPlan::getFloor(int i){
    return this->floors[i];
}

bool ShipPlan::isValid(){
    bool free;
    for(int i=0; i<length; i++){
        for(int j=0; j<width; j++){
            free = false;
            for(int level=0; level<numFloors; level++){
                Container* current = this->getFloor(level)->getContainerAtPosition(i,j);
                if(!free){
                    if(current== nullptr || !(current->getBlocked()))
                        free = true;
                }
                else{
                    if(current != nullptr && current->getBlocked())
                        return false;
                }
            }
        }
        return true;
    }
}