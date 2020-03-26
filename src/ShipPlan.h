//
// Created by itay on 22/03/2020.
//
#include <vector>
#include "Floor.h"

#ifndef STOWAGE_MODEL_SHIPPLAN_H
#define STOWAGE_MODEL_SHIPPLAN_H

using namespace std;
class ShipPlan {
    private:
        int numFloors;
        int  length;
        int width;
        Floor** floors;

    public:
        ShipPlan(int numFloors, int length, int width);
        ~ShipPlan();
        int getNumFloors();
        int getLength();
        int getWidth();
        Floor* getFloor(int i);

};


#endif //STOWAGE_MODEL_SHIPPLAN_H
