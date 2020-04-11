//
// Created by zivco on 21/03/2020.
//

#include "Floor.h"

Floor::Floor(int index, int length, int width) {
    this->index = index;
    this->length=length;
    this->width=width;
    this->floorMap = new vector<vector<Container*>>(length, vector<Container*>(width, nullptr));
}
Floor::~Floor() {
//    std::cout << "Delete Floor: " << this->index << std::endl;
    delete floorMap;
}

Container* Floor::getContainerAtPosition(int i, int j) const{
    if(i < 0 or j < 0 or i > length or j > width)
        return nullptr;
    return (*this->floorMap)[i][j];
}

void Floor::setContainerAtPosition(int i, int j, Container* value) {
    if(i < 0 or j < 0 or i > length or j > width)
        return;
    //you can set if the place is empty, or if the place is full set only to empty
    if(floorMap->at(i).at(j) == nullptr || (!floorMap->at(i).at(j)->getBlocked() && value == nullptr))
        floorMap->at(i).at(j) = value;
}

