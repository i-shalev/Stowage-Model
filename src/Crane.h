//
// Created by zivco on 29/03/2020.
//

#ifndef STOWAGE_MODEL_CRANE_H
#define STOWAGE_MODEL_CRANE_H

#include "Ship.h"
#include <list>

using namespace std;
enum result{ SUCCESS=0, DEST_NOT_IN_LIST=1, INVALID_INDEX=2, FULL_PLACE=3, EMPTY_BELOW=4, CONTAINER_ABOVE=5, WRONG_CONTAINER=6};
class Crane{
    Ship* curShip;
public:
    Crane(Ship* ship): curShip(ship){}
    Crane(){}
    Crane(const Crane& crane)  = delete;
    result Load(Container* cont, int level, int i, int j);
    result Unload(string contId, int level, int i, int j, Container** answer);
    Ship* getShip(){ return curShip;}
    void setShip(Ship* newShip){ this->curShip=newShip;}
};


#endif //STOWAGE_MODEL_CRANE_H
