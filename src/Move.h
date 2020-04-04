//
// Created by zivco on 04/04/2020.
//

#ifndef STOWAGE_MODEL_MOVE_H
#define STOWAGE_MODEL_MOVE_H

#include "Container.h"
enum operation{LOAD=0, UNLOAD=1, NEXT_DEST=2};

class Move{
    operation opcode;
    Container* toLoad;
    std::string idToUnload;
    int level;
    int i;
    int j;
    //seperate constr' to load/unload/move
    Move(){ this->opcode=NEXT_DEST;}
    Move(const string& idToUnload, int level, int i, int j){
        this->opcode = UNLOAD;
        this->i = i;
        this->j = j;
        this->idToUnload=idToUnload;
    }
    Move(Container* toLoad, int level, int i, int j){
        this->opcode = LOAD;
        this->i = i;
        this->j = j;
        this->toLoad=toLoad;
    }
};


#endif //STOWAGE_MODEL_MOVE_H
