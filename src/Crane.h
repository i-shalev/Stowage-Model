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
    std::vector<std::string>* errors;
    std::set<std::string> temporaryUnloaded;
    std::set<std::string> destinationsOfLoadedContainers;

public:
    Crane(Ship *ship, std::vector<std::string> *_errors) : curShip(ship), errors(_errors){}
    Crane(){}
    Crane(const Crane& crane)  = delete;
    result Load(const std::string& contId, int level, int i, int j);
    result Unload(const std::string& contId, int level, int i, int j);
    result Move(/*string id, int level, int i, int j, int toLevel, int toi, int toj*/);
    Ship* getShip() const { return curShip;}
    void setShip(Ship* newShip){ this->curShip=newShip;}
    int executeOperationList(const std::string& pathToFile);
    bool disconnect();
};


#endif //STOWAGE_MODEL_CRANE_H
