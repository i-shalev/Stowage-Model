//
// Created by zivco on 29/03/2020.
//

#include "Crane.h"

result Crane::Load(string contId, int level, int i, int j) {
    /*---------------need to implement! -------------------------*/
    Container* cont; // = getContainerByIdFromCurrentPort(contId);
    /*---------------need to implement! -------------------------*/



    //check the container target is in the ship's destination list
    if(!curShip->willVisit(cont->getDest())){
        return DEST_NOT_IN_LIST;
    }

    // check the place is OK, i.e - legal, empty and not empty below.
    //check valid indexes
    if(level > curShip->getPlan().getNumFloors() || level < 0 ||
        i > curShip->getPlan().getLength() || i < 0 ||
        j > curShip->getPlan().getWidth() || j < 0){
        return INVALID_INDEX;
    }

    //check if the place is empty
    if(curShip->getPlan().getFloor(level)->getContainerAtPosition(i,j) != nullptr) //blocking container or other container
        return FULL_PLACE;
    //check if there is something below
    if(level > 0 && curShip->getPlan().getFloor(level-1)->getContainerAtPosition(i,j) == nullptr)
        return EMPTY_BELOW;

    //load the container
    curShip->getPlan().getFloor(level)->setContainerAtPosition(i,j,cont);
    return SUCCESS;

}

result Crane::Unload(string contId, int level, int i, int j, Container** answer) {
    //check index
    if(level > curShip->getPlan().getNumFloors() || level < 0 ||
       i > curShip->getPlan().getLength() || i < 0 ||
       j > curShip->getPlan().getWidth() || j < 0){
        return INVALID_INDEX;
    }
    if(curShip->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getBlocked()){
        return INVALID_INDEX; //cant remove blocking container
    }
    //check if there is something below
    if(level < curShip->getPlan().getNumFloors() - 1 && curShip->getPlan().getFloor(level+1)->getContainerAtPosition(i,j) != nullptr)
        return CONTAINER_ABOVE;
    if(contId.compare(curShip->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getId())!=0)
        return WRONG_CONTAINER;
    *answer = curShip->getPlan().getFloor(level)->getContainerAtPosition(i,j);
    curShip->getPlan().getFloor(level)->setContainerAtPosition(i,j,nullptr);
    return SUCCESS;
}
result Crane::Move(string id, int level, int i, int j, int toLevel, int toi, int toj) {
   Container* result;
   int rc = Unload(id, level, i, j, &result) !=SUCCESS;
   if(rc !=SUCCESS)
       return static_cast<enum result>(rc);
   return Load(id,toLevel, toi, toj);
}

int Crane::executeOperationList(const string& path) {
    int price = 0;
    ifstream fin;
    try{
        fin.open(path, ios::in);
    } catch (const std::exception& e) {
        std::cout << "ERROR: Failed to open file" << std::endl;
        return false;
    }
    string line, id;
    size_t sz;
    size_t last_index = -1;
    int level, i, j;
    while(getline(fin, line)) {
        if( line.at(0) == 'U'){
            std::cout << "got unload" << std::endl;
            id =  line.substr(2,11) ;
            level = std::stoi(line.substr(14,line.length()-1),&sz);
            last_index = 14 + sz + 1;
            i =  std::stoi(line.substr(last_index,line.length()-1),&sz) ;
            last_index = last_index + sz + 1;
            j =  std::stoi(line.substr(last_index,line.length()-1),&sz) ;
            Container* ans; // actually redundant....
            if(this->Unload(id,level,i,j,&ans) != SUCCESS)
                return -1;
            price++;
        }
        else if( line.at(0) == 'L'){
            std::cout << "got unload" << std::endl;
            id =  line.substr(2,11) ;
            level = std::stoi(line.substr(14,line.length()-1),&sz);
            last_index = 14 + sz + 1;
            i =  std::stoi(line.substr(last_index,line.length()-1),&sz) ;
            last_index = last_index + sz + 1;
            j =  std::stoi(line.substr(last_index,line.length()-1),&sz) ;
            if(this->Load(id,level,i,j) != SUCCESS)
                return -1;
            price++;
        }
        else if( line.at(0) == 'R'){
            std::cout << "got reject" << std::endl;
            id =  line.substr(2,11);
            price++; // TODO: make sure it really costs...
        }
        else{
            std::cout << "wrong file format" << std::endl;
        }
    }
    return price;
}

