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
    int price = 0;
    std::ifstream fin;
    try{
        fin.open(path, std::ios::in);
    } catch (const std::exception& e) {
        errors.push_back("Failed to open file");
        return -1;
    }
    std::string line, id;
    size_t sz, last_index = -1;
    int level, i, j;
    while(getline(fin, line)) {
        if( line.at(0) == 'U'){
            id =  line.substr(3,11) ;
            level = std::stoi(line.substr(16,line.length()-1),&sz);
            last_index = 16 + sz + 2;
            i =  std::stoi(line.substr(last_index,line.length()-1),&sz) ;
            last_index = last_index + sz + 2;
            j =  std::stoi(line.substr(last_index,line.length()-1),&sz) ;
            int rc = this->Unload(id,level,i,j);
            if( rc!= SUCCESS){
                errors.push_back("Unload " + id +" " + std::to_string(level) + " " + std::to_string(i) + " " + std::to_string(j) + " is invalid operation.");
                switch(rc){
                    case 2:
                        errors.emplace_back("Error: index out of range");
                        break;
                    case 5:
                        errors.emplace_back("Error: cant unload container. container above");
                        break;
                    case 6:
                        errors.emplace_back("Error: there is other container in this place");
                        break;
                    case 7:
                        errors.emplace_back("Error: there is no container in this place");
                        break;
                }
                return -1;
            }
            price++;
        }
        else if( line.at(0) == 'L'){
            id =  line.substr(3,11) ;
            level = std::stoi(line.substr(16,line.length()-1),&sz);
            last_index = 16 + sz + 2;
            i =  std::stoi(line.substr(last_index,line.length()-1),&sz) ;
            last_index = last_index + sz + 2;
            j =  std::stoi(line.substr(last_index,line.length()-1),&sz) ;
            int rc = this->Load(id,level,i,j);
            if(rc != SUCCESS){
                errors.push_back("Load " + id +" " + std::to_string(level) + " " + std::to_string(i) + " " + std::to_string(j) + " is invalid operation.");
                switch(rc){
                    case 1:
                        errors.emplace_back("Error: cant load the container because the ship will not visit its destination");
                       break;
                    case 2:
                        errors.emplace_back("Error: index out of range");
                        break;
                    case 3:
                        errors.emplace_back("Error: cant load container. the location is full");
                        break;
                    case 4:
                        errors.emplace_back("Error: cant load container. there is nothing below" );
                        break;
                    case 6:
                        errors.emplace_back("Error: cant find the container, wrong ID");
                        break;
                }
                return -1;
            }
            price++;
        }
        else if( line.at(0) == 'M'){
            id =  line.substr(3,11) ;
            level = std::stoi(line.substr(16,line.length()-1),&sz);
            last_index = 16 + sz + 2;
            i =  std::stoi(line.substr(last_index,line.length()-1),&sz) ;
            last_index = last_index + sz + 2;
            j =  std::stoi(line.substr(last_index,line.length()-1),&sz) ;
            last_index = last_index + sz + 2;
            int toLevel =  std::stoi(line.substr(last_index,line.length()-1),&sz) ;
            last_index = last_index + sz + 2;
            int toI =  std::stoi(line.substr(last_index,line.length()-1),&sz) ;
            last_index = last_index + sz + 2;
            int toJ =  std::stoi(line.substr(last_index,line.length()-1),&sz) ;
            int rc = this->Move(id,level,i,j, toLevel, toI, toJ);
            if(rc != SUCCESS){
                errors.push_back("Move " + id +" " + std::to_string(level) + " " + std::to_string(i) + " " + std::to_string(j)+
                " " + std::to_string(toLevel) + " " + std::to_string(toI) + " " + std::to_string(toJ) + " is invalid operation.");
                switch(rc){
                    case 1:
                        errors.emplace_back("Error: cant load the container because the ship will not visit its destination");
                        break;
                    case 2:
                        errors.emplace_back("Error: index out of range");
                        break;
                    case 3:
                        errors.emplace_back("Error: cant load container. the location is full");
                        break;
                    case 4:
                        errors.emplace_back("Error: cant load container. there is nothing below" );
                        break;
                    case 5:
                        errors.emplace_back("Error: cant unload container. container above");
                        break;
                    case 6:
                        errors.emplace_back("Error: there is other container in this place");
                        break;
                    case 7:
                        errors.emplace_back("Error: there is no container in this place");
                        break;
                }
                return -1;
            }
            price++;
        }
        else if( line.at(0) == 'R'){
            id =  line.substr(3,11);
        }
        else{
            errors.emplace_back("wrong file format");
        }
    }
    fin.close();
    return price;
}

