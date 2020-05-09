//
// Created by zivco on 29/03/2020.
//

#ifndef STOWAGE_MODEL_CRANE_H
#define STOWAGE_MODEL_CRANE_H

#include "Ship.h"
#include <list>
#include <fstream>
#include <set>

//using namespace std;
enum result{ SUCCESS=0, DEST_NOT_IN_LIST=1, INVALID_INDEX=2, FULL_PLACE=3, EMPTY_BELOW=4, CONTAINER_ABOVE=5, WRONG_CONTAINER=6};
class Crane{
    Ship* curShip;
    Port* curPort;

public:
    Crane(Ship *ship, Port* port) : curShip(ship), curPort(port){}
    Crane(const Crane& crane)  = delete;
    result Load(const std::string& contId, int level, int i, int j);
    result Unload(const std::string& contId, int level, int i, int j);
    //result Move(/*string id, int level, int i, int j, int toLevel, int toi, int toj*/);
    int executeOperationList(const std::string& path, std::vector<std::string> errors);
};


#endif //STOWAGE_MODEL_CRANE_H
