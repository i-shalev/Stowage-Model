//
// Created by itay on 22/03/2020.
//

#include "ShipPlan.h"

ShipPlan::ShipPlan(int numFloors, int length, int width)
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

