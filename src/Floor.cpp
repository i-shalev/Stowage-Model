//
// Created by zivco on 21/03/2020.
//

#include "Floor.h"

Floor::Floor(int index, int length, int width, vector<vector<int>> blocks) {
    this->index = index;
    this->length=length;
    this->width=width;
    this->floorMap = new vector<vector<Container*>>(length, vector<Container*>(width, nullptr));
    int wid, len;
    for(auto & block : blocks) {
        wid = block.at(0);
        len = block.at(1);
        if(wid!=-1 & len!=-1)
            floorMap->at(wid).at(len) = new Container(-1, "blocked", "blocked", true);
    }


}
Floor::~Floor() {
    delete floorMap;
}

Container* Floor::getContainerAtPosition(int i, int j){
    if(i<0 | j<0 | i>length | j>width)
        return nullptr;
    return (*this->floorMap)[i][j];
}

void Floor::setContainerAtPosition(int i, int j, Container* value) {
    if(i<0 | j<0 | i>length | j>width)
        return;
    (*floorMap)[i][j] = value;
}

