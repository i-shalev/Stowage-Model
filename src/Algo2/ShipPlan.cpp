//
// Created by itay on 22/03/2020.
//

#include "ShipPlan.h"
void printvec(const std::vector<std::vector<int>> vec){
    for(size_t i=0; i<vec.size(); i++){
        for(size_t j=0; j<vec.at(i).size(); j++)
            std::cout << vec.at(i).at(j) << ",";
        std::cout << std::endl;
    }
}
ShipPlan::ShipPlan(int numFloors, int length, int width, const std::vector<std::vector<int>>& blocks, std::vector<std::string>* _errors)
{
    std::vector<std::vector<int>> floorBlocks; //unused
    this->numFloors = numFloors;
    this->length = length;
    this->width = width;
    this->floors = (Floor**) malloc(numFloors * sizeof(Floor*));
    this->errors = _errors;

    for(int i=0; i<numFloors; i++)
    {
        this->floors[i] = new Floor(i, length, width/*, floorBlocks*/);
    }
    for(auto tripleVec : blocks){
        if(tripleVec.at(0)==-1 || tripleVec[1]==-1 || tripleVec[2]==-1){
            errors->push_back("Warning: problem while reading line in ShipPlan file");
//            std::cout << "Warning: problems while reading file" << std::endl;
            continue;
        }
        if(tripleVec[0] >= length || tripleVec[1] >= width || tripleVec[2] >= numFloors) {
            errors->push_back("Warning: too big numbers in ShipPlan file");
//            std::cout << "Warning: too big numbers in file" << std::endl;
            continue;
        }
        for(int i=0; i<numFloors - tripleVec[2]; i++){
            floors[i]->setContainerAtPosition(tripleVec[0], tripleVec[1], blockingContainer);
        }
    }
}

ShipPlan::~ShipPlan() {
    delete blockingContainer;
    for(int i=0; i<numFloors; i++) {
       delete this->floors[i];
    }
    delete[] this->floors;
}

int ShipPlan::getNumFloors() const{
    return this->numFloors;
}

int ShipPlan::getLength() const{
    return this->length;
}

int ShipPlan::getWidth() const{
    return this->width;
}

Floor* ShipPlan::getFloor(int i) const{
    return this->floors[i];
}

bool ShipPlan::isValid(){
    bool free;
    for(int i=0; i<length; i++){
        for(int j=0; j<width; j++){
            free = false;
            for(int level=0; level<numFloors; level++){
                Container* current = this->getFloor(level)->getContainerAtPosition(i,j);
                if(!free){
                    if(current== nullptr || !(current->getBlocked()))
                        free = true;
                }
                else{
                    if(current != nullptr && current->getBlocked())
                        return false;
                }
            }
        }
    }
    return true;
}

void ShipPlan::printShipPlan() const{
    std::cout << "num Floors: " << this->numFloors  << std::endl;
    std::cout << "length: "     << this->length     << std::endl;
    std::cout << "width: "      << this->width      << std::endl;
    for (int i=0; i < numFloors; i++) {
        std::cout << "Floor "     << i << ": ";
        for(int j=0; j < length ; j++ ) {
            for(int k=0; k < width; k++){
                Container* container = this->getFloor(i)->getContainerAtPosition(j, k);
                if(container != nullptr) {
                    if(container->getBlocked()) {
                        std::cout << " (" << j << "," << k << ") - blocked,  ";
                    } else {
                        std::cout << " (" << j << "," << k << ") - " << container->getId() << " to " << container->getDest() << std::endl;
                    }
                }
            }
        }
        std::cout <<  std::endl;
    }
}