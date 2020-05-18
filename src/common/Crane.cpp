//
// Created by zivco on 29/03/2020.
//

#include "Crane.h"

result Crane::Load(const std::string& contId, int level, int i, int j) {
    Container* cont = curPort->getContainerByID(contId);
    //check the container target is in the ship's destination list
    if(cont == nullptr)
        return WRONG_CONTAINER;
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
    curPort->removeContainer(contId);
    return SUCCESS;

}

result Crane::Unload(const std::string& contId, int level, int i, int j) {
    //check index
    if(level > curShip->getPlan().getNumFloors() || level < 0 ||
       i > curShip->getPlan().getLength() || i < 0 ||
       j > curShip->getPlan().getWidth() || j < 0){
        return INVALID_INDEX;
    }
    if(curShip->getPlan().getFloor(level)->getContainerAtPosition(i,j) == nullptr){
        return NOTHING_HERE;
    }
    if(curShip->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getBlocked()){
        return INVALID_INDEX; //cant remove blocking container
    }
    //check if there is something below
    if(level < curShip->getPlan().getNumFloors() - 1 && curShip->getPlan().getFloor(level+1)->getContainerAtPosition(i,j) != nullptr)
        return CONTAINER_ABOVE;
    if(contId!=curShip->getPlan().getFloor(level)->getContainerAtPosition(i,j)->getId())
        return WRONG_CONTAINER;
    Container* answer = curShip->getPlan().getFloor(level)->getContainerAtPosition(i,j);
    if((answer)->getDest() != curShip->getCurrentDestination()) {
        curPort->addContainer(answer);
    }
    else
        delete answer;
    curShip->getPlan().getFloor(level)->setContainerAtPosition(i,j,nullptr);
    return SUCCESS;
}

result Crane::Move(const std::string& contId, int level, int i, int j, int toLevel, int toI, int toJ){
    result res = this->Unload(contId, level, i, j);
    if( res!= SUCCESS){
        return res;
    }
    res = this->Load(contId, toLevel, toI, toJ);
    if(res != SUCCESS){
        this->Load(contId, level,i,j); //back to origin
        return res;
    }
    return SUCCESS;
}

int Crane::executeOperationList(const std::string& path, std::vector<std::string>& errors) {
    int price = 0; std::ifstream fin;
    try{ fin.open(path, std::ios::in);}
    catch (const std::exception& e) {errors.push_back("Failed to open file"); return -1;}
    std::string line, id; int level, i, j;
    while(getline(fin, line)) {
        std::vector<std::string> vec;
        getArgsFromInstruction(vec, line);
        if(vec.size() != 2 and vec.size() != 5 and vec.size() != 8){
            errors.emplace_back("Error: instruction wrong format (wrong number of parameters in line).");
            return -1;
        }
        if(vec.size() == 2 && vec.at(0) == "R"){
            id =  vec.at(1);
            if(!(curPort->wasContainerWithID(id))){errors.emplace_back("Error: bad instruction (reject non exist id)."); return -1;}
        } else if (vec.size() == 5) {
            id =  vec.at(1);
            try{
                level = std::stoi(vec.at(2));
                i =  std::stoi(vec.at(3)) ;
                j =  std::stoi(vec.at(4)) ;
            } catch (const std::exception& e) {
                errors.emplace_back("Error: instruction wrong format (not numbers).");
                return -1;
            }
            if(vec.at(0) == "L"){
                int rc = this->Load(id,level,i,j);
                if(rc != SUCCESS){
                    errors.push_back("Load " + id +" " + std::to_string(level) + " " + std::to_string(i) + " " + std::to_string(j) + " is invalid operation.");
                    switch(rc){
                        case 1:
                            errors.emplace_back("Error: cant load the container because the ship will not visit its destination"); break;
                        case 2:
                            errors.emplace_back("Error: index out of range"); break;
                        case 3:
                            errors.emplace_back("Error: cant load container. the location is full"); break;
                        case 4:
                            errors.emplace_back("Error: cant load container. there is nothing below" ); break;
                        case 6:
                            errors.emplace_back("Error: cant find the container, wrong ID"); break;
                    }
                    return -1;
                }
                price++;
            } else if(vec.at(0) == "U"){
                int rc = this->Unload(id,level,i,j);
                if( rc!= SUCCESS){
                    errors.push_back("Unload " + id +" " + std::to_string(level) + " " + std::to_string(i) + " " + std::to_string(j) + " is invalid operation.");
                    switch(rc){
                        case 2:
                            errors.emplace_back("Error: index out of range"); break;
                        case 5:
                            errors.emplace_back("Error: cant unload container. container above"); break;
                        case 6:
                            errors.emplace_back("Error: there is other container in this place"); break;
                        case 7:
                            errors.emplace_back("Error: there is no container in this place"); break;
                    }
                    return -1;
                }
                price++;
            } else {
                errors.emplace_back("Error: instruction wrong format (invalid instruction).");
                return -1;
            }
        } else if (vec.size() == 8 and vec.at(0) == "M") {
            int toLevel, toI, toJ;
            try{
                level = std::stoi(vec.at(2));
                i =  std::stoi(vec.at(3)) ;
                j =  std::stoi(vec.at(4)) ;
                toLevel = std::stoi(vec.at(5)) ;
                toI = std::stoi(vec.at(6)) ;
                toJ = std::stoi(vec.at(7)) ;
            } catch (const std::exception& e) {
                errors.emplace_back("Error: instruction wrong format (not numbers).");
                return -1;
            }
            int rc = this->Move(id,level,i,j, toLevel, toI, toJ);
            if(rc != SUCCESS){
                errors.push_back("Move " + id +" " + std::to_string(level) + " " + std::to_string(i) + " " + std::to_string(j)+
                                 " " + std::to_string(toLevel) + " " + std::to_string(toI) + " " + std::to_string(toJ) + " is invalid operation.");
                switch(rc){
                    case 1:
                        errors.emplace_back("Error: cant load the container because the ship will not visit its destination"); break;
                    case 2:
                        errors.emplace_back("Error: index out of range"); break;
                    case 3:
                        errors.emplace_back("Error: cant load container. the location is full"); break;
                    case 4:
                        errors.emplace_back("Error: cant load container. there is nothing below" ); break;
                    case 5:
                        errors.emplace_back("Error: cant unload container. container above"); break;
                    case 6:
                        errors.emplace_back("Error: there is other container in this place"); break;
                    case 7:
                        errors.emplace_back("Error: there is no container in this place"); break;
                }
                return -1;
            }
            price++;
        } else{ errors.emplace_back("wrong file format");}
    }
    fin.close();
    return price;
}

