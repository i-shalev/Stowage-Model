//
// Created by zivco on 21/03/2020.
//

#include "Floor.h"

Floor::Floor(int index, int length, int width) {
    this->index = index;
    this->length=length;
    this->width=width;
    this->floorMap = (Container***) malloc(length * sizeof(Container**));
    for(int i=0; i<length; i++){
        floorMap[i] = (Container**) malloc(width * sizeof(Container*));
        for(int j=0; j<width; j++){
            floorMap[i][j] = nullptr;
        }
    }

}
Floor::~Floor() {
    for(int i=0; i<length; i++){
        delete[] floorMap[i];
    }
    delete[] floorMap;
}

Container* Floor::getContainerAtPosition(int i, int j){
    if(i<0 | j<0 | i>length | j>width)
        return nullptr;
    return this->floorMap[i][j];
}

void Floor::setContainerAtPosition(int i, int j, Container* value) {
    if(i<0 | j<0 | i>length | j>width)
        return;
    floorMap[i][j] = value;
}

