//
// Created by zivco on 29/03/2020.
//

#ifndef STOWAGE_MODEL_CRANE_H
#define STOWAGE_MODEL_CRANE_H

#include "Ship.h"
#include <list>

using namespace std;
enum result{ SUCCESS=0, DEST_NOT_IN_LIST, INVALID_INDEX, FULL_PLACE, EMPTY_BELOW, CONTAINER_ABOVE, WRONG_CONTAINER};
class Crane{
    Ship* curShip;
public:
    Crane(Ship* ship): curShip(ship){}
    Crane(const Crane& crane)  = delete;
    result Load(Container* cont, int level, int i, int j);
    result Unload(string contId, int level, int i, int j, Container** answer);
    Ship* getShip(){ return curShip;}
    void setShip(Ship* newShip){ this->curShip=newShip;}
};


#endif //STOWAGE_MODEL_CRANE_H
