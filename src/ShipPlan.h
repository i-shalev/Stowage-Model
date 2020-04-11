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
        Floor** floors = nullptr;
        Container* blockingContainer = new Container(-1,"BLOCK", "BLOCK", true);
        vector<string>* errors;

    public:
        ShipPlan(int numFloors, int length, int width, const vector<vector<int>> &blocks, vector<string> *_errors);
        ShipPlan(const ShipPlan& s) = delete; // disable copy constr'. need deep copy
        ~ShipPlan();
        int getNumFloors() const;
        int getLength() const;
        int getWidth() const;
        Floor* getFloor(int i) const;
        void printShipPlan() const;

    bool isValid();
};


#endif //STOWAGE_MODEL_SHIPPLAN_H
