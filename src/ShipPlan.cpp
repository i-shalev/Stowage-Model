//
// Created by itay on 22/03/2020.
//

#include "ShipPlan.h"

ShipPlan::ShipPlan(int numFloors, int length, int width, int*** blocks)
{
    this->numFloors = numFloors;
    this->length = length;
    this->width = width;
    this->floors = (Floor**) malloc(numFloors * sizeof(Floor*));

    for(int i=0; i<numFloors; i++)
    {
        this->floors[i] = new Floor(i, length, width);
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
                if(!free){
                    if(!(this->getFloor(level)->getContainerAtPosition(i,j)->getBlocked()))
                        free = true;
                }
                else{
                    if(this->getFloor(level)->getContainerAtPosition(i,j)->getBlocked())
                        return false;
                }
            }
        }
        return true;
    }
}